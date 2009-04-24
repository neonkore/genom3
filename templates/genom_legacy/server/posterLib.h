<?
from string import *
# create a list of out ports
outports = []
for p in comp.portsMap():
    if p.data().type == PortType.Outgoing:
	outports.append(p.data())

?>
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

#ifndef <!comp.name()!>_POSTER_LIB_H
#define <!comp.name()!>_POSTER_LIB_H

#include "posterLib.h"

/* Types definis ou utilises par le module */
#include "<!comp.name()!>Type.h"
#include "<!comp.name()!>Error.h"

/* 
 * Definition de la SDI_C 
*/
typedef struct {
  CNTRL_TASK_DESCRIPTION cntrlTask;
  EXEC_TASK_DESCRIPTION execTaskTab[<!upper(comp.name())!>_NB_EXEC_TASK];
  ACTIVITY_DESCRIPTION activityTab[MAX_ACTIVITIES];
} <!upper(comp.name())!>_CNTRL_STR;

/* For consistency with other posters */
typedef <!upper(comp.name())!>_CNTRL_STR <!upper(comp.name())!>_CNTRL_POSTER_STR;
 
/* 
 * Definition des structures de posters
 */
#include "<!comp.name()!>PosterLibStruct.h"



/* Posters */
#define <!upper(comp.name())!>_CNTRL_POSTER_NAME         "<!comp.name()!>Cntrl"
<?
# $listPosterNameDeclare$
for port in outports:
    print "#define %s_%s_POSTER_NAME \"%s%s\"\n" % (upper(comp.name()), upper(port.name), comp.name(), port.name);
?>

/*---------------- PROTOTYPES DES FONCTIONS EXTERNES ------------------*/

#ifdef __cplusplus
 extern "C" {
#endif

extern STATUS <!comp.name()!>PosterInit ( void );
extern POSTER_ID <!comp.name()!>CntrlPosterID ();
extern STATUS <!comp.name()!>CntrlPosterRead ( <!upper(comp.name())!>_CNTRL_STR *<!comp.name()!>CntrlStrId );
extern STATUS <!comp.name()!>CntrlPosterInit ( void );

<?
for port in outports:
    print "extern STATUS " + comp.name() + port.name + "PosterInit ( void );"
    print "extern POSTER_ID " + comp.name() + port.name + "PosterID ( void );"
    write("extern STATUS" + comp.name() + port.name + "PosterRead ( ")
    print upper(comp.name()) + "_" + upper(port.name) + "_POSTER_STR *x );"
# done for each poster member in genom2, do it for struct members ?
#    print "extern STATUS " + comp.name() + port.name + port.name + "PosterRead ( " + port.type.toCType(true) + " *" + port.name + " );"
?>

#ifdef __cplusplus
 }
#endif

#endif
