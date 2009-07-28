
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

#include <oprs-type_f-pub.h>

#include <pu-enum_f.h>
#include <pu-genom_f.h>

#include "<!comp.name()!>Type.h"

#include "<!comp.name()!>PosterLib.h"

#include "<!comp.name()!>DecodeOpenprs.h"

<?
for t in typesVect:
  if t.identifier() is None or t.identifier() == IDSType.identifier() or is_dynamic(t):
    continue

  ctype = MapTypeToC(t, True)
  ?>
Term *pu_decode_genom_<!type_proto_prefix(t)!>(char *name, <!ctype!> *str, int tabsize)
{
  Pred_Func_Rec *fr = find_or_create_pred_func(declare_atom((name?name:"<!t.identifier()!>")));
  TermList tl = sl_make_slist();
  int elt;
  for(elt=0;elt<tabsize;elt++) {
<?
  if t.kind() == IdlType.Struct:
    s = t.asStructType()
    for m in s.members():
      if m.data.kind() == IdlType.Array:
	arraySize = m.data.asArrayType().bounds()[0]
	arrayType = m.data.asArrayType().type() 
	?>
      sl_add_to_tail(tl, pu_decode_genom_<!type_proto_prefix(arrayType)!>("<!m.key!>", (<!MapTypeToC(arrayType, True)!> *) (str+elt)-><!m.key!>, <!arraySize!>));
<?
      elif m.data.kind() == IdlType.String: ?>
      sl_add_to_tail(tl, pu_decode_genom_string("<!m.key!>", (str+elt)-><!m.key!>, 1, <!m.data.asStringType().bound()!>));
<?
      else: ?>
      sl_add_to_tail(tl, pu_decode_genom_<!type_proto_prefix(m.data)!>("<!m.key!>", &(str+elt)-><!m.key!>, 1));
<?
  elif t.kind() == IdlType.Enum:
    e = t.asEnumType()
    elseStr = ""
    for x in e.enumerators(): 
      ?>
      <!elseStr!>if (*(str+elt) == <!x!>) add_to_tail(tl, PUMakeTermAtom("<!x!>"));
<?
      elseStr = "else "
    ?>
      else {
        fprintf(stdout, "Not valid value %d for enum <!t.identifier()!>\n", *(str+elt));
        add_to_tail(tl, PUMakeTermAtom("UNKNOWN_<!t.identifier()!>_ENUM_VALUE"));
      }
<?
  else: ?>
<?
  ?>
  }
  return build_term_expr(build_expr_pfr_terms(fr, tl));
}
<?
?>

<?
for t in typesVect:
  if t.identifier() is None or t.identifier() == IDSType.identifier() or is_dynamic(t):
    continue
  ctype = MapTypeToC(t, True)
  ?>
Term *pu_decode_genom_<!t.identifier()!>(char *name, <!ctype!> *str, int tabsize)
{
      return  pu_decode_genom_<!type_proto_prefix(t)!>(name, str, tabsize);
}
<?
?>
