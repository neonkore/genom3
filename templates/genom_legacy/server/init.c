<?

if initService == 0:
    requestFlag = False
    nbInputParams = "0"
    inputDeclare = ""
    inputNamePtrC = ""
    inputSize = "0"
    inputShow = ""
    inputNamePtr = "NULL"
    requestNum = "0"

    inputUsage = ""
    inputA2Type = ""
    inputFlat = "void"
    inputFlatDeclare = ""
    inputFillStruct = ""
    inputFlatNamePtrC = ""
    inputVerif = ""
else:
    requestFlag = True
    request = initService.name
    requestNum = comp.serviceIndex(initService.name)

    if len(initService.inputs()) == 0: # no inputs parameters
	inputUsage = ""
	inputA2Type = ""
	inputFlat = "void"
	inputFlatDeclare = ""
	inputFillStruct = ""
	inputFlatNamePtrC = ""
	inputVerif = ""

	nbInputParams = "0"
	inputSize = "0"
#	inputShow = ""
	inputNamePtr = "NULL"
#	requestNum = "0"  is it a bug ?
	flatList = []
	inputDeclare = ""
    else:
	inputName = initService.inputs()[0]
	inputType = typeFromIdsName(inputName)
	inputDeclare = MapTypeToC(inputType,True) + " " + inputName + ";"
	inputSize = "sizeof(" + MapTypeToC(inputType,True) + ")"
# todo: reuse name from print.c
#	inputShow =  "print_" + MapTypeToC(inputType,True) + "(stdout, " + ");"
	inputNamePtr = "&" + inputName
	inputNamePtrC = inputNamePtr + ","

	nbInputParams = "0"
	inputUsage = ""

	flatList = flatStruct(inputType, inputName)
	inputFlatNamePtrC = ""
	inputFlat = ""
	first = True
	for x in flatList:
	    if not first:
		inputFlatNamePtrC += ", "
		inputFlat += ", "
	    else:
		first = False
	    inputFlatNamePtrC += x[1]
	    inputFlat += MapTypeToC(x[0],True) + " " + x[1]
?>
/* 
 * Copyright (c) 1993-2004 LAAS/CNRS
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
 
/****************************************************************************
 *   LABORATOIRE D'AUTOMATIQUE ET D'ANALYSE DE SYSTEMES - LAAS / CNRS       
 *   PROJET HILARE II - ROUTINE D'INIT INTERACTIF       
 *   FICHIER SOURCE: <!comp.name()!>Init.c                                          
 ****************************************************************************/

/* VERSION ACTUELLE / HISTORIQUE DES MODIFICATIONS :
 */

/* DESCRIPTION :
   Cette routine doit etre appelee dans le shell de vxWorks ou UNIX.
   */

/*----------------------------- INCLUSIONS ---------------------------------*/

/* Inclusions generales */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <portLib.h>
#include <errnoLib.h>
#include <h2initGlob.h>

#define <!upper(comp.name())!>_PROMPT "usage: <!comp.name()!>SendInitRqst (<!inputFlat!>)\n"

/* Inclusions pour acceder au module */
#include "<!comp.name()!>MsgLib.h"

/*------------------- PROTOTYPE DES FONCTIONS EXPORTEES -------------------*/

STATUS <!comp.name()!>SendInitRqst ( <!inputFlat!> );

/*------------------- PROTOTYPES DES FONCTIONS LOCALES --------------------*/

<?
if requestFlag:?>
static STATUS <!comp.name()!>InitInitTask (CLIENT_ID *clientId, int size);
static STATUS <!comp.name()!>InitEnd (CLIENT_ID clientId);
<?
?>

/*---------------------------   MAIN  ------------------------------------*/

int
main(int argc, char **argv)
{
<?
if requestFlag:
    for x in flatList:
	print " " + MapTypeToC(x[0]) + " " + x[1] + ";"
?>

  if (--argc != <!nbInputParams!>) {
    fprintf(stderr, <!upper(comp.name())!>_PROMPT);
    exit(1);
  }

  /* Conversion des parametres */
<?
if requestFlag:
    i = 0
    for x in flatList:
	c = convertFun(x[0])
	if c == "":
	    print "   " + x[1] + " = "  + "argv[" + str(i) + "];"
	else:
	    print "   " + x[1] + " = " + c + "(argv[" + str(i) + "]);"
	i += 1
?>
  /* Appel de la fonction d'init */
  <!comp.name()!>SendInitRqst(<!inputFlatNamePtrC!>);


  return 0;
} /* main */

/**
 **   <!comp.name()!>Init - Emission de la requete d'init
 **/

STATUS <!comp.name()!>SendInitRqst (<!inputFlat!>)
{
<?
if requestFlag:
    ?>
  CLIENT_ID initCId;        /* Id client */
  int bilan=OK;
  int status;
  int rqstId;
  int activity;
  int size=<!inputSize!>;
  <!inputDeclare!>

  /* Defini la valeur d'un tic (en nombre de tic par seconde) */
  h2initGlob(20);

  /* Initialisation de la tache <!comp.name()!>Init */
  if (<!comp.name()!>InitInitTask (&initCId,size) == ERROR)
    return (ERROR);
	  
  /* Instanciation de la structure parametre de la requete */
<?
    for x in flatList:
	print "   " + inputName + "." + x[1].replace("__", ".") + " = " + x[1] + ";"
    ?>
  
  /* Affichage de la structure parametre de la requete */
<?
    for x in flatList:
	write("   printf(\"" + inputName + "." + x[1].replace("__", ".") + " = " + formatStringForType(x[0]) + "\\n\", ")
	print inputName + "." + x[1].replace("__", ".") + ");"
    ?>

  /* Pb: il faut la bibliotheque print.c (possible mais lourd)
     $inputShow$  */

  errnoSet(0);
  
  /* Emettre la requete */
  if ((status = csClientRqstSend (initCId, <!requestNum!>, 
				  (void *) <!inputNamePtr!>,
				  size, (FUNCPTR) NULL, 
				  TRUE, 0, 
				  TIME_WAIT_REPLY, &rqstId)) == ERROR) {
    bilan = errnoGet();
    printf("Error (status %d) ", status);
    h2printErrno(bilan);
    return ERROR;
  }
  
  /* Reception de la replique intermediaire */
  status = csClientReplyRcv (initCId, rqstId, 
			     BLOCK_ON_INTERMED_REPLY, 
			     (void *) &activity, sizeof(int), 
			     (FUNCPTR) NULL, 
			     (void *) NULL, 0, 
			     (FUNCPTR) NULL);

  /* On traite les status */
  switch(status) {
    case WAITING_FINAL_REPLY:
      printf ("-- Intermed reply: OK\n");
      if (csClientReplyRcv (initCId, rqstId, BLOCK_ON_FINAL_REPLY, 
			    (void *) NULL, 0, (FUNCPTR) NULL, 
			    (void *) NULL, 0, 
			    (FUNCPTR) NULL) == ERROR) {
          bilan = errnoGet();
          if (H2_MODULE_ERR_FLAG(bilan))
	    printf("-- Final reply: ");
	  else 
	    printf("** Error : ");
      }
      else
	printf("-- Final reply: ");
      break;

    case FINAL_REPLY_OK:
      printf("-- Final reply: ");
      break;

    case FINAL_REPLY_TIMEOUT:
      printf("** Final reply time out: \n");
      break;

    case ERROR:
      if (H2_MODULE_ERR_FLAG(bilan))
	printf("-- Final reply: ");
      else 
	printf("** Error : ");
      break;

    default:
      printf("** Error (status %d) ", status);
      break;
    } /* switch */
  bilan = errnoGet();
  h2printErrno(bilan);

  /* On netoie */
  <!comp.name()!>InitEnd(initCId);

  if (status != FINAL_REPLY_OK || bilan != OK)
    return ERROR;

  return OK;

<?
else:
    ?>
  printf ("There is no init request\n");
  return OK;

<?

?>
}



/*----------------------- ROUTINES LOCALES ---------------------------------*/


<?
if requestFlag:
    ?>
/*****************************************************************************
 *
 *  <!comp.name()!>InitInitTask - Routine d'initialisation de la tache d'essai
 *
 *  Description:
 *
 *  Retourne : OK ou ERROR
 */

static STATUS <!comp.name()!>InitInitTask (CLIENT_ID *clientId, int size)
     
{
  char mboxName[30];                /* Nom de base de la boite aux lettres */

  /* Copier le nom de base des mboxes */
  (void) strcpy (mboxName, "t<!comp.name()!>Init");
    
  /* Initialisation des boites aux lettres */
  if (csMboxInit (mboxName, 0, <!upper(comp.name())!>_CLIENT_MBOX_REPLY_SIZE) != OK) {
    h2perror("<!comp.name()!>Init: csMboxInit failed\n");
    return (ERROR);
  }


  /* S'initialiser comme client */
  if (csClientInit(<!upper(comp.name())!>_MBOX_NAME, 
		   size,
		   sizeof(int),
		   sizeof(int),
		   clientId) != OK) {
      h2perror("<!comp.name()!>Init: <!comp.name()!>ClientInit failed\n");
      csMboxEnd();
      return (ERROR);
    }

  /* Initialisation OK */
  return (OK);
}
 
 
/*****************************************************************************
 *
 *  <!comp.name()!>InitEnd  -  Termine proprement la tache
 *
 *  Description:
  *  Cette fonction termine les taches en-cours et desalloue les objets.
 *
 *  Retourne: OK ou ERROR
 */

static STATUS <!comp.name()!>InitEnd (CLIENT_ID initCId)

{
  /* Desalouer les objets */
  if (csClientEnd (initCId) != OK || csMboxEnd () != OK ) {
    printf ("<!comp.name()!>Init: pb liberation des objets alloue's\n");
    return (ERROR);
  }
  
  /* Bravo, c'est fini */
  return (OK);
}


<?

?>
