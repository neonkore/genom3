/* 
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

/* --- Message-based communication for module <!comp.name()!> --------------- */

#if defined(__RTAI__) && defined(__KERNEL__)
# include <linux/kernel.h>
#else
# include <stdio.h>
# include <stddef.h>
#endif

#include <portLib.h>
#include <errnoLib.h>

#include "<!comp.name()!>MsgLib.h"
#include "<!comp.name()!>Error.h"
 
/*========================================================================
 *
 *  <!comp.name()!>ClientInit  -  Routine d'initialisation d'un client de <!comp.name()!>
 *
 *  Description:
 *  Cette routine  doit etre appelee par un client du module, pour pouvoir 
 *  l'utiliser. Attention: la fonction csMboxInit doit etre appelee avant.
 *
 *  Retourne: OK ou ERROR
 */
 
STATUS <!comp.name()!>ClientInit (CLIENT_ID *pClientId)

{
  STATUS status;

  /* Appeler la routine d'initialisation d'un client CS */
  status = csClientInit (<!upper(comp.name())!>_MBOX_NAME, <!upper(comp.name())!>_MAX_RQST_SIZE,
			 <!upper(comp.name())!>_MAX_INTERMED_REPLY_SIZE, 
			 <!upper(comp.name())!>_MAX_REPLY_SIZE, 
			pClientId);
  if (status != OK) 
    h2perror("<!comp.name()!>ClientInit");

  <!comp.name()!>RecordH2errMsgs();

  return status;
}
 
/*-------------------------------------------------------------------------
 *
 *  <!comp.name()!>ClientEnd - Routine de fin d'un client de <!comp.name()!>
 *
 *  Description:
 *  Cette routine libere les objets alloues par le client.
 *
 *  Retourne : OK ou ERROR
 */

STATUS <!comp.name()!>ClientEnd (CLIENT_ID clientId)         /* Id. du client */
     
{
  STATUS status;
  /* Appeler la routine de liberation du client */
  status = csClientEnd (clientId);
  if (status != OK) 
    h2perror("<!comp.name()!>ClientEnd");
  return status;
}


/*========================================================================
 *
 *  <!comp.name()!>AbortRqstSend  -  Emission d'une requete de controle
 *
 *  Description: Arret de l'activite specifie'e en parametre
 *
 *  Retourne : OK ou ERROR
 */
 
int <!comp.name()!>AbortRqstSend (CLIENT_ID clientId, 
			   int *pRqstId,
			   int *activity,
			   int replyTimeOut)
{
  errnoSet(0);

  /* Emettre la requete */
  if (csClientRqstSend (clientId, <!upper(comp.name())!>_ABORT_RQST, (void *) activity,
			sizeof(int), (FUNCPTR) NULL, FALSE, 0, replyTimeOut, 
			pRqstId) == ERROR)
    return(ERROR);

  return(OK);
}

/*-------------------------------------------------------------------------
 *
 *  <!comp.name()!>AbortReplyRcv  -  Reception de la replique finale
 *
 *  Description:
 *
 *  Retourne : ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK ou
 *             WAITING_FINAL_REPLY
 */
 
int <!comp.name()!>AbortReplyRcv (CLIENT_ID clientId, 
			   int rqstId, 
			   int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY */
			   int *bilan)
{
  int status;    /* ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK */

  /* Verifier le flag de type de blocage */
  if (block != NO_BLOCK && block != BLOCK_ON_FINAL_REPLY) {
    errnoSet (S_<!comp.name()!>_stdGenoM_BAD_BLOCK_TYPE);
    return (ERROR);
  }
  
  status = csClientReplyRcv (clientId, rqstId, block, (void *) NULL, 0,  
			     (FUNCPTR) NULL, (void *) NULL,
			     0, (FUNCPTR) NULL);
  if (status == ERROR) {
    *bilan = errnoGet();

    /* Filtrage type d'erreur */
    if (H2_MODULE_ERR_FLAG(*bilan))
      return(FINAL_REPLY_OK);
  }
  
  return(status);
}
 
/*-------------------------------------------------------------------------
 *
 *  <!comp.name()!>AbortRqstAndRcv  -  Requete de controle
 *
 *  Description: Arret de l'activite specifie'e en parametre
 *  Cette fonction emet une requete et attend la replique finale.
 *
 *  Retourne : ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK
 */
 
int <!comp.name()!>AbortRqstAndRcv (CLIENT_ID clientId, 
			     int *activity,
			     int *bilan)

{
  int rqstId;                  /* Ou` mettre l'id de la requete */
  int status;
  
  errnoSet(0);
  *bilan = OK;
  
  /* Emettre la requete */
  if (csClientRqstSend (clientId, <!upper(comp.name())!>_ABORT_RQST, (void *) activity,
			sizeof(int), (FUNCPTR) NULL, FALSE, 0, 
			TIME_WAIT_REPLY, 
			&rqstId) == ERROR) {
    *bilan = errnoGet();
    return(ERROR);
  }
  
  /* Reception de la replique */
  status = csClientReplyRcv (clientId, rqstId, BLOCK_ON_FINAL_REPLY, 
			     (void *) NULL, 0, (FUNCPTR) NULL, 
			     (void *) NULL, 0, 
			     (FUNCPTR) NULL);
  if (status == ERROR) {
    *bilan = errnoGet();
    
    /* Filtrage type d'erreur */
    if (H2_MODULE_ERR_FLAG(*bilan))
      return(FINAL_REPLY_OK);
  }
  
  return(status);
}


/* liste des requetes/repliques de controle */

/* liste des requetes/repliques d'execution */

<?
for s in comp.servicesMap():
    service = s.data()
    serviceNum = "%s_%s_RQST" % (upper(comp.name()), upper(service.name))

    if len(service.inputs()) == 0:
	inputSize = "0"
	inputName = "NULL"
	input = ""
    else:
	inputShortName = service.inputs()[0]
	inputName = "in_" + inputShortName

	t = comp.typeFromIdsName(inputShortName)
	input = t.toCType(True)
	inputSize = "sizeof(" + input + ")"
	if(t.kind != IdlKind.String):
	    input += " *"
	else:
	    output += " "
	input += inputName + ","

    if len(service.output) == 0:
	outputSize = "0"
	outputName = "NULL"
	output = ""
    else:
	outputName = "out_" + service.output

	t = comp.typeFromIdsName(service.output)
	output = t.toCType(True)
	outputSize = "sizeof(" + output + ")"
	if(t.kind != IdlKind.String):
	    output += " *"
	else:
	    output += " "
	output += outputName + ","

    if service.type == ServiceType.Control:
	?>
/*=========================================================================
 *
 *  <!comp.name()!><!service.name!>RqstSend  -  Emission d'une requete de controle
 *
 *  Retourne : OK ou ERROR
 */
 
STATUS <!comp.name()!><!service.name!>RqstSend (CLIENT_ID clientId, 
				  int *pRqstId,
				  <!input!>
				  int replyTimeOut)
{
  errnoSet(0);

  /* Emettre la requete */
  if (csClientRqstSend (clientId, <!serviceNum!>, (void *) <!inputName!>,
			<!inputSize!>, (FUNCPTR) NULL, FALSE, 0, replyTimeOut, 
			pRqstId) == ERROR)
    return ERROR;
  return OK;
}

/*-------------------------------------------------------------------------
 *  <!comp.name()!><!service.name!>ReplyRcv  -  Reception de la replique finale
 *
 *  Retourne : ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK ou
 *             WAITING_FINAL_REPLY
 */
 
int <!comp.name()!><!service.name!>ReplyRcv (CLIENT_ID clientId, 
			       int rqstId, 
			       int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY */
			       <!output!>
			       int *bilan)
{
  int status;    /* ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK */

  /* Verifier le flag de type de blocage */
  if (block != NO_BLOCK && block != BLOCK_ON_FINAL_REPLY) {
    errnoSet (S_<!comp.name()!>_stdGenoM_BAD_BLOCK_TYPE);
    return (ERROR);
  }
  
  if ((status = csClientReplyRcv (clientId, rqstId, block, (void *) NULL, 0,  
				  (FUNCPTR) NULL, (void *) <!outputName!>,
				  <!outputSize!>, (FUNCPTR) NULL)) == ERROR) {
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
  }
  else *bilan = OK;
  
  return(status);
}
 
/*-------------------------------------------------------------------------
 *  <!comp.name()!><!service.name!>RqstAndRcv  -  Emission/reception requete de controle
 *
 *  Retourne : ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK
 */
 
int <!comp.name()!><!service.name!>RqstAndRcv (CLIENT_ID clientId, 
				 <!input!> 
				 <!output!>
				 int *bilan)
{
  int rqstId;                  /* Ou` mettre l'id de la requete */
  int status;
  
  errnoSet(0);
  
  /* Emettre la requete */
  if (csClientRqstSend (clientId, <!serviceNum!>, (void *) <!inputName!>,
			<!inputSize!>, (FUNCPTR) NULL, FALSE, 0, 
			TIME_WAIT_REPLY, &rqstId) == ERROR) {
    *bilan = errnoGet();
    return(ERROR);
  }
  
  /* Reception de la replique */
  if ((status = csClientReplyRcv (clientId, rqstId, BLOCK_ON_FINAL_REPLY, 
				  (void *) NULL, 0, (FUNCPTR) NULL, 
				  (void *) <!outputName!>, <!outputSize!>, 
				  (FUNCPTR) NULL)) == ERROR) {
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
  }
  else *bilan = OK;
  
  return(status);
}

<?
    else:
	?>
/*========================================================================
 *
 *  <!comp.name()!><!service.name!>RqstSend  -  Emission d'une requete d'execution
 *
 *  Retourne : OK ou ERROR
 */
 
STATUS <!comp.name()!><!service.name!>RqstSend (CLIENT_ID clientId, int *pRqstId,
				<!input!>
				int replyTimeOut)
{
  errnoSet(0);

  /* Emettre la requete */
  if (csClientRqstSend (clientId, <!serviceNum!>, (void *) <!inputName!>,
			<!inputSize!>, (FUNCPTR) NULL, TRUE, TIME_WAIT_REPLY, 
			replyTimeOut, pRqstId) == ERROR)
    return ERROR;
  return OK;
}

/*-------------------------------------------------------------------------
 *  <!comp.name()!><!service.name!>ReplyRcv  -  Reception des repliques
 *
 *  Retourne : ERROR ou FINAL_REPLY_OK ou 
 *             WAITING_INTERMED_REPLY ou INTERMED_REPLY_TIMEOUT ou
 *             WAITING_FINAL_REPLY    ou FINAL_REPLY_TIMEOUT
 */
 
int <!comp.name()!><!service.name!>ReplyRcv (CLIENT_ID clientId, int rqstId, 
			       int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY 
					    BLOCK_ON_INTERMED_REPLY */
			       <!output!>
			       int *activity, int *bilan)
{
  int status;    /* ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK */

  /* Verifier le flag de type de blocage */
  if (block != NO_BLOCK && block != BLOCK_ON_FINAL_REPLY 
      && block != BLOCK_ON_INTERMED_REPLY) {
    errnoSet (S_<!comp.name()!>_stdGenoM_BAD_BLOCK_TYPE);
    return (ERROR);
  }
  
  if ((status = csClientReplyRcv (clientId, rqstId, block, 
				  (void *) activity, sizeof(int), 
				  (FUNCPTR) NULL, (void *) <!outputName!>,
				  <!outputSize!>, (FUNCPTR) NULL)) == ERROR) {
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
  }
  else *bilan = OK;
  
  return(status);
}
 
/*-------------------------------------------------------------------------
 *  <!comp.name()!><!service.name!>RqstAndAck  -  Emet la requete d'exec, 
 *                                  attend la replique intermediaire
 *
 *  Retourne : ERROR ou INTERMED_REPLY_TIMEOUT ou WAITING_FINAL_REPLY ou 
 *             FINAL_REPLY_OK 
 */
 
int <!comp.name()!><!service.name!>RqstAndAck (CLIENT_ID clientId, int *pRqstId,
				 int replyTimeOut,
				 <!input!> 
				 <!output!>
				 int *activity, int *bilan)
{
  int status;
  
  errnoSet(0);
  
  /* Emettre la requete */
  if (csClientRqstSend (clientId, <!serviceNum!>, (void *) <!inputName!>,
			<!inputSize!>, (FUNCPTR) NULL, TRUE, 
			TIME_WAIT_REPLY, replyTimeOut, 
			pRqstId) == ERROR) {
    *bilan = errnoGet();
    return(ERROR);
  }
  
  /* Reception de la replique intermediaire */
  if ((status = csClientReplyRcv (clientId, *pRqstId, BLOCK_ON_INTERMED_REPLY, 
                             (void *) activity, sizeof(int), (FUNCPTR) NULL, 
                             (void *) <!outputName!>, <!outputSize!>, 
                             (FUNCPTR) NULL)) == ERROR) {
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
  }
  else *bilan = OK;
 
  return(status);
}

/*-------------------------------------------------------------------------
 *  <!comp.name()!><!service.name!>RqstAndRcv  -  Emet la requete d'execution,
 *                                  Attend les repliques.
 *
 *  Retourne : ERROR ou FINAL_REPLY_TIMEOUT ou FINAL_REPLY_OK ou 
 *             INTERMED_REPLY_TIMEOUT
 */
 
int <!comp.name()!><!service.name!>RqstAndRcv (CLIENT_ID clientId, 
				 int replyTimeOut,
				 <!input!> 
				 <!output!>
				 int *activity,
				 int *bilan)
{
  int rqstId;                  /* Ou` mettre l'id de la requete */
  int status;
  
  errnoSet(0);
  *bilan = OK;
  
  /* Emettre la requete */
  if (csClientRqstSend (clientId, <!serviceNum!>, (void *) <!inputName!>,
			<!inputSize!>, (FUNCPTR) NULL, TRUE, 
			TIME_WAIT_REPLY, replyTimeOut, 
			&rqstId) == ERROR) {
    *bilan = errnoGet();
    return(ERROR);
  }
  
  /* Reception de la replique intermediaire */
  status = csClientReplyRcv (clientId, rqstId, BLOCK_ON_INTERMED_REPLY, 
			     (void *) activity, sizeof(int), (FUNCPTR) NULL, 
			     (void *) <!outputName!>, <!outputSize!>, 
			     (FUNCPTR) NULL);
  switch(status) {

  case ERROR:
    *bilan = errnoGet();
    if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
    break;

  case WAITING_FINAL_REPLY:
    if ((status = csClientReplyRcv (clientId, rqstId, BLOCK_ON_FINAL_REPLY, 
			       (void *) NULL, 0, (FUNCPTR) NULL, 
			       (void *) <!outputName!>, <!outputSize!>, 
			       (FUNCPTR) NULL)) == ERROR) {
      *bilan = errnoGet();
      if (H2_MODULE_ERR_FLAG(*bilan)) return(FINAL_REPLY_OK);
    }
  }
  return(status);
}
