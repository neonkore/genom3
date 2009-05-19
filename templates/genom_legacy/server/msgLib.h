/* 
 * Copyright (c) 1993-2003 LAAS/CNRS
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

/*------------------  Fichier généré automatiquement ------------------*/
/*------------------  Ne pas éditer manuellement !!! ------------------*/

#ifndef <!comp.name()!>_MSG_LIB_H
#define <!comp.name()!>_MSG_LIB_H

#include "portLib.h"
#include "csLib.h"
#include "posterLib.h"
 
#include "<!comp.name()!>Type.h"

#define TIME_WAIT_REPLY           /*2000*/ 0
 



/*----------- Tailles des messages et boites aux lettres  ----------*/

/* Taille du buffer utilise dans la bal pour un mesage de taille size
 * (cf servMbox pour UNIX et h2rgnLib pour VXWORKS )
 */


#define BUF_SIZE(size) \
   (size + 4 - (size & 3) + sizeof(LETTER_HDR) + 8)

/**
 ** Boite aux lettres reception requetes 
 **/
#define  <!upper(comp.name())!>_MBOX_NAME                      "<!comp.name()!>"
<?
# todo: compute this size from all the request
?>
#define  <!upper(comp.name())!>_MAX_RQST_SIZE              <!maxArgsSize()!>

#define  <!upper(comp.name())!>_MBOX_RQST_SIZE       \
 (BUF_SIZE(<!upper(comp.name())!>_MAX_RQST_SIZE) * SERV_NMAX_RQST_ID)

/** 
 ** Taille max repliques (serveur)
 **/
#define  <!upper(comp.name())!>_MAX_INTERMED_REPLY_SIZE   (sizeof(int))

#define  <!upper(comp.name())!>_MAX_REPLY_SIZE             <!maxArgsSize()!>

/** 
 ** Taille bal clients pour reception repliques 
 **/
#define  <!upper(comp.name())!>_CLIENT_MBOX_REPLY_SIZE   \
((BUF_SIZE(<!upper(comp.name())!>_MAX_REPLY_SIZE) \
   + BUF_SIZE(<!upper(comp.name())!>_MAX_INTERMED_REPLY_SIZE)) \
 * CLIENT_NMAX_RQST_ID)

/*-------------------------- Liste des requetes  -------------------------*/

<? #$listRequests$
i = 0
for s in servicesMap:
    print "#define %s_%s_RQST %d" % (upper(comp.name()), upper(s.data().name), i)
    i += 1
print "#define %s_ABORT_RQST %d" % (upper(comp.name()), i)
?>

/*---------------- PROTOTYPES DES FONCTIONS EXTERNES ------------------*/

#ifdef __cplusplus
extern "C" {
#endif

STATUS <!comp.name()!>ClientInit (CLIENT_ID *pClientId);

STATUS <!comp.name()!>ClientEnd (CLIENT_ID clientId);

int <!comp.name()!>AbortRqstSend (CLIENT_ID clientId, 
			   int *pRqstId,
			   int *activity,
			   int replyTimeOut);

int <!comp.name()!>AbortReplyRcv (CLIENT_ID clientId, 
			   int rqstId, 
			   int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY */
			   int *bilan);

int <!comp.name()!>AbortRqstAndRcv (CLIENT_ID clientId, 
			     int *activity,
			     int *bilan);

<?
for s in servicesMap:
    service = s.data()
    serviceInfo = services_info_dict[service.name]
    serviceNum = "%s_%s_RQST" % (upper(comp.name()), upper(service.name))

    if serviceInfo.inputFlag:
	inputName = "in_" + serviceInfo.inputName
	input = serviceInfo.inputTypePtr + inputName + ","
    else:
	input = ""

    if serviceInfo.outputFlag:
	outputName = "out_" + serviceInfo.outputName
	output = serviceInfo.outputTypePtr + " " + outputName + ","
    else:
	output = ""

    if service.type == ServiceType.Control:
	?>
extern STATUS <!comp.name()!><!service.name!>RqstSend (CLIENT_ID clientId, 
					 int *pRqstId,
					 <!input!>
					 int replyTimeOut);

int <!comp.name()!><!service.name!>ReplyRcv (CLIENT_ID clientId, 
			       int rqstId, 
			       int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY */
			       <!output!>
			       int *bilan);

int <!comp.name()!><!service.name!>RqstAndRcv (CLIENT_ID clientId, 
				 <!input!> 
				 <!output!>
				 int *bilan);
<?
    else:
	?>
extern STATUS <!comp.name()!><!service.name!>RqstSend (CLIENT_ID clientId, int *pRqstId,
					 <!input!>
					 int replyTimeOut);

extern int <!comp.name()!><!service.name!>ReplyRcv (CLIENT_ID clientId, int rqstId, 
				      int block,  /* NO_BLOCK BLOCK_ON_FINAL_REPLY 
						     BLOCK_ON_INTERMED_REPLY */
				      <!output!>
				      int *activity, int *bilan);


extern int <!comp.name()!><!service.name!>RqstAndAck (CLIENT_ID clientId, int *pRqstId,
					int replyTimeOut,
					<!input!>
					<!output!>
					int *activity, int *bilan);

extern int <!comp.name()!><!service.name!>RqstAndRcv (CLIENT_ID clientId, 
					int replyTimeOut,
					<!input!> 
					<!output!>
					int *activity,
					int *bilan);

<?

?>

#ifdef __cplusplus
}
#endif
#endif /* <!comp.name()!>_MSG_LIB_H */


