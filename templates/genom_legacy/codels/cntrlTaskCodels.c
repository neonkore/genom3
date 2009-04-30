
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

#include <portLib.h>

#include "server/<!comp.name()!>Header.h"
#include "userCodels.h"

STATUS returnCodeToStatus(int res)
{
  switch(res) {
<?
errorSet = createErrorList()
for e in errorSet:
    print "    case ERROR_" + e + ": return ERROR;"
?>
    case USER_OK:  return OK;
    default:
	return ERROR;
  }
}

int returnCodeToReport(int res)
{
  switch(res) {
<?
errorSet = createErrorList()
for e in errorSet:
    print "    case ERROR_" + e + ": return S_"+ comp.name() + "_" + e + ";"
?>
    default:
      return S_stdGenoM_CONTROL_CODEL_ERROR;
  }
}

<?
for s in comp.servicesMap():
    service = s.data()
    for c in service.codels():
	codel = c.data()
	if service.type != ServiceType.Control and c.key() != "control" :
	    continue
	?>
/*------------------------------------------------------------------------
 * <!codel.name!>_codel  -  control codel of CONTROL request <!service.name!>
 *
 * Description:    
 * Report: OK
<?
	for m in service.errorMessages():
	    print " *   S_" + comp.name() + "_" + m
	?> *         
 *
 * Returns:    OK or ERROR
 */

<!codelSignatureFull(c, s.data())!>
{<?
	for port in codel.outPorts:
	    posterId = upper(comp.name()) + "_" + upper(port) + "_POSTER_ID"
	    posterAddr = "outport_" + port
	    ?>
  /* find a pointer to <!port!> poster*/
  static <!upper(comp.name())!>_<!upper(port)!>_POSTER_STR *<!posterAddr!> = NULL;
  <!posterAddr!> = posterAddr(<!posterId!>);
  if (<!posterAddr!> == NULL) {
    *report = errnoGet();
    return ERROR;
  }<?
	?>
  /* Lock access to posters*/<?
	for port in codel.outPorts:
	    ?>
  posterTake(<!upper(comp.name())!>_<!upper(port)!>_POSTER_ID, POSTER_WRITE);<?
	?>

  /*call real codel */
  int res = <!real_codel_call(codel)!>;
  if(res < 0)
      *report = returnCodeToReport(res);

  /* release lock on posters */<?
	for port in codel.outPorts:
	    ?>
  posterGive(<!upper(comp.name())!>_<!upper(port)!>_POSTER_ID);
<?
	?>
  return returnCodeToStatus(res);
}
<?
?>

