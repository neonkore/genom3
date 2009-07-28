
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
for name, service in servicesDict.iteritems():
    print "static BOOL %sTest%s(TEST_STR *id, int rq, int ac, BOOL ch);" % (comp.name(), name)
?>

static  TEST_RQST_DESC_STR <!comp.name()!>TestRqstFuncTab[] = {
<? # $requestFuncTabDeclare$
out = ""
for name, service in servicesDict.iteritems():
    serviceInfo = services_info_dict[name]
    out += "  {%s_%s_RQST, %sTest%s, %s, %s},\n" % (upper(comp.name()), upper(name), comp.name(), name, serviceInfo.inputSize, serviceInfo.outputSize)
print out[:-2] + "};" 
?>

static char *<!comp.name()!>TestRequestNameTab[] = {
<? # $requestNameTabDeclare$
out = ""
for name, service in servicesDict.iteritems():
    out += "   \"" + name + service_description_string(service)
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
for name, service in servicesDict.iteritems():
    serviceInfo = services_info_dict[name]
    ?>
/**
 **  Emission et reception de la requete <!comp.name()!>Test<!service.name!>
 **/

static BOOL <!comp.name()!>Test<!service.name!> (TEST_STR *testId, int rqstNum, 
				    int acti, BOOL silent)

{
<? # $inputDeclarations$ 
    if serviceInfo.inputFlag:
	if serviceInfo.inputType.kind() == IdlType.String:
	  print "int inputDims[1] = {" + str(serviceInfo.inputType.asStringType().bound()) + "};"
	  inputNbDimensions = 1
	else:
	  print  "int *inputDims = NULL;"
	  inputNbDimensions = 0
 # $outputDeclarations$
    if serviceInfo.outputFlag:
	print  "int *outputDims = NULL;"

    if service.type != Service.Control:
	?>
  /* Saisie données */
  if (!TEST_ACTIVITY_ON(testId, acti)) {
<?
 # $inputScan$
    if serviceInfo.inputFlag:
	outputServices = find_service_with_same_output(service,serviceInfo.inputName)
	#similarPosters = findPosterWithSameOutput()
	#todo meme chose avec les posters

	funcnames = ""
	for ss in outputServices:
	    if funcnames != "":
		funcnames += ", "
	    funcnames += "\"request " + ss.name + " " + service_description_string(ss) 
	    if ss.inputs():
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
  printf ("-- Enter <!serviceInfo.inputTypePtr!> <!serviceInfo.inputName!>:");
  scan_<!serviceInfo.inputTypeProto!>(stdin, stdout, (<!serviceInfo.inputTypePtr!>)TEST_RQST_INPUT(testId,rqstNum), <!serviceInfo.inputNewline!>, <!inputNbDimensions!>, inputDims);
<?
    if service.type != Service.Control:
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
    if serviceInfo.outputFlag:
	?>
  if (!silent) {
    printf ("<!serviceInfo.outputName!> = ");
    print_<!serviceInfo.outputTypeProto!>(stdout, (<!serviceInfo.outputTypeC!> *)TEST_RQST_OUTPUT(testId,rqstNum), <!serviceInfo.outputNewline!>, 0, outputDims, stdin);
  }
<?
    ?>
  return TRUE;
}
<?
?>
