
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

<?
for s in servicesMap:
    print "static BOOL %sTest%s(TEST_STR *id, int rq, int ac, BOOL ch);" % (comp.name(), s.data().name)
?>

static  TEST_RQST_DESC_STR <!comp.name()!>TestRqstFuncTab[] = {
<? # $requestFuncTabDeclare$
out = ""
for s in servicesMap:
    service = s.data()
    if len(service.inputs()) > 0:
	inputSize = sizeOfIdsMember(service.inputs()[0])
    else:
	inputSize = "0"
    outputSize = sizeOfIdsMember(service.output)
    out += "  {%s_%s_RQST, %sTest%s, %s, %s},\n" % (upper(comp.name()), upper(service.name), comp.name(), service.name, inputSize, outputSize)
print out[:-2] + "};" 
?>

static char *<!comp.name()!>TestRequestNameTab[] = {
<? # $requestNameTabDeclare$
out = ""
for s in servicesMap:
    service = s.data()
    out += "   \"" + service.name + serviceDescString(service)
    out+= "\",\n"
print out[:-2] + "\n};" 
?>

static char *<!comp.name()!>TestPosterNameTab[] = {
<? # $posterNameTabDeclare$
out = ""
for port in outports:
    out += "   \"%s\",\n" % port.name
print out[:-2] + "\n};" 
?>

static STATUS (*<!comp.name()!>TestPosterShowFuncTab[])() = {
<? #  $posterShowFuncTabDeclare$
for port in outports:
    print "   " + comp.name() + port.name + "PosterShow,"
?>   <!comp.name()!>CntrlPosterShow,
   <!comp.name()!>CntrlPosterActivityShow
};

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
			  <!upper(comp.name())!>_CLIENT_MBOX_REPLY_SIZE,
			  <!upper(comp.name())!>_ABORT_RQST,
			  <!nbServices!>,
			  <!comp.name()!>TestRequestNameTab,
			  <!comp.name()!>TestRqstFuncTab,
			  <!len(outports)!>, 
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
  if (csClientInit (<!upper(comp.name())!>_MBOX_NAME, <!upper(comp.name())!>_MAX_RQST_SIZE,
		    <!upper(comp.name())!>_MAX_INTERMED_REPLY_SIZE, 
		    <!upper(comp.name())!>_MAX_REPLY_SIZE, &TEST_CID(testStr)) != OK) {
      (void) h2perror("Client init failed");
      testEnd(testStr);
  }
  
  /* S'initialiser comme client des posters */
  printf ("ok.   Poster init ...");
  if (<!comp.name()!>PosterInit () != OK)
    h2perror ("Warning: not found all <!comp.name()!>'s posters");

  printf ("ok.");
}
 
<?
for s in servicesMap:
    service = s.data()
    inputFlag = len(service.inputs()) > 0
    if inputFlag:
	inputName = service.inputs()[0]
	t = typeFromIdsName(inputName)
	if t != None:
	    b = t.kind() == IdlKind.Struct or t.kind() == IdlKind.Typedef or t.kind() == IdlKind.Array or t.kind() == IdlKind.Named
	    if b:
		inputNewline = "1"
	    else:
		inputNewline = "0"
	inputType = t.toCType(True)
	inputTypeProto = typeProtoPrefix(t)

    outputFlag = len(service.output) > 0
    if outputFlag:
	outputName = service.output
	t = typeFromIdsName(outputName)
	if t != None:
	    b = t.kind() == IdlKind.Struct or t.kind() == IdlKind.Typedef or t.kind() == IdlKind.Array or t.kind() == IdlKind.Named
	    if b:
		outputNewline = "1"
	    else:
		outputNewline = "0"
	outputType = t.toCType(True)
	outputTypeProto = typeProtoPrefix(t)
    ?>
/**
 **  Emission et reception de la requete <!comp.name()!>Test<!service.name!>
 **/

static BOOL <!comp.name()!>Test<!service.name!> (TEST_STR *testId, int rqstNum, 
				    int acti, BOOL silent)

{
<? # $inputDeclarations$ 
    if inputFlag:
	print  "int *inputDims = NULL;"
 # $outputDeclarations$
    if outputFlag:
	print  "int *outputDims = NULL;"

    if service.type != ServiceType.Control:
	?>
  /* Saisie données */
  if (!TEST_ACTIVITY_ON(testId, acti)) {
<?
 # $inputScan$
    if inputFlag:
	outputServices = findServiceWithSameOutput(service,inputName)
	#similarPosters = findPosterWithSameOutput()
	#todo meme chose avec les posters

	funcnames = ""
	for ss in outputServices:
	    if funcnames != "":
		funcnames += ", "
	    funcnames += "\"request " + ss.name + " " + serviceDescString(ss) 
	    if len(ss.inputs()) > 0:
		funcnames += " - needs input "
	    funcnames += "\""

	if funcnames != "":
	    ?>
  const char *menu[] = {"<!inputName!>", <!funcnames!>};
  if(testPrintGetInputMenu(testId, <!len(outputServices) + 1!>, menu, rqstNum)) {
      switch(TEST_RQST_GET_CMD(testId,rqstNum)) {
<?
	    i = 0
	    for ss in outputServices:
		?>
	case <!i!>:
	  if (!testSendAndGetInput(testId, <!upper(comp.name())!>_<!upper(ss.name)!>_RQST, acti, TEST_RQST_INPUT(testId,rqstNum)))
	    return FALSE;
<?
		i+= 1
	    ?>
	default:
	  break;
      }
  }
<?
	?>
  printf ("-- Enter <!inputType!> <!inputName!>:");
  scan_<!inputTypeProto!>(stdin, stdout, (<!inputType!> *)TEST_RQST_INPUT(testId,rqstNum), <!inputNewline!>, 0, inputDims);
<?
    if service.type != ServiceType.Control:
	?>
  }

  /* Emission d'une requete + replique intermediaire 
     ET/OU reception replique finale (selon TEST_ACTIVITY_RQST_ID) */
  if (!testSendAndOrRcvExecRqst(testId, rqstNum, acti, silent))
     return FALSE;
<?
    else:
	?>
  if (!testSendAndRcvCntrlRqst(testId, rqstNum, acti, silent))
     return FALSE;

<? #  $outputPrint$
    if outputFlag:
	?>
  if (!silent) {
    printf ("<!outputName!> = ");
    print_<!outputTypeProto!>(stdout, (<!outputType!> *)TEST_RQST_OUTPUT(testId,rqstNum), <!outputNewline!>, 0, outputDims, stdin);
  }
<?
    ?>
  return TRUE;
}
<?
?>
