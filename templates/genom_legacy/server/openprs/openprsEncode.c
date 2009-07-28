
/* 
 * Copyright (c) 1999-2003 LAAS/CNRS
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

/*----------------------------------------------------------------------
 *   Automatically generated: do not edit by hand.
 *----------------------------------------------------------------------*/

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <portLib.h>

#undef FREE

#include <macro-pub.h>
#include <opaque-pub.h>
#include <constant-pub.h>
#include <oprs-type-pub.h>
#include <pu-mk-term_f.h>
#include <pu-parse-tl_f.h>

#include <pu-enum_f.h>
#include <pu-genom_f.h>

#include "<!comp.name()!>Type.h"
#include "<!comp.name()!>PosterLib.h"
#include "<!comp.name()!>EncodeOpenprs.h"

<?
for t in typesVect:
  if t.identifier() is None or t.identifier() == IDSType.identifier():
    continue

  ctype = MapTypeToC(t, True)
  ?>
Term *pu_encode_genom_<!type_proto_prefix(t)!>(char *name, Expression *tc,
                 <!ctype!> *str, int tabsize)
{
  if (!pu_check_ttc_name(tc, name,"ViamSave"))
    return FALSE;
  else {
    int elt;
    for(elt=0;elt<tabsize;elt++) {
<?
  if t.kind() == IdlKind.Struct:
    s = t.asStructType()
    counter = 1
    for m in s.members():
      ?>
      Expression *tc_tmp;
      if (! PUGetOprsTermCompSpecArg(tc, elt*tabsize+<!counter!>, EXPRESSION, &tc_tmp))
         return FALSE;
<?
      counter += 1
      if m.data.kind() == IdlKind.Array:
	arraySize = m.data.asArrayType().bounds()[0]
	arrayType = m.data.asArrayType().type() 
	?>
      if (! pu_encode_genom_<!type_proto_prefix(arrayType)!>("<!m.key!>", tc_tmp,  (<!type_proto_prefix(arrayType)!> *) (str+elt)-><!m.key!>, <!arraySize!>))
         return FALSE;
<?
      elif m.data.kind() == IdlKind.String:
	?>
      if (! pu_encode_genom_string("<!m.key!>", tc_tmp, (str+elt)-><!m.key!>, <!m.data.asStringType().bound()!>))
         return FALSE;
<?
      else: ?>
      if (! pu_encode_genom_<!type_proto_prefix(m.data)!>("<!m.key!>", tc_tmp, &(str+elt)-><!m.key!>, 1))
         return FALSE;
<?
  elif t.kind() == IdlKind.Enum:
    ?>
      Symbol val;
      if (!PUGetOprsTermCompSpecArg(tc, elt+1, ATOM, &val))
	   return FALSE;
<?
    elseStr = ""
    for x in t.asEnumType().enumerators(): 
      ?>
      <!elseStr!> if (strcmp(val,"<!x!>")==0) *(str+elt) = <!x!>;
<?
      elseStr = "else"
    ?>
      else {
        fprintf(stdout, "Not valid value %s for enum <!t.identifier()!>", val);
        return FALSE;
      }
<?
  ?>
    }
  }
  return TRUE;
}
<?
?>

<?
for t in typesVect:
  if t.identifier() is None or t.identifier() == IDSType.identifier():
    continue
  ctype = MapTypeToC(t, True)
  ?>
Term *pu_encode_genom_<!t.identifier()!>(char *name, Expression *tc,
                 <!ctype!> *str, int tabsize)
{
      return  pu_encode_genom_<!type_proto_prefix(t)!>(name, tc, str, tabsize);
}
<?
?>