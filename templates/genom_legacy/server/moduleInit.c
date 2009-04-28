
/* --- FILE GENERATED WITH GENOM, DO NOT EDIT BY HAND ------------------ */

/* 
 * Copyright (c) 2004-2005
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

/* module <!comp.name()!> initialization routines */

#if defined(__RTAI__) && defined(__KERNEL__)
# include <linux/init.h>
# include <linux/module.h>
# include <linux/kernel.h>
# include <linux/sched.h>

#else
# include <stdio.h>
#  include <sys/param.h>
#  include <stdlib.h>
#  include <fcntl.h>
#  include <string.h>
#  include <errno.h>
#  include <unistd.h>
#  include <sys/utsname.h>

#  define PID_FILE	".<!comp.name()!>.pid"
#endif /* RTAI && KERNEL */

#include <portLib.h>
#include <shellLib.h>
#include <h2initGlob.h>

#include <taskLib.h>
#include <errnoLib.h>
#include <h2evnLib.h>

#include "<!comp.name()!>Header.h"
#include "commonStructLib.h"
#include "<!comp.name()!>MsgLib.h"

#define CNTRL_TASK_MIN_STACK_SIZE 4096
#define EXEC_TASK_MIN_STACK_SIZE  4096


/* --- Global variables ---------------------------------------------- */

/* Initialization semaphores */
SEM_ID sem<!comp.name()!>CntrlTaskInit;
SEM_ID sem<!comp.name()!>InitExecTab[<!upper(comp.name())!>_NB_EXEC_TASK];

/* Les taches */
void <!comp.name()!>CntrlTask ();
<? # $execTaskTabDescribe$
for t in comp.tasksMap():
    print "extern void %s%s(%s_CNTRL_STR *, %s *);" % (comp.name(), t.data().name, upper(comp.name()), comp.IDSType.toCType(True))
?>
typedef struct {
	char *name;
	int priority;
	int size;
	void (*func)();
} <!upper(comp.name())!>_EXEC_TASK_STR;

<!upper(comp.name())!>_EXEC_TASK_STR <!comp.name()!>ExecTaskTab[] = {
<?
out = ""
for t in comp.tasksMap():
    task = t.data()
    out += "{\"" + comp.name() + task.name + "\", "
    out += "%d, " % task.priority
    if task.stackSize > 0:
	out += str(task.stackSize)
    else:
	out += "5000"
	sys.stderr.write("No stack size specified for task '%s'" % task.name)
    out += ", " + comp.name() + task.name + " },\n"
print out[:-2] + "\n};"
?>

/* Internal data structures */
<!internalDataType!> *<!comp.name()!>DataStrId = NULL;
<!upper(comp.name())!>_CNTRL_STR *<!comp.name()!>CntrlStrId = NULL;

#ifdef PID_FILE
/* name of the file containing module's PID */
char pidFilePath[MAXPATHLEN];
#endif

/****************************************************************************
*
*  <!comp.name()!>TaskInit  -  Initialisation de l'module de <!comp.name()!>
*
*  Description:
*  Routine d'initialisation du module <!comp.name()!>, a etre appelee au moment
*  du boot.
*
*  Retourne: OK ou ERROR
*/

STATUS
<!comp.name()!>TaskInit()
{
  int i;
#ifdef PID_FILE
  FILE* pidFile;
  int pidFileFd;
  char *pidDir;
  struct utsname uts;
#endif /* PID_FILE */
  char strerr[64];

  /*
   * Create internal data structures
   */

  /* SDI_f */
  if (commonStructCreate (sizeof (<!internalDataType!>), 
			  (void *) &<!comp.name()!>DataStrId) 
      != OK)  {
    h2perror ("<!comp.name()!>TaskInit: cannot create the Functional Data Base");
    goto error;
  }
  
  /* SDI_c */
  if (commonStructCreate (sizeof (<!upper(comp.name())!>_CNTRL_STR), 
			  (void *) &<!comp.name()!>CntrlStrId) 
      != OK)  {
    h2perror ("<!comp.name()!>TaskInit: cannot create the Control Data Base");
    goto error;
  }

#ifdef PID_FILE
  /*
   * Create pid file
   */
  pidDir = getenv("H2DEV_DIR");
  if (pidDir == NULL) {
      pidDir = getenv("HOME");
  }
  if (pidDir == NULL) {
      pidDir = "/tmp";
  }
  if (uname(&uts) == -1) {
      errnoSet(errno);
      goto error;
  }
  snprintf(pidFilePath, MAXPATHLEN, "%s/%s-%s", pidDir, PID_FILE, uts.nodename);
  pidFileFd = open(pidFilePath, O_CREAT|O_EXCL|O_WRONLY, 0644);
  if (pidFileFd < 0) {
      fprintf(stderr, "<!comp.name()!>: error creating %s: %s\n",
	      pidFilePath, strerror(errno));
      goto error;
  }
  pidFile = fdopen(pidFileFd, "w");
  if (pidFile == NULL) {
      perror("<!comp.name()!>: error opening pid file");
      goto error;
  }
  fprintf(pidFile, "%d ", getpid());
  fputc('\n', pidFile);
  fclose(pidFile);
#endif /* PID_FILE */

  /* 
   * Spawn control task
   */
  logMsg("<!comp.name()!>: spawning control task.\n");

  sem<!comp.name()!>CntrlTaskInit = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
  if (sem<!comp.name()!>CntrlTaskInit == NULL) {
     h2perror("<!comp.name()!>TaskInit: cannot create control semaphore");
     goto error; 
  }
  
  if (taskSpawn("<!comp.name()!>CntrlTask", 10 /* priorite */, VX_FP_TASK,
		<!upper(comp.name())!>_MAX_RQST_SIZE + CNTRL_TASK_MIN_STACK_SIZE /*size*/, 
		(FUNCPTR)<!comp.name()!>CntrlTask) == ERROR) {
     h2perror("<!comp.name()!>TaskInit: cannot spawn <!comp.name()!>CntrlTask");
     goto error;
  }
  /* wait for initialization */
  if (semTake(sem<!comp.name()!>CntrlTaskInit, WAIT_FOREVER) != OK)
     goto error;
  
  semDelete(sem<!comp.name()!>CntrlTaskInit);
  sem<!comp.name()!>CntrlTaskInit = NULL;

  /* check task status */
  if (CNTRL_TASK_STATUS == ERROR) {
     errnoSet(CNTRL_TASK_BILAN);
     h2perror("<!comp.name()!>: Control task failed");
     goto error;
  }

  /* 
   * Spawn execution tasks
   */
  for (i=0; i<<!upper(comp.name())!>_NB_EXEC_TASK; i++) {
    logMsg("<!comp.name()!>: spawning task %s.\n", <!comp.name()!>ExecTaskTab[i].name);

    sem<!comp.name()!>InitExecTab[i] = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
    if (sem<!comp.name()!>InitExecTab[i] == NULL) {
       h2perror("<!comp.name()!>TaskInit: cannot create init semaphore");
       goto error; 
    }
    
    if (taskSpawn(<!comp.name()!>ExecTaskTab[i].name, 
		  <!comp.name()!>ExecTaskTab[i].priority, VX_FP_TASK, 
		  <!comp.name()!>ExecTaskTab[i].size + EXEC_TASK_MIN_STACK_SIZE, 
		  (FUNCPTR)<!comp.name()!>ExecTaskTab[i].func) == ERROR) {
       h2perror("<!comp.name()!>TaskInit: cannot spawn exec task");
       goto error;
    }
    /* wait for initialization */
    if (semTake(sem<!comp.name()!>InitExecTab[i], WAIT_FOREVER) != OK) {
       h2perror("<!comp.name()!>TaskInit: semTake init sem");
       goto error;
    }
    
    semDelete(sem<!comp.name()!>InitExecTab[i]);
    sem<!comp.name()!>InitExecTab[i] = NULL;

    /* check task status */
    if (EXEC_TASK_STATUS(i) == ERROR) {
       logMsg("<!comp.name()!>TaskInit: Exec task %s failed : %s\n", 
	      <!comp.name()!>ExecTaskTab[i].name, h2getErrMsg(EXEC_TASK_BILAN(i), strerr, 64));
       errnoSet(EXEC_TASK_BILAN(i));
       goto error;
    }
  }

  /* wake up control task once for poster initialization */
  h2evnSignal(CNTRL_TASK_ID);

  /*
   * Module is ready
   */
  logMsg("<!comp.name()!>: all tasks are spawned\n");

  return OK;

  error:
  /* Destroy initialization semaphores */
  for (i=0; i<<!upper(comp.name())!>_NB_EXEC_TASK; i++) if (sem<!comp.name()!>InitExecTab[i]) {
     semDelete(sem<!comp.name()!>InitExecTab[i]);
     sem<!comp.name()!>InitExecTab[i] = NULL;
  }

  if (sem<!comp.name()!>CntrlTaskInit) {
     semDelete(sem<!comp.name()!>CntrlTaskInit);
     sem<!comp.name()!>CntrlTaskInit = NULL;
  }

  /* Destroy internal data structures */
  if (<!comp.name()!>CntrlStrId) {
     commonStructDelete(<!comp.name()!>CntrlStrId);
     <!comp.name()!>CntrlStrId = NULL;
  }

  if (<!comp.name()!>DataStrId) {
     commonStructDelete(<!comp.name()!>DataStrId);
     <!comp.name()!>DataStrId = NULL;
  }
  return ERROR;
}


#if defined(__RTAI__) && defined(__KERNEL__)

/*
 * --- Module initialization for RTAI -----------------------------------
 *
 */

MODULE_DESCRIPTION("<!comp.name()!> module");

int
<!comp.name()!>_module_init(void)
{
   if (<!comp.name()!>TaskInit() == ERROR) {
      /* we don't report an error to insmod: there might still be
       * suspended tasks lying around and the module's code must remain
       * in memory. */
      h2perror("Error moduleInit");
   }

   return 0;
}

void
<!comp.name()!>_module_exit(void)
{
   ;
}

module_init(<!comp.name()!>_module_init);
module_exit(<!comp.name()!>_module_exit);

#endif /* RTAI && KERNEL */


#if !defined(__KERNEL__)
/*
 * --- main for unix ----------------------------------------------------
 */
static char *prompt = "<!comp.name()!>> ";
extern char *optarg;
extern int optind;

static void 
usage(void)
{
    fprintf(stderr, "usage: <!comp.name()!> [-b|-e|-s] [-f freq]\n");
    exit(3);
}

int
main(int argc, char *argv[])
{
    int c;
    /* Options par defaut */
    int backgroundOpt = 0, essaiOpt = 0, shellOpt = 0;
    int frequence = 100;		/* frequence par defaut */
    int errFlag = 0;
    int waitPipe[2];
    char buf[80];

    /* Parse les options */
    while ((c = getopt(argc, argv, "besf:")) != -1) {
	switch (c) {
	  case 'b':
	    backgroundOpt++;
	    break;
	  case 'e':
	    if (shellOpt) {
		errFlag++;
	    } else {
		essaiOpt++;
	    }
	  case 's':
	    if (essaiOpt) {
		errFlag++;
	    } else {
		shellOpt++;
	    }
	    break;
	  case 'f':
	    frequence = atoi(optarg);
	    break;
	  case '?':
	    errFlag++;
	    break;
	} /* switch */
    } /* while */
    if (errFlag) {
	/* Erreur */
	usage();
    }

    if (backgroundOpt) {
	if (shellOpt || essaiOpt) {
	    /* Erreur */
	    usage();
	}
	/* Creation d'un pipe pour envoyer un message a la fin de l'init */
	pipe(waitPipe);
	
	/* Lancement en background */
	if (fork() != 0) {
	    /* close(waitPipe[1]); */
	    /* Attend le lancement du module par une lecture sur le pipe */
	    if (read(waitPipe[0], buf, sizeof(buf)) <= 0) {
		fprintf(stderr, 
			"<!comp.name()!> moduleInit: error waiting for module start\n");
	    } 
	    exit(0);
	}
	/* close(waitPipe[0]); */
    }

    /* Initialise environnement comLib */
<?
if periodicFlag:
    ?>
    errFlag = h2initGlob(100);
<?
else:
    ?>
    errFlag = h2initGlob(0);
<?
?>
    if (errFlag == ERROR) {
	/* informe la tache en avant plan */
	write(waitPipe[1], "<!comp.name()!>: h2initGlob error\n", 17); 
	exit(2);
    }
    /* Demarrage des taches du module */
    if (<!comp.name()!>TaskInit() == ERROR) {
	printf("<!comp.name()!>: Error moduleInit: ");
	h2printErrno(errnoGet());
	/* informe la tache en avant plan */
	write(waitPipe[1], "<!comp.name()!>: moduleTaskInit error\n", 21); 
	exit(1);
    }
    if (shellOpt) {
	/* Lance un shell */
	shellMainLoop(stdin, stdout, stderr, prompt);
    } else if (essaiOpt) {
	fprintf(stderr, "<!comp.name()!>: not implemented\n");
    } 
    /* informe la tache en avant plan */
    if (backgroundOpt) {
	write(waitPipe[1], "<!comp.name()!>: OK\n", 3);
	/* close(waitPipe[1]); */
    }
    /* Wait forever */
    while (1) {
	select(64, NULL, NULL, NULL, NULL);
    }
    unlink(pidFilePath);
    exit(0);
}
#endif /* !__KERNEL__ */
