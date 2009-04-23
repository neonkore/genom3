<?
import string; from string import *;
abortRequestNum = len(comp.servicesMap()) + 1;

# try to find an init service
initRqstNb = -1
i=-1
for s in comp.servicesMap():
    i +=1
    if s.data().type == ServiceType.Init:
      initRqstNb = i
?>
/* --- FILE GENERATED BY GENOM, DO NOT EDIT BY HAND ------------------- */

/* 
 *      Autonomous Systems Lab, Swiss Federal Institute of Technology.
 * Copyright (c) 1993-2005 LAAS/CNRS
 * All rights reserved.
 *
 * Redistribution and use  in source  and binary  forms,  with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   1. Redistributions of  source  code must retain the  above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice,  this list of  conditions and the following disclaimer in
 *      the  documentation  and/or  other   materials provided  with  the
 *      distribution.
 *
 * THIS  SOFTWARE IS PROVIDED BY  THE  COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND  ANY  EXPRESS OR IMPLIED  WARRANTIES,  INCLUDING,  BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES  OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR  PURPOSE ARE DISCLAIMED. IN  NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR      CONTRIBUTORS  BE LIABLE FOR   ANY    DIRECT, INDIRECT,
 * INCIDENTAL,  SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE   OF THIS SOFTWARE, EVEN   IF ADVISED OF   THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

/*
 * Control task body
 */

#if defined(__RTAI__) && defined(__KERNEL__)
# define exit(x)	taskDelete(0)
#else
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
#endif

#include <portLib.h>

#include "<!comp.name()!>Header.h"

#include <taskLib.h> 
#include <errnoLib.h>
#include <commonStructLib.h>
#include <h2evnLib.h>
#include <h2timerLib.h>

#include "<!comp.name()!>MsgLib.h"
#include "genom/moduleEvents.h"

/* Print debugging information */
#define GENOM_DEBUG_CNTRLTASK

#ifdef GENOM_DEBUG_CNTRLTASK
# define LOGDBG(x)	logMsg x
#else
# define LOGDBG(x)
#endif

/*-------------------- VARIABLES IMPORTEES ---------------------------------*/

/* Semaphores d'initialisation et de debug */
extern SEM_ID sem<!comp.name()!>CntrlTaskInit;   

extern char pidFilePath[];

/*-------------------- VARIABLES GLOBALES ---------------------------------*/

/* Nombre d'activites ZOMBIE */
/*XXXXX Voila une variable qu'il serait judicieux de mettre de la SDI de controle a l'occasion XXXXX*/
static int <!comp.name()!>NbZombie=0;
static POSTER_ID <!comp.name()!>CntrlPosterId;            /* Poster de controle */
static int <!comp.name()!>LastActivityNum=-1;
static int <!comp.name()!>LastAbsolutActivityNum=-1;
<?
# $execTaskNameTabDeclare$
write("static char *" + comp.name() + "ExecTaskNameTab[] = {\n");
task_list = ""
for t in comp.tasksMap():
    task_list += "\"" + t.data().name + "\",\n"
print task_list[:-2] # remove the last ',' 
print "};"
?>
static BOOL <!comp.name()!>SignalAbort;

#define TIMEOUT_CNTRL_TASK 2000 /* 10 sec (pas encore utilise) */

static MODULE_EVENT_STR moduleEventCntrl;

/*---------------- PROTOTYPES DES FONCTIONS LOCALES ------------------------*/

static void   <!comp.name()!>CntrlIntEvnExec   (SERV_ID ServId);
static void   <!comp.name()!>CntrlExtEvnExec   (SERV_ID ServId);
static void   <!comp.name()!>SpawnActivities   (SERV_ID servId);
static void   <!comp.name()!>CntrlPosterWrite  (void);
static BOOL   <!comp.name()!>AbortActivity     (SERV_ID servId, int activity);
static STATUS <!comp.name()!>CntrlInitTask     (SERV_ID *ServId);
static int    allocActivity             (int rqstId, 
					 int requestNum, 
					 int execTaskNum, 
					 int reentrantFlag,
					 SERV_ID servId);
static void   freeActivity              (int activity);
static BOOL   controlExecTaskStatus(SERV_ID servId, int rqstId);
static BOOL   controlExecTaskStatusAndInitRqst(SERV_ID servId, int rqstId);
static void   <!comp.name()!>SendFinalReply    (SERV_ID servId, int activity);
static void   <!comp.name()!>RqstAbortActivity (SERV_ID ServId, int rqstId);
static void   <!comp.name()!>CntrlTaskSuspend  (BOOL giveFlag);
static void   <!comp.name()!>ReplyAndSuspend   (SERV_ID servId, int rqstId, 
					 BOOL giveFlag);      
static void   <!comp.name()!>SignalHandler(int);
static void   <!comp.name()!>SignalEnd(void);

/*---------------------- FONCTION EXPORTEE ---------------------------------*/

/*****************************************************************************
 *
 *   <!comp.name()!>CntrlTask - Tache de controle du module <!upper(comp.name())!>
 *
 *   Description:
 *   Cette tache attend suspendue l'arrivee de messages dans sa boite de
 *   reception ou d'evenements internes. A la reception d'une lettre,
 *   la requete correspondante est traitee. Les changement d'etat internes 
 *   sont pris en compte et les repliques finales sont emises au client.
 *   Cette tache boucle a jamais.
 */

void
<!comp.name()!>CntrlTask()
{
  static SERV_ID <!comp.name()!>ServId;              /* Id du serveur */

  /* Routine d'initialisation */
  CNTRL_TASK_STATUS = <!comp.name()!>CntrlInitTask (&<!comp.name()!>ServId);
  if (CNTRL_TASK_STATUS == ERROR)
     CNTRL_TASK_BILAN = errnoGet();
  else
     CNTRL_TASK_BILAN = OK;
  
  /* Donner le sem de fin d'initialisation */
  semGive (sem<!comp.name()!>CntrlTaskInit) ;

  /* Se suspendre en cas de probleme */
  if (CNTRL_TASK_STATUS == ERROR) {
     <!comp.name()!>CntrlTaskSuspend(FALSE);
  }

  moduleEventCntrl.moduleNum = <!str(comp.uniqueId)!>;

  /* Record a signal handler */
  <!comp.name()!>SignalAbort = FALSE;
  signal(SIGTERM, <!comp.name()!>SignalHandler);
  /* Boucler indefiniment */
  FOREVER
    {
      /* Bloquer dans l'attente d'evenements internes et externes */
      if (h2evnSusp (0) != TRUE)
	 <!comp.name()!>CntrlTaskSuspend (FALSE);

      /* Prendre l'acces aux SDI */
      commonStructTake ((void *) <!comp.name()!>CntrlStrId);
      commonStructTake ((void *) <!comp.name()!>DataStrId);
 
      if (<!comp.name()!>SignalAbort) {
	      <!comp.name()!>SignalEnd();
      }
      /* Traiter les evenements internes */
      <!comp.name()!>CntrlIntEvnExec (<!comp.name()!>ServId);

      /* Traiter les evenements externes */
      <!comp.name()!>CntrlExtEvnExec (<!comp.name()!>ServId);

      /* Lancer les activites en attentes */
      <!comp.name()!>SpawnActivities(<!comp.name()!>ServId);

      /* Mettre a jour le poster de controle */
      <!comp.name()!>CntrlPosterWrite();

      CNTRL_TASK_BILAN = errnoGet();

     /* Liberer l'acces aux SDI */
      commonStructGive ((void *) <!comp.name()!>DataStrId);
      commonStructGive ((void *) <!comp.name()!>CntrlStrId);

    }
}



/*-------------------- ROUTINES LOCALES ------------------------------------*/


 
/*****************************************************************************
 *
 *  <!comp.name()!>CntrlIntEvnExec  -  Traiter les evenements internes produits
 *                              par les taches d'execution
 */

static void <!comp.name()!>CntrlIntEvnExec(SERV_ID servId)
{
  int activity;

  /* Activites terminees ou activite a reveiller */
  for (activity = 0;  activity < MAX_ACTIVITIES; activity++) {

    /* Activite terminee */
    if (ACTIVITY_EVN(activity) == ETHER) {
      
      /* Est-ce l'activité d'init ? */
      if (INIT_RQST != -1)
	if (ACTIVITY_RQST_TYPE(activity) == INIT_RQST)
	  if (ACTIVITY_BILAN(activity) == OK)
	    INIT_RQST = -1;

      /* On envoie la replique finale */
      <!comp.name()!>SendFinalReply (servId, activity);
      
      /* On libere l'activite */
      freeActivity (activity);
    }
    
    /* Activite suspendue */
    else if (ACTIVITY_EVN(activity) == ZOMBIE) {
      
      /* On envoie la replique finale */
      <!comp.name()!>SendFinalReply (servId, activity);

      /* On bloque l'activite */
      <!comp.name()!>NbZombie++;
      ACTIVITY_STATUS(activity) = ZOMBIE;
      ACTIVITY_EVN(activity) = NO_EVENT;
    }

    /* Activite en attente */
    else if (ACTIVITY_EVN(activity) == SLEEP) {
      ACTIVITY_STATUS(activity) = SLEEP;
      ACTIVITY_EVN(activity) = NO_EVENT;
    }

    /* XXX Eveil systematique - le traitement des requetes est fait apres -> si l'une d'elle met le flag EXEC_TASK_WAKE_UP_FLAG on ne le voie pas ici */
/*    else if (ACTIVITY_STATUS(activity) == SLEEP) {*/
/*      && EXEC_TASK_WAKE_UP_FLAG(ACTIVITY_TASK_NUM(activity))) {*/
/*      ACTIVITY_EVN(activity) = EXEC;
      h2evnSignal(EXEC_TASK_ID(ACTIVITY_TASK_NUM(activity)));
    }*/

  } /* for */

  /* Reset demande d'eveil (pas utilise actuelement) */
/*  for (i=0; i<<!upper(comp.name())!>_NB_EXEC_TASK; i++)
    EXEC_TASK_WAKE_UP_FLAG(i) = FALSE;*/
}


/*****************************************************************************
 *
 *  <!comp.name()!>CntrlExtEvnExec  -  Traiter les evenements externes: arrivee
 *                              de requetes
 */

static void <!comp.name()!>CntrlExtEvnExec (SERV_ID <!comp.name()!>ServId)
     
{
  int mboxStatus;                    /* Etat de la boite aux lettres */
  int status;
  int i;
  BOOL wakeUp=FALSE;
  
  /* Obtenir l'etat de la boite aux lettres de reception de requetes */
  status = (mboxStatus = csMboxStatus (RCV_MBOX));
 
  /* Execution de la requete */
  if (mboxStatus & RCV_MBOX)  {
    status = csServRqstExec (<!comp.name()!>ServId);
    wakeUp = TRUE;
  }

  /* Suspendre la tache, en cas d'erreur */
  if (status == ERROR)
    <!comp.name()!>CntrlTaskSuspend (TRUE);

  /* On eveil toutes les taches d'exec (non periodique) un petit coup */
  if (wakeUp)
    for (i=0; i<<!upper(comp.name())!>_NB_EXEC_TASK; i++)
      if (!EXEC_TASK_PERIOD(i)) h2evnSignal(EXEC_TASK_ID(i));
}

/*****************************************************************************
 *
 *  <!comp.name()!>SpawnActivities  -  Lancer les activitees pretes
 *
 *  Description:  Pour chaque activite en attente, regarde si les activites
 *                incompatibles ont ete arretees. En ce cas lance l'activite.
 */

static void <!comp.name()!>SpawnActivities (SERV_ID servId)
     
{
  int activity;
  int i, j;
  int nbIncomp;

  /* Pour chaque activite */
  for (activity = 0; activity < MAX_ACTIVITIES; activity++) 

    /* Activite en attente */
    if (ACTIVITY_STATUS(activity) == INIT 
	&& ACTIVITY_EVN(activity) == NO_EVENT) {
      
      /* Activite incompatibles */
      nbIncomp = ACTIVITY_NB_INCOMP(activity);
      
      /* Liste des activites encore incompatibles */
      for (i=0, j=0; i<nbIncomp; i++)
	if (ACTIVITY_STATUS(ACTIVITY_TAB_INCOMP(activity)[i]) == ETHER
	    || ACTIVITY_STATUS(ACTIVITY_TAB_INCOMP(activity)[i]) == ZOMBIE)
	  ACTIVITY_NB_INCOMP(activity)--;
	else
	  ACTIVITY_TAB_INCOMP(activity)[j++]
	    = ACTIVITY_TAB_INCOMP(activity)[i]; 
      
      /* Il n'y a plus d'activite incompatible */
      if (ACTIVITY_NB_INCOMP(activity) == 0) {
    
	/* Declencher l'execution */
	ACTIVITY_EVN(activity) = START;
	h2evnSignal(EXEC_TASK_ID(ACTIVITY_TASK_NUM(activity)));
	
	/* Envoyer la replique intermediaire */
	if (csServReplySend (servId, ACTIVITY_RQST_ID(activity), INTERMED_REPLY, 
			     OK, (void *) &ACTIVITY_ID(activity), sizeof(int), 
			     (FUNCPTR) NULL) != OK)
	  <!comp.name()!>CntrlTaskSuspend (TRUE);
      }      /* Activite a lancer */
    }        /* Activite en attente */
}


/*****************************************************************************
 *
 *  <!comp.name()!>CntrlPosterWrite  - Met a jour le poster de controle
 *
 *  Description: Se suspend en cas d'erreur
 */

static void <!comp.name()!>CntrlPosterWrite ()
{
  if (posterWrite (<!comp.name()!>CntrlPosterId, 0, (void *) <!comp.name()!>CntrlStrId,
		   sizeof (<!upper(comp.name())!>_CNTRL_STR))
      != sizeof (<!upper(comp.name())!>_CNTRL_STR)) {
    h2perror("<!comp.name()!>CntrlPosterWrite");
    <!comp.name()!>CntrlTaskSuspend (TRUE);
  }
}

/*-------------------- FONCTIONS AUXILIAIRES -------------------------------*/

<?
# $tabRequestFuncDeclare$

# print functions declarations
for s in comp.servicesMap():
    print "static void " + comp.name() + "Cntrl" + s.data().name + "(SERV_ID servId, int rqstId);"

# print $$TabRequestFunc array
print "\nstatic void (*" + comp.name() + "TabRequestFunc[])() = {"
l = ""
for s in comp.servicesMap():
    l += comp.name() + "Cntrl" + s.data().name + ",\n"
print l[:-2] + "\n};"

# print $$TabRequestNum array (just ints from 0 to the number of services minus one)
print "static int " + comp.name() + "TabRequestNum[] = {"
i = 0
l = ""
for s in comp.servicesMap(): 
    l +=  str(i) + ", "
    i += 1
print l[:-2] + "};"
?>

/*****************************************************************************
 *
 *  <!comp.name()!>CntrlInitTask - Routine d'initialisation de la tache de controle
 */


static STATUS
<!comp.name()!>CntrlInitTask(SERV_ID *<!comp.name()!>ServId)
{
  int i;

  CNTRL_SDI_F = SDI_F;

  /* Creation de la boite aux lettres de reception des requetes */
  if (csMboxInit (<!upper(comp.name())!>_MBOX_NAME, 
		  <!upper(comp.name())!>_MBOX_RQST_SIZE, 0)
      != OK) {
    logMsg("<!comp.name()!>CntrlTask: csMboxInit: ");
    h2printErrno(errnoGet());
    return (ERROR);
  }
  LOGDBG(("<!comp.name()!>CntrlInitTask: created mailbox\n"));
  
  /* S'initialiser comme serveur */
  if (csServInitN (<!upper(comp.name())!>_MAX_RQST_SIZE, 
		  <!upper(comp.name())!>_MAX_REPLY_SIZE >= <!upper(comp.name())!>_MAX_INTERMED_REPLY_SIZE ?
		  <!upper(comp.name())!>_MAX_REPLY_SIZE : <!upper(comp.name())!>_MAX_INTERMED_REPLY_SIZE, 
		  NB_RQST_TYPE,
		  <!comp.name()!>ServId) 
      != OK) { 
    logMsg("<!comp.name()!>CntrlTask: csServInit: ");
    h2printErrno(errnoGet());
    return (ERROR);
  }
  LOGDBG(("<!comp.name()!>CntrlInitTask: initialized mailbox as a server\n"));
  
  /* Installer les routines de traitement des requetes */
  for (i=0; i<NB_RQST_TYPE-1; i++) {
    if (csServFuncInstall (*<!comp.name()!>ServId, <!comp.name()!>TabRequestNum[i], 
			   (FUNCPTR) <!comp.name()!>TabRequestFunc[i]) != OK) {
      logMsg("<!comp.name()!>CntrlTask: csServFuncInstall: ");
      h2printErrno(errnoGet());
      return (ERROR);
    }
  }
  LOGDBG(("<!comp.name()!>CntrlInitTask: installed requests\n"));

  /* Installer la requete abort
     (c'est le parser qui attribue le premier numero libre de requete ) */
  if (csServFuncInstall (*<!comp.name()!>ServId, <!str(abortRequestNum)!>, 
			 (FUNCPTR) <!comp.name()!>RqstAbortActivity) != OK) {
    logMsg("<!comp.name()!>CntrlTask: csServFuncInstall: ");
    h2printErrno(errnoGet());
    return (ERROR);
  }
  LOGDBG(("<!comp.name()!>CntrlInitTask: installed abort request\n"));
  
  /* Initialiser la structure de controle */
  STOP_MODULE_FLAG = FALSE;
  for (i=0; i<MAX_ACTIVITIES; i++) {
    ACTIVITY_STATUS(i) = ETHER;
    ACTIVITY_EVN(i) = NO_EVENT;
    ACTIVITY_TASK_NUM(i) = -1;
  }
  for (i=0; i<<!upper(comp.name())!>_NB_EXEC_TASK; i++)
    EXEC_TASK_NB_ACTI(i) = 0;

  CNTRL_NB_EXEC_TASKS = <!upper(comp.name())!>_NB_EXEC_TASK;

  /* La requete d'init */
  INIT_RQST = <!str(initRqstNb)!>;

  /* Creer le poster de controle */
  if (posterCreate (<!upper(comp.name())!>_CNTRL_POSTER_NAME, sizeof (<!upper(comp.name())!>_CNTRL_STR), 
                    &<!comp.name()!>CntrlPosterId) != OK) {
    logMsg("<!comp.name()!>CntrlTask: posterCreate: ");
    h2printErrno(errnoGet());
    return (ERROR);
  }
  LOGDBG(("<!comp.name()!>CntrlInitTask: created control poster\n"));
 
  /* Obtenir son propre identificateur de tache */
  CNTRL_TASK_ID = taskIdSelf ();
  NB_ACTIVITIES = 0;

  /* Record errors */
  <!comp.name()!>RecordH2errMsgs();

  /* C'est OK */
  return (OK);
}

/*****************************************************************************
 *
 *  <!comp.name()!>AbortActivity -  Arrete une activite
 *
 *  Description : Demande l'arret de l'activite si celle ci n'est pas
 *                deja arretee. Si elle est deja arretee, libere l'activite.
 *
 *  Retourne : TRUE si activite terminee, FALSE sinon
 */
 
static BOOL <!comp.name()!>AbortActivity (SERV_ID servId, int activity)
{
  
  switch (ACTIVITY_STATUS(activity)) {
    
    /* Activite inexistante */
  case ETHER:
    return(TRUE);

    /* Activite plantee */
  case ZOMBIE:
    freeActivity(activity);
    return(TRUE);
    
    /* Activite pas encore demarree */
  case INIT:
    <!comp.name()!>SendFinalReply(servId, activity);
    freeActivity (activity);
    return(TRUE);

    /* L'acitivite roupille */
  case SLEEP:
    ACTIVITY_EVN(activity) = INTER;
    h2evnSignal(EXEC_TASK_ID(ACTIVITY_TASK_NUM(activity)));
    return(FALSE);
    
    /* Activite en cours d'exec: START, EXEC, END, FAIL, INTER , SLEEP */
  default:
    switch (ACTIVITY_EVN(activity)) {
      
      /* Activite terminee */
    case ETHER:
      <!comp.name()!>SendFinalReply(servId, activity);
      freeActivity (activity);
      return(TRUE);
      
      /* Activite va se planter */
    case FAIL:
      return(FALSE);

      /* Interruption deja demandee */
    case INTER:
      return FALSE;

      /* Demande d'interruption END, START, EXEC, SLEEP? */
    default:
      ACTIVITY_EVN(activity) = INTER;
      h2evnSignal(EXEC_TASK_ID(ACTIVITY_TASK_NUM(activity)));
      return(FALSE);
    }
  }
}

/****************************************************************************
 *
 *  allocActivity  -  Allocation/Initialisation d'une activite
 *
 *  Retourne : Numero de l'activite ou -1
 */

static int allocActivity (int rqstId,     /* Id de la requete */
			  int requestNum, /* Type d'activite */
			  int execTaskNum,
			  int reentrantFlag,
			  SERV_ID servId)
{
  int activity, i;
  
  /* Il y a une activite ZOMBIE */
  if (<!comp.name()!>NbZombie != 0) {
    if (csServReplySend (servId, rqstId, FINAL_REPLY, 
			 S_<!comp.name()!>_stdGenoM_WAIT_ABORT_ZOMBIE_ACTIVITY, 
			 (void *) NULL, 0, (FUNCPTR) NULL) != OK)
      <!comp.name()!>CntrlTaskSuspend (TRUE);
    return -1;
  }

  /* Recherche d'une activite libre */
  /*  activity = 0;
  while(ACTIVITY_STATUS(activity) != ETHER && activity < MAX_ACTIVITIES) 
    activity++;*/

  
  for(i=0; i<MAX_ACTIVITIES; i++) {
    activity = (<!comp.name()!>LastActivityNum+1+i)%MAX_ACTIVITIES;
    if (ACTIVITY_STATUS(activity) == ETHER)
      break;
  }

  /* Pas d'activite libre */
  if (/*activity*/ i == MAX_ACTIVITIES) {
    if (csServReplySend (servId, rqstId, FINAL_REPLY, 
			 S_<!comp.name()!>_stdGenoM_TOO_MANY_ACTIVITIES, 
			 (void *) NULL, 0, (FUNCPTR) NULL) != OK)
      <!comp.name()!>CntrlTaskSuspend (TRUE);
    return -1;
  }

  <!comp.name()!>LastActivityNum = activity;
  <!comp.name()!>LastAbsolutActivityNum++;

  /* Cree l'activite */
  ACTIVITY_STATUS(activity) = INIT;
  ACTIVITY_EVN(activity) = NO_EVENT;
  ACTIVITY_RQST_TYPE(activity) = requestNum;
  ACTIVITY_RQST_ID(activity) = rqstId;
  ACTIVITY_ID(activity) = <!comp.name()!>LastAbsolutActivityNum;
  ACTIVITY_BILAN(activity) = OK;
  ACTIVITY_NB_INCOMP(activity) = 0;
  ACTIVITY_TASK_NUM(activity) = execTaskNum;
  ACTIVITY_REENTRANCE(activity) = reentrantFlag;

  /* Initialise les parametres */
  ACTIVITY_INPUT_ID(activity) = NULL;
  ACTIVITY_INPUT_SIZE(activity) = 0;
  ACTIVITY_OUTPUT_ID(activity) = NULL;
  ACTIVITY_OUTPUT_SIZE(activity) = 0;

  /* Une activite de plus */
  EXEC_TASK_NB_ACTI(execTaskNum)++;
  NB_ACTIVITIES++;

  return(activity);
}

/****************************************************************************
 *
 *  controlExecTaskStatus  -  Controle l'état des tâches d'exec
 *
 */
static BOOL controlExecTaskStatus(SERV_ID servId, int rqstId)
{
  int i;

  /* Exec tasks status */
  for (i=0; i<<!upper(comp.name())!>_NB_EXEC_TASK; i++)
    if (EXEC_TASK_STATUS(i) == ERROR) {
      /* XXXXX Il faudrait definir un bilan spécifique:
	 S_<!comp.name()!>_stdGenoM_EXEC_TASK_SUSPENDED */
      csServReplySend (servId, rqstId, FINAL_REPLY, 
		       S_<!comp.name()!>_stdGenoM_SYSTEM_ERROR,
		       (void *) NULL, 0, (FUNCPTR) NULL);
      return FALSE;
    }

  return TRUE;
}

/****************************************************************************
 *
 *  controlExecTaskStatusAndInitRqst  -  Controle l'état des tâches d'exec
 *					 et la requete d'init
 *
 */
static BOOL controlExecTaskStatusAndInitRqst(SERV_ID servId, int rqstId)
{
  int i;

  /* Exec tasks status */
  for (i=0; i<<!upper(comp.name())!>_NB_EXEC_TASK; i++)
    if (EXEC_TASK_STATUS(i) == ERROR) {
      /* XXXXX Il faudrait definir un bilan spécifique:
	 S_<!comp.name()!>_stdGenoM_EXEC_TASK_SUSPENDED */
      csServReplySend (servId, rqstId, FINAL_REPLY, 
		       S_<!comp.name()!>_stdGenoM_SYSTEM_ERROR,
		       (void *) NULL, 0, (FUNCPTR) NULL);
      return FALSE;
    }

  /* Init request */
  if (INIT_RQST != -1) {
    csServReplySend (servId, rqstId, FINAL_REPLY, 
		     S_<!comp.name()!>_stdGenoM_WAIT_INIT_RQST, 
		     (void *) NULL, 0, (FUNCPTR) NULL);
    return FALSE;
  }    

  return TRUE;
}

/****************************************************************************
 *
 *  freeActivity  -  Retire l'activite du tableau d'activite
 */

static void freeActivity (int activity)
{
  int execTaskNum;
  
  /* C'etait une activite ZOMBIE */
  if (ACTIVITY_STATUS(activity) == ZOMBIE) <!comp.name()!>NbZombie--;

  /* Petit netoyage */
  execTaskNum = ACTIVITY_TASK_NUM(activity);
  ACTIVITY_STATUS(activity) = ETHER;
  ACTIVITY_EVN(activity) = NO_EVENT;
  ACTIVITY_TASK_NUM(activity) = -1;
  EXEC_TASK_NB_ACTI(execTaskNum)--;
  NB_ACTIVITIES --;
}


/****************************************************************************
 *
 *  <!comp.name()!>SendFinalReply  -  Envoie la replique finale
 *
 *  Description : Envoie la replique finale avec le bilan d'execution si 
 *                tout c'est bien termine' (dernier etat END), ou avec un 
 *                message d'interruption (dernier etat INTER ou INIT)
 *                ou d'echec (dernier etat FAIL).
 */

static void <!comp.name()!>SendFinalReply (SERV_ID servId,
				    int activity)

{
  int bilan;

  /* 
   * Select bilan according to the last event 
   */
  switch(ACTIVITY_EVN(activity)) {

    /* Normal end or "clean" interruption */
  case ETHER:
    /* Clean interruption or not yet started  */
    if (ACTIVITY_STATUS(activity) == INIT ||
	ACTIVITY_STATUS(activity) == INTER) {
      bilan = S_<!comp.name()!>_stdGenoM_ACTIVITY_INTERRUPTED;
      ACTIVITY_BILAN(activity) = bilan;
    }
    
    /* Normal end */
    else {
      bilan = ACTIVITY_BILAN(activity);
      if (bilan != OK && !H2_MODULE_ERR_FLAG(bilan)) {
	CNTRL_TASK_BILAN = bilan;
	bilan = S_<!comp.name()!>_stdGenoM_SYSTEM_ERROR;
      }
    }
    break;
    
    /* Activity not yet started */
  case START:
    if (ACTIVITY_STATUS(activity) == INIT) {
      bilan = S_<!comp.name()!>_stdGenoM_ACTIVITY_INTERRUPTED;
      ACTIVITY_BILAN(activity) = bilan;      
    }
    break;

    /* Echec */
  case ZOMBIE:
    bilan = S_<!comp.name()!>_stdGenoM_ACTIVITY_FAILED;
    break;

    /* Interruption before starting (state == INIT) */
  case NO_EVENT:
    bilan = S_<!comp.name()!>_stdGenoM_ACTIVITY_INTERRUPTED;
    ACTIVITY_BILAN(activity) = bilan;
    break;

    /* Impossible termination event */
  default:
    logMsg("<!comp.name()!>CntrlTask: activity %d state %d event %d !?! \n", 
	   activity, ACTIVITY_STATUS(activity), ACTIVITY_EVN(activity));
    errnoSet(S_<!comp.name()!>_stdGenoM_FORBIDDEN_ACTIVITY_TRANSITION);
    bilan = S_<!comp.name()!>_stdGenoM_FORBIDDEN_ACTIVITY_TRANSITION;
    ACTIVITY_BILAN(activity) = bilan;
  }
  
  /* 
   * Send the final reply 
   */
  if (csServReplySend (servId, ACTIVITY_RQST_ID(activity), FINAL_REPLY, 
		       bilan, (void *) ACTIVITY_OUTPUT_ID(activity),
		       ACTIVITY_OUTPUT_SIZE(activity), (FUNCPTR) NULL) != OK)
    <!comp.name()!>CntrlTaskSuspend (TRUE);

  moduleEventCntrl.eventType = STATE_END_EVENT;
  moduleEventCntrl.activityNum = activity;
  moduleEventCntrl.activityState = ACTIVITY_EVN(activity);
  moduleEventCntrl.rqstType = ACTIVITY_RQST_TYPE(activity);
  moduleEventCntrl.taskNum = ACTIVITY_TASK_NUM(activity);
  sendModuleEvent(&moduleEventCntrl);
}


/****************************************************************************
 *
 *  <!comp.name()!>CntrlTaskSuspend  -  Suspension de la tache de controle
 *
 *  Description : recupere le bilan, puis suspend la tache de controle
 *
 */

static void <!comp.name()!>CntrlTaskSuspend (BOOL giveFlag)

{
  char strerr[64];

  /* Indiquer qu'une erreur a ete detectee */
  CNTRL_TASK_STATUS = ERROR;
  CNTRL_TASK_BILAN = errnoGet();

  logMsg("<!comp.name()!>CntrlTaskSuspend: %s\n", h2getErrMsg(CNTRL_TASK_BILAN, strerr, 64));

  /* Mettre a jour le poster de controle */
  if (posterWrite (<!comp.name()!>CntrlPosterId, 0, (void *) <!comp.name()!>CntrlStrId,
		   sizeof (<!upper(comp.name())!>_CNTRL_STR)) != sizeof (<!upper(comp.name())!>_CNTRL_STR)) {
    h2perror("<!comp.name()!>CntrlTaskSuspend: posterWrite");
  }

   /* Liberer les SDIs */
  if (giveFlag) {
    commonStructGive ((void *) <!comp.name()!>DataStrId);
    commonStructGive ((void *) <!comp.name()!>CntrlStrId);    
  }

  /* Suspendre la tache */
  taskSuspend (0);
}



/****************************************************************************
 *
 *  <!comp.name()!>ReplyAndSuspend  -  Repliquer et suspendre
 *
 *  Description : envoie la replique puis se suspend
 */

static void <!comp.name()!>ReplyAndSuspend (SERV_ID servId,  
				     int rqstId,
				     BOOL giveFlag)
{
  char strerr[64];

  /* Indiquer qu'une erreur a ete detectee */
  CNTRL_TASK_STATUS = ERROR;
  CNTRL_TASK_BILAN = errnoGet();

  logMsg("<!comp.name()!>CntrlTaskReplyAndSuspend: %s\n", 
	 h2getErrMsg(CNTRL_TASK_BILAN, strerr, 64));

  /* Mettre a jour le poster de controle */
  if (posterWrite (<!comp.name()!>CntrlPosterId, 0, (void *) <!comp.name()!>CntrlStrId,
		   sizeof (<!upper(comp.name())!>_CNTRL_STR)) != sizeof (<!upper(comp.name())!>_CNTRL_STR)) {
    h2perror ("<!comp.name()!>ReplyAndSuspend: posterWrite");
  }

/*XXX  <!comp.name()!>CntrlPosterShow();*/


  /* Liberer les SDIs */
  if (giveFlag) {
    commonStructGive ((void *) <!comp.name()!>DataStrId);
    commonStructGive ((void *) <!comp.name()!>CntrlStrId);    
  }

  /* Envoyer le code d'erreur vers le client */
  (void) csServReplySend (servId, rqstId, FINAL_REPLY, CNTRL_TASK_BILAN, 
			  (void *) NULL, 0, (FUNCPTR) NULL);

  /* Suspendre la tache */
  taskSuspend (0);

}

/*------------------- TRAITEMENT DES REQUETES  -----------------------------*/


/****************************************************************************
 *
 *  <!comp.name()!>RqstAbortActivity  -  Traitement de la requete Abort
 */

static void <!comp.name()!>RqstAbortActivity (SERV_ID servId, int rqstId)

{
  int activityId; /* Absolut activity id */
  int activity; /* Local activity Id */
  int bilan = OK;
  int i;

  /* Get activity to abort */
  if (csServRqstParamsGet (servId, rqstId, (void *) &activityId, 
			   sizeof(int), (FUNCPTR) NULL) != OK)
    <!comp.name()!>ReplyAndSuspend (servId, rqstId, TRUE);

  /* Activity identification */
  if (activityId < 0) {

    switch (activityId) 
      {
	/* Activity -88: restart suspended exec tasks */
      case GENOM_RESUME_EXEC_TASK:
	for (i=0; i<<!upper(comp.name())!>_NB_EXEC_TASK; i++) {
	  taskResume(EXEC_TASK_ID(i));
	  EXEC_TASK_STATUS(i) = OK;
	}
	break;
    
      case GENOM_PRINT_TIME_RQST:
	if (GENOM_PRINT_TIME_FLAG) {
	  GENOM_PRINT_TIME_FLAG = 0;
	  /* reset time max for next display */
	  for (i=0; i<<!upper(comp.name())!>_NB_EXEC_TASK; i++) {
	    EXEC_TASK_MAX_PERIOD(i) = 0;
	  }
	}
	else {
	  GENOM_PRINT_TIME_FLAG = 1;
	}
	break;

      case GENOM_VERBOSE_RQST:
	if (GENOM_VERBOSE_LEVEL)
	  GENOM_VERBOSE_LEVEL = 0;
	else
	  GENOM_VERBOSE_LEVEL = 1;
	break;

	/* Activity -99: stop the module */
      case GENOM_END_MODULE:

	/* Test if there is on going activities */
	if (NB_ACTIVITIES != 0) {
	  bilan = S_<!comp.name()!>_stdGenoM_WAIT_ABORT_ZOMBIE_ACTIVITY;
	  /* bilan = S_stdGenoM_ACTIVITIES_stdGenoM_REMAINED; */
	  break;
	}

	/* End the module */
	else {

	  STOP_MODULE_FLAG = TRUE;
	  for (i=<!upper(comp.name())!>_NB_EXEC_TASK-1; i > -1; i--) 
	    EXEC_TASK_WAKE_UP_FLAG(i) = TRUE;

	  /* give back Internal Data Structures */
	  commonStructGive ((void *) <!comp.name()!>DataStrId);
	  commonStructGive ((void *) <!comp.name()!>CntrlStrId);

	  /* Interrupt exec task */
	  for (i=<!upper(comp.name())!>_NB_EXEC_TASK-1; i > -1; i--) {
	    logMsg("Killing task %s ... \n", <!comp.name()!>ExecTaskNameTab[i]);
	    taskResume(EXEC_TASK_ID(i));
	    h2evnSignal(EXEC_TASK_ID(i));
	  }

	  /* Wait end */
	  for (i=<!upper(comp.name())!>_NB_EXEC_TASK-1; i > -1; i--) {
	    while (EXEC_TASK_WAKE_UP_FLAG(i))
	      h2evnSusp(0);
	    logMsg("    ... task %s killed\n", <!comp.name()!>ExecTaskNameTab[i]);
	  }

	  /* send final reply */
	  csServReplySend (servId, rqstId, FINAL_REPLY, bilan, 
			   (void *) NULL, 0, (FUNCPTR) NULL);

	  
	  /* Destruction of mboxes, SDI and posters and BYE ! */
	  csMboxEnd();
	  commonStructDelete ((void *) <!comp.name()!>DataStrId);
	  commonStructDelete ((void *) <!comp.name()!>CntrlStrId);
	  posterDelete(<!comp.name()!>CntrlPosterId);
	  logMsg("<!comp.name()!>CntrlTask ended\n");
	  /* Remove PID file */
	  unlink(pidFilePath);
	  exit(0);
	}
	break;

      default:
	bilan = S_<!comp.name()!>_stdGenoM_UNKNOWN_ACTIVITY;
	break;
      } /* switch */
  }
  
  /* Numéro d'activité positive inconnu */
  else if(activityId > <!comp.name()!>LastAbsolutActivityNum) {
    bilan = S_<!comp.name()!>_stdGenoM_UNKNOWN_ACTIVITY;
  }
    
  /* Numéro valide */
  else {

    /* Recherche de l'activité */
    for (activity = 0; activity < MAX_ACTIVITIES; activity++)
      if (ACTIVITY_ID(activity) == activityId)
	break;

    /* Activité déjà terminée */
    if (activity == MAX_ACTIVITIES || ACTIVITY_STATUS(activity) == ETHER)
      bilan = S_<!comp.name()!>_stdGenoM_ACTIVITY_ALREADY_ENDED;

    /* Activité trouvée: interruption */
    else 
      <!comp.name()!>AbortActivity(servId, activity);
  }

  /* Send final reply */
  if (csServReplySend (servId, rqstId, FINAL_REPLY, bilan, 
		       (void *) NULL, 0, (FUNCPTR) NULL) != OK)
    <!comp.name()!>CntrlTaskSuspend (TRUE); 
}

/****************************************************************************
 * <!comp.name()!>SignalHandler - Handle signals in a module
 */
void
<!comp.name()!>SignalHandler(int sig)
{
	printf("Signal received %d\n", sig);
	<!comp.name()!>SignalAbort = TRUE;	/* indicate that we wanna end */
	h2evnSignal(CNTRL_TASK_ID);	/* wake up control task */
}

/****************************************************************************
 *
 * <!comp.name()!>End - terminate a module upon signal reception
 */
static void
<!comp.name()!>SignalEnd(void)
{
	int i;

	STOP_MODULE_FLAG = TRUE;
	for (i=<!upper(comp.name())!>_NB_EXEC_TASK-1; i > -1; i--) 
		EXEC_TASK_WAKE_UP_FLAG(i) = TRUE;
	
	/* give back Internal Data Structures */
	commonStructGive ((void *) <!comp.name()!>DataStrId);
	commonStructGive ((void *) <!comp.name()!>CntrlStrId);
	
	/* Interrupt exec task */
	for (i=<!upper(comp.name())!>_NB_EXEC_TASK-1; i > -1; i--) {
		logMsg("Killing task %s ... \n", <!comp.name()!>ExecTaskNameTab[i]);
		taskResume(EXEC_TASK_ID(i));
		h2evnSignal(EXEC_TASK_ID(i));
	}
	
	/* Wait end */
	for (i=<!upper(comp.name())!>_NB_EXEC_TASK-1; i > -1; i--) {
		while (EXEC_TASK_WAKE_UP_FLAG(i))
			h2evnSusp(0);
		logMsg("    ... task %s killed\n", <!comp.name()!>ExecTaskNameTab[i]);
	}
	
	
	/* Destruction of mboxes, SDI and posters and BYE ! */
	csMboxEnd();
	commonStructDelete ((void *) <!comp.name()!>DataStrId);
	commonStructDelete ((void *) <!comp.name()!>CntrlStrId);
	posterDelete(<!comp.name()!>CntrlPosterId);
	logMsg("<!comp.name()!>CntrlTask ended\n");
	  /* Remove PID file */
	  unlink(pidFilePath);
	  exit(0);
}

/* Requetes de type controle */

<?
serviceNum = 0
for s in comp.servicesMap():
    service = s.data()
    controlFuncFlag = (service.codel("control") == 0)
    if len(service.inputs()) == 0:
	inputFlag = False
	inputSize = "0"
	inputNamePtr = "NULL"
	inputRefPtr = "NULL"
    else:
	inputFlag = True
	inputName = service.inputs()[0]
	inputSize = "sizeof((*" + comp.name() + "DataStrId)." + inputName + ")"
	inputSize = inputSize[:-1] # remove the last '+'

	t = comp.typeFromIdsName(inputName)
	if(t.kind == IdlKind.String):
	    inputNamePtr = inputName
	else:
	    inputNamePtr = "&" + inputName
	inputRefPtr = "&((*" + comp.name() + "DataStrId)." + inputName + ")" 

    # same for output
    if service.output:
	outputFlag = False
	outputSize = "0"
	outputNamePtr = "NULL"
	outputRefPtr = "NULL"
    else:
	outputFlag = True
	outputName = service.output
	outputSize = "sizeof((*" + comp.name() + "DataStrId)." + outputName + ")"
	outputSize = outputSize[:-1] # remove the last '+'

	t = comp.typeFromIdsName(inputName)
	if(t.kind == IdlKind.String):
	    outputNamePtr = outputName
	else:
	    outputNamePtr = "&" + outputName
	outputRefPtr = "&((*" + comp.name() + "DataStrId)." + outputName + ")" 

    if service.type == ServiceType.Control: ?>
/*****************************************************************************
 *
 *  <!comp.name()!>Cntrl<!service.name!> - Control request
 *
 */

<?
    controlFunc = ""
    if controlFuncFlag:
	controlFunc = "extern int " + service.codel("control").name + "();"
    ?>

static void <!comp.name()!>Cntrl<!service.name!> (SERV_ID servId, int rqstId)

{
  int i;
  $tabCompatibilityDeclare$
  int *compatibilityTab = <!comp.name()!><!service.name!>Compatibility;
  int bilan=OK;
<? 
    if inputFlag and controlFuncFlag: 
	for s in service.inputs():
	    print comp.typeFromIdsName(s).toCType(True) + " " + s + ";"
    ?>

  /*--------------------------------------------------------------
   * Control init done and exec tasks status 
   */
  if (!controlExecTaskStatus(servId, rqstId))
    return;

  /*--------------------------------------------------------------
   * Interruption of the incompatible activities
   */
  for (i = 0; i < MAX_ACTIVITIES; i++) 
    if (ACTIVITY_STATUS(i) != ETHER && ! compatibilityTab[ACTIVITY_RQST_TYPE(i)]) 
      <!comp.name()!>AbortActivity (servId, i);

  /* 
   * Send the associated event
   */
  moduleEventCntrl.eventType = STATE_START_EVENT;
  moduleEventCntrl.activityState = EXEC;
  moduleEventCntrl.rqstType = <!str(serviceNum)!>;
  moduleEventCntrl.taskNum = -1;
  sendModuleEvent(&moduleEventCntrl);

  /*--------------------------------------------------------------
   * Call control func
   */
<?
    if controlFuncFlag:
	?>
  {
    BOOL status;

<?
	if inputFlag: 
	    ?> 
    /* Get inputs */
    if (csServRqstParamsGet (servId, rqstId, (void *) <!inputNamePtr!>, 
			     <!inputSize!>, (FUNCPTR) NULL) != OK)
      <!comp.name()!>ReplyAndSuspend (servId, rqstId, TRUE);
    
    /* Call control func */
    status = <!controlFunc!> (<!inputNamePtr!>, &bilan);
<?
	else:
	    ?>
    status = <!controlFunc!> (&bilan);
<?

	?>
    if (status != OK) {
      if (bilan == OK)
	bilan =  S_<!comp.name()!>_stdGenoM_CONTROL_CODEL_ERROR;
      if (csServReplySend (servId, rqstId, FINAL_REPLY, bilan, 
			   (void *) NULL, 0, (FUNCPTR) NULL) != OK)
	<!comp.name()!>CntrlTaskSuspend (TRUE);
      return;
    }
  }
<?

    ?>

 /*-------------------------------------------------------------
  * Record inputs
  */
<? 
    if inputFlag:
	if controlFuncFlag:
	    print "memcpy ((void *) " + inputRefPtr + ", (void *) " + inputNamePtr + ", " + inputSize + ");"
	else:?>
  if (csServRqstParamsGet (servId, rqstId, (void *) <!inputRefPtr!>,
			   <!inputSize!>, (FUNCPTR) NULL) != OK)
    <!comp.name()!>ReplyAndSuspend (servId, rqstId, TRUE);
<?

    ?>

 /*-------------------------------------------------------------
  * Send final reply
  */
  if (csServReplySend (servId, rqstId, FINAL_REPLY, bilan, 
		       (void *) <!outputRefPtr!>, <!outputSize!>,
		       (FUNCPTR) NULL) != OK)
    <!comp.name()!>CntrlTaskSuspend (TRUE);

  moduleEventCntrl.eventType = STATE_END_EVENT;
  moduleEventCntrl.activityState = ETHER;
  sendModuleEvent(&moduleEventCntrl);
}

<?
    serviceNum += 1

?>


/* Requetes de type execution */

