
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
errorSet = create_error_list()
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
errorSet = create_error_list()
for e in errorSet:
    print "    case ERROR_" + e + ": return S_"+ comp.name() + "_" + e + ";"
?>
    default:
      return S_stdGenoM_CONTROL_CODEL_ERROR;
  }
}

<?
for name, service in servicesDict.iteritems():
    for c in service.codels():
	codel = c.data()
	if service.type != Service.Control and c.key() != "control" :
	    continue
	# check for a special 'connect inport' service
	if len(service.inputs()) == 1 and service.inputs()[0].identifier == connectIDSMember:
	    continue
	?>
extern <!real_codel_signature(codel, service)!>;

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

<!codel_full_signature(c, service)!>
{
<?
	codel_lock(codel, service)
	?>

  /*call real codel */
  int res = <!real_codel_call(codel, service)!>;
  if(res < 0)
      *report = returnCodeToReport(res);

<?
	codel_release(codel, service)
	?>

  return returnCodeToStatus(res);
}
<?

# 'connect inport' services' codels
idx = len(outports)
for port in inports:
  name = "connect" + port.name
  service = servicesDict[name]
  posterAddr = port.name + "_inport"
  posterType = MapTypeToC(port.idlType, True);
  posterId = upper(comp.name()) + "_" + upper(port.name) + "_POSTER_ID"
  ?>
/*------------------------------------------------------------------------
 * <!name!>_codel  -  control codel of CONTROL request <!name!>
 *
 * Description:    
 * Report: OK     
 *
 * Returns:    OK or ERROR
 */
STATUS <!codel_signature(service.codel("control"), service)!>
{
    POSTER_ID posterId;
    if (posterFind(in__connect_str, &posterId) == ERROR) {
	*report = errnoGet();
	return ERROR;
    }
    CNTRL_TASK_POSTER_ID[<!idx!>] = posterId;

<?
  if is_dynamic(port.idlType):
    print "/* find a pointer to " + port.name + " poster*/"
    print posterType + " *" + posterAddr + " = posterAddr(" + posterId + ");"
    print "if ("+posterAddr+" == NULL) {"
    print "  *report = errnoGet();"
    print "  return ETHER;"
    print "}"

    print "posterTake(" + posterId + ", POSTER_READ);"

    print "int currentOffset = sizeof(" + posterType + ");"
    print "char *start = (char*) " + posterAddr + ";"

    copy_type(port.idlType,  "SDI_F->" + posterAddr,  "(*" + posterAddr + ")")
    print "posterGive(" + posterId + ");"
  ?>

    return OK;
}
<?
  idx += 1
?>
