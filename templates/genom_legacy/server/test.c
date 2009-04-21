
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
 
/****************************************************************************
 *   LABORATOIRE D'AUTOMATIQUE ET D'ANALYSE DE SYSTEMES - LAAS / CNRS       
 *   PROJET HILARE II - ROUTINE D'TEST INTERACTIF       
 *   FICHIER SOURCE: <!comp.name()!>Test.c                                          
 ****************************************************************************/

/* VERSION ACTUELLE / HISTORIQUE DES MODIFICATIONS :
 */

/* DESCRIPTION :
   Cette routine doit etre appelee dans le shell de vxWorks ou UNIX.
   */

/*----------------------------- INCLUSIONS ---------------------------------*/

/* Inclusions generales */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#include <portLib.h>
#include <h2initGlob.h>
#include <taskLib.h>

/* Inclusions pour acceder au module */
#include "<!comp.name()!>MsgLib.h"
#include "<!comp.name()!>PosterShowLib.h" 
#include "<!comp.name()!>Print.h"
#include "<!comp.name()!>Scan.h"

/* #include "<!comp.name()!>Reports.h" */

#include "genom/testLib.h"


void <!comp.name()!>Test (int testNumber);

/*------------------- PROTOTYPES DES FONCTIONS LOCALES --------------------*/

static void <!comp.name()!>TestInitTask (TEST_STR* testStr);

$requestFuncTabDeclare$
  $requestNameTabDeclare$

  $posterNameTabDeclare$
  $posterShowFuncTabDeclare$


/*--------------------------------------------------------------------------*/

int
main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "usage: <!comp.name()!>Test <numMBox>\n");
    exit(1);
  }
  if (h2initGlob(0) == ERROR) {
      exit(2);
  }
  <!comp.name()!>Test(atoi(argv[1]));
  /*NOTREACHED*/
  return 0;
} /* main */

/**
 **   <!comp.name()!>Test - Fonction de test du module <!comp.name()!>
 **/

void
<!comp.name()!>Test (int testNumber)     
{
  TEST_STR *testStr;
  
  /* Allocation de la structure */
  if ((testStr = testInit(testNumber, "<!comp.name()!>",
			  $MODULE$_CLIENT_MBOX_REPLY_SIZE,
			  $MODULE$_ABORT_RQST,
			  $nbRequest$,
			  <!comp.name()!>TestRequestNameTab,
			  <!comp.name()!>TestRqstFuncTab,
			  $nbPosterData$, 
			  <!comp.name()!>TestPosterNameTab,
			  <!comp.name()!>TestPosterShowFuncTab)) == NULL)
    return;

  /* Init specifiques */
  <!comp.name()!>TestInitTask (testStr);
  
  /* Fonction principale */
  testMain(testStr);
}

/*----------------------- ROUTINES LOCALES ---------------------------------*/


/**
 **  <!comp.name()!>TestInitTask - Routine d'initialisation de la tache d'essai
 **/

static void <!comp.name()!>TestInitTask (TEST_STR *testStr)
     
{
  /* S'initialiser comme client */
  printf ("client init ...");
  if (csClientInit ($MODULE$_MBOX_NAME, $MODULE$_MAX_RQST_SIZE,
		    $MODULE$_MAX_INTERMED_REPLY_SIZE, 
		    $MODULE$_MAX_REPLY_SIZE, &TEST_CID(testStr)) != OK) {
      (void) h2perror("Client init failed");
      testEnd(testStr);
  }
  
  /* S'initialiser comme client des posters */
  printf ("ok.   Poster init ...");
  if (<!comp.name()!>PosterInit () != OK)
    h2perror ("Warning: not found all <!comp.name()!>'s posters");

  printf ("ok.");
}
 
