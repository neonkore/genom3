
/* --- FILE GENERATED WITH GENOM, DO NOT EDIT BY HAND ------------------ */

/* 
 * Copyright (c) 2003-2005 LAAS/CNRS
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

/* DESCRIPTION :
   Bibliotheque de fonctions qui permettent la lecture au format xml
   du contenu des posters du module depuis un XML
*/

#include <stdio.h>
#include <stdlib.h>

#include <portLib.h>
#include <h2timeLib.h>

#include "<!comp.name()!>PosterLib.h"
#include "<!comp.name()!>PosterXMLLib.h"
#include "<!comp.name()!>PrintXML.h"

/*  prototypes de la bibliotheque printState */
extern char const * h2GetEvnStateString(int num);

<?
# $execTaskNameTabDeclare$
write("static char *" + comp.name() + "ExecTaskNameTab[] = {\n");
task_list = ""
for t in comp.tasksMap():
    task_list += "\"" + t.data().name + "\",\n"
print task_list[:-2] # remove the last ',' 
print "};"
?>

static GENOM_POSTER_XML tabPosterXML[] = {
{"Cntrl", "", <!comp.name()!>CntrlPosterXML},
<? #$tabPosterXML$
l = ""
nbPosterXML = 1
for p in outports:
    l += "{\"%s\", \"\", %s%sPosterXML},\n" % (p.name, comp.name(), p.name)
    nbPosterXML += 1
print l[:-2] + "\n};"
?>


/*****************************************
   MAIN PROGRAM
*****************************************/

void
web<!comp.name()!>(FILE *f, int argc, char **argv, char **argn)
{
  int poster;
  
  fprintfBuf(f,
	  "HTTP/1.0 200 OK\nPragma: no-cache\n"
	  "Content-Type: text/plain\n\n");

  /* debut du xml */
  xmlHeader(f);
  xmlBalise("<!comp.name()!>",BEGIN_BALISE_NEWLINE,f,0);
  xmlBalise("errorParam",BEGIN_BALISE,f,1);

  /* get parameters */
  poster = getPosterXML(f, argc, argv, argn, <!nbPosterXML!>, tabPosterXML);

  fprintfBuf(f, "</errorParam>\n");

  if (poster == -2) {
    posterListXML(f, <!nbPosterXML!>, tabPosterXML);
  }
  else if (poster != -1) {
    tabPosterXML[poster].posterXMLFunc(f);
  }

  xmlBalise("<!comp.name()!>",TERMINATE_BALISE, f, 0);
  
}

/* ---------------- LE POSTER DE CONTROLE ------------------------------ */

static void 
<!comp.name()!>ActivitiesXML (FILE *f, <!upper(comp.name())!>_CNTRL_STR *sdic)
{
#define <!upper(comp.name())!>_NB_RQST_EXEC <!nb_exec_service()!>
#if <!upper(comp.name())!>_NB_RQST_EXEC != 0

<? #  $requestNameTabDeclare$
write("static char *" + comp.name() + "ExecRqstNameTab[] = {\n");
service_list = ""
for name, service in servicesDict.iteritems():
    if service.type != Service.Control:
	service_list += "\"" + name + "\",\n"
print service_list[:-2] # remove the last ',' 
print "};"

print "static int " + comp.name() + "ExecRqstNumTab[] = {"
i = 0
l = ""
for name, service in servicesDict.iteritems(): 
    if service.type != Service.Control:
	l +=  str(i) + ", "
    i += 1
print l[:-2] + "};"
?>

  int i;
  ACTIVITY_EVENT evn;
  ACTIVITY_STATE status;
  int bilan;
  int rqst;
  char strerr[64];

  for (i=0; i<MAX_ACTIVITIES; i++) {
    status = M_ACTIVITY_STATUS(sdic,i);
    bilan = M_ACTIVITY_BILAN(sdic,i);

    if (status != ETHER || bilan != OK) {
      evn = M_ACTIVITY_EVN(sdic,i);
      
      xmlBalise("activity", BEGIN_BALISE_NEWLINE, f, 1);

      xmlBalise("id", BEGIN_BALISE, f, 2);
      fprintf(f, "%d", M_ACTIVITY_ID(sdic, i));
      xmlBalise("id", TERMINATE_BALISE, f, 0);
      
      /* find the name */
      rqst=0;
      while(<!comp.name()!>ExecRqstNumTab[rqst] != M_ACTIVITY_RQST_TYPE(sdic,i) 
	    && rqst<<!upper(comp.name())!>_NB_RQST_EXEC)
	rqst++;
      xmlBalise("name", BEGIN_BALISE, f, 2);
      fprintf(f, "%s", 
	  rqst == <!upper(comp.name())!>_NB_RQST_EXEC?
	  "Unknown" : <!comp.name()!>ExecRqstNameTab[rqst]);
      xmlBalise("name", TERMINATE_BALISE, f, 0);
      
      xmlBalise("task", BEGIN_BALISE, f, 2);
      fprintf(f, "%s", 
	  M_ACTIVITY_TASK_NUM(sdic,i) == -1 ? 
	  "not exec" : <!comp.name()!>ExecTaskNameTab[M_ACTIVITY_TASK_NUM(sdic,i)]);
      xmlBalise("task", TERMINATE_BALISE, f, 0);

      xmlBalise("status", BEGIN_BALISE, f, 2);
      fprintf(f, "%s", 
	  evn == NO_EVENT ?
	  h2GetEvnStateString(status) : h2GetEvnStateString(evn));
      xmlBalise("status", TERMINATE_BALISE, f, 0);
      
      xmlBalise("errno", BEGIN_BALISE, f, 2);
      fprintf(f, "%s", h2getErrMsg(bilan, strerr, 64));
      xmlBalise("errno", TERMINATE_BALISE, f, 0);

      xmlBalise("activity", TERMINATE_BALISE, f, 1);
    }

  }	/* for */

#endif /* <!upper(comp.name())!>_NB_RQST_EXEC != 0 */
}

/* ----------------------------------------------------------------------
 *
 *  <!comp.name()!>CntrlPosterXML - Affichage du poster de controle du module
 *
 */

STATUS <!comp.name()!>CntrlPosterXML (FILE *f)
{
  <!upper(comp.name())!>_CNTRL_STR *sdic;
  int i;
  char strerr[64];

  /* Read the control IDS */
  sdic = (<!upper(comp.name())!>_CNTRL_STR *)malloc(sizeof(<!upper(comp.name())!>_CNTRL_STR));
  if (sdic == NULL) {
    h2perror ("<!comp.name()!>CntrlPosterXML");
    return ERROR;
  }
  if (<!comp.name()!>CntrlPosterRead (sdic) != OK) {
    free(sdic);
    return ERROR;
  }

  /* 
   * display 
   */
  /* control task */
  xmlBalise("task", BEGIN_BALISE_NEWLINE, f, 1);

  xmlBalise("name", BEGIN_BALISE, f, 2);
  fprintf(f, "ControlTask");
  xmlBalise("name", TERMINATE_BALISE, f, 0);

  xmlBalise("status", BEGIN_BALISE, f, 2);
  fprintf(f, "%s", 
	  M_CNTRL_TASK_STATUS(sdic)==OK ? "OK":"ERROR");
  xmlBalise("status", TERMINATE_BALISE, f, 0);
  if (M_CNTRL_TASK_STATUS(sdic) != OK) {
    xmlBalise("errno", BEGIN_BALISE, f, 2);
    fprintf(f, "%s", h2getErrMsg(M_CNTRL_TASK_BILAN(sdic), strerr, 64));
    xmlBalise("errno", TERMINATE_BALISE, f, 0);
  }
  xmlBalise("task", TERMINATE_BALISE, f, 1);

  /* execution tasks */
  for (i=0; i<<!upper(comp.name())!>_NB_EXEC_TASK; i++) {
    xmlBalise("task", BEGIN_BALISE_NEWLINE, f, 1);

    xmlBalise("name", BEGIN_BALISE, f, 2);
    fprintf(f, "%s", <!comp.name()!>ExecTaskNameTab[i]);
    xmlBalise("name", TERMINATE_BALISE, f, 0);

    xmlBalise("status", BEGIN_BALISE, f, 2);
    fprintf(f, "%s", M_EXEC_TASK_STATUS(sdic,i) == OK ? "OK": "ERROR");
    xmlBalise("status", TERMINATE_BALISE, f, 0);
    if (M_EXEC_TASK_STATUS(sdic, i) != OK) {
      xmlBalise("errno", BEGIN_BALISE, f, 2);
      fprintf(f, "%s", h2getErrMsg(M_EXEC_TASK_BILAN(sdic,i), strerr, 64));
      xmlBalise("errno", TERMINATE_BALISE, f, 0);
    }

    if (M_EXEC_TASK_PERIOD(sdic,i) != 0) {
      xmlBalise("period", BEGIN_BALISE, f, 2);
      fprintfBuf (f, "%d", (int)(M_EXEC_TASK_PERIOD(sdic,i)*1000.));
      xmlBalise("period", TERMINATE_BALISE, f, 0);
    }
    xmlBalise("task", TERMINATE_BALISE, f, 1);

  }

  /* Activities */
  <!comp.name()!>ActivitiesXML(f, sdic);

  free(sdic);
  return OK;
}

/**
 ** Dump only activities. XXX not used ? 
 **/
STATUS <!comp.name()!>CntrlPosterActivityXML (FILE *f)
{
  <!upper(comp.name())!>_CNTRL_STR *sdic;

  /* Lecture de la SDI de controle */
  sdic = (<!upper(comp.name())!>_CNTRL_STR *)malloc(sizeof(<!upper(comp.name())!>_CNTRL_STR));
  if (sdic == NULL) {
    h2perror ("<!comp.name()!>CntrlPosterActivityXML");
    return ERROR;
  }
  if (<!comp.name()!>CntrlPosterRead (sdic) != OK) {
    free(sdic);
    return ERROR;
  }
  <!comp.name()!>ActivitiesXML(f, sdic);
  free(sdic);
  return OK;
}

/* ---------------- LES POSTERS FONCTIONNELS ------------------------------ */

<?
for p in outports:
    poster_type = upper(comp.name()) + "_" + upper(p.name) + "_POSTER_STR"
    if is_dynamic_port(port):
      t = dynamic_port_type(port)
    else:
      t = port.idlType
    ?>
/* --  <!p.name!> ------------------------------------------------- */

STATUS <!comp.name()!><!p.name!>PosterXML(FILE *f)
{
  BOOL err=FALSE;
  H2TIME posterDate;
  <!poster_type!> x;

  xmlBalise("<!p.name!>",BEGIN_BALISE_NEWLINE,f,1);
  xmlBalise("error",BEGIN_BALISE,f,2);

  if (<!comp.name()!><!p.name!>PosterRead(&x) == ERROR) {
    h2perror("<!comp.name()!><!p.name!>PosterXML");
    err=TRUE;
    fprintfBuf(f, "poster read failed");
  }
  fprintfBuf(f, "</error>\n");
  if (!err) {
    printXML_<!type_proto_prefix(t)!>(f, "data", &x, 2, 0, NULL, NULL);
    posterIoctl(<!comp.name()!><!p.name!>PosterID(), FIO_GETDATE, &posterDate);
    xmlBalise("date",BEGIN_BALISE,f,2);
    fprintf(f, "%04d/%02d/%02d %02d:%02d:%02d.%03d",
             posterDate.year + 1900, posterDate.month,
             posterDate.date + 1, posterDate.hour,
             posterDate.minute, posterDate.sec,
             posterDate.msec);
    fprintf(f, "</date>\n");
  }
  xmlBalise("<!p.name!>",TERMINATE_BALISE,f,1);
  return OK;
}
<?
?>
