<?
stdGenomErrors = ["ACTIVITY_INTERRUPTED", \
  "TOO_MANY_ACTIVITIES", "ACTIVITY_FAILED", \
  "WAIT_ABORT_ZOMBIE_ACTIVITY", "UNKNOWN_ACTIVITY", \
  "FORBIDDEN_ACTIVITY_TRANSITION", "SYSTEM_ERROR", \
  "ACTIVITY_ALREADY_ENDED", "WAIT_INIT_RQST"," EXEC_TASK_SUSPENDED", \
  "CONTROL_CODEL_ERROR", "BAD_BLOCK_TYPE", \
  "BAD_POSTER_TYPE"]

errorSet = createErrorList()
?>
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

/*------------------  Fichier généré automatiquement ------------------*/
/*------------------  Ne pas éditer manuellement !!! ------------------*/


#ifndef <!comp.name()!>_ERROR_H
#define <!comp.name()!>_ERROR_H

#include "h2errorLib.h"
#include "genom/genomError.h"

#define M_<!comp.name()!>    (<!comp.uniqueId!>)

/* -- MODULES ERRORS -------------------------------------------------- */

/* demo errors */

<? # $listCntrlFailures$$listExecFailures$
i = 1
for e in errorSet:
    print "#define S_" + comp.name() + "_" + e + " H2_ENCODE_ERR(M_" + comp.name() + ", " + str(i) + ")"
    i += 1
?>

/* std errors */
<? # $listStdFailures$
i = 1
for s in stdGenomErrors:
    print "#define S_stdGenoM_" + s + " " + str(encodeError(i))
    i += 1
?>

/* static H2_ERROR[] */
#define <!upper(comp.name())!>_H2_ERR_MSGS {\
<? # $listTabFailures$
i = 1
out = ""
for e in errorSet:
    out += "   {\"" + e + "\", " + str(i) +"}, \\\n"
    i += 1
print out[:-4] + " \\ \n}"
?>

extern int <!comp.name()!>RecordH2errMsgs(void);



/*-------------------------- end file loading ---------------------------*/
#endif
