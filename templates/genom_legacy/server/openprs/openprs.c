
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

#include <stdlib.h>

#include <portLib.h>

#include "<!comp.name()!>MsgLib.h"
#undef FREE

#include <macro-pub.h>
#include <opaque-pub.h>
#include <constant-pub.h>
#include <oprs-type-pub.h>
#include <pu-mk-term_f.h>
#include <pu-parse-tl_f.h>

#include <oprs-type_f-pub.h>
#include <oprs-sprint_f-pub.h>

#include <user-decl-oprs-post_f.h>
#include <user-decl-oprs-com_f.h>

#include <oprs-com.h>
#include <oprs-com_f.h>
#include <oprs-post_f.h>

#include <pu-enum_f.h>
#include <pu-genom_f.h>

#include "<!comp.name()!>Error.h"
#include "<!comp.name()!>MsgLib.h"
#include "<!comp.name()!>PosterLib.h"

#include "<!comp.name()!>DecodeOpenprs.h"
#include "<!comp.name()!>EncodeOpenprs.h"

/*----------------------------------------------------------------------*/

/*
 * Requests declaration
 */
void init_<!comp.name()!>_rqst_type_table ()
{
<?
for name,service in servicesDict.iteritems():
  serviceInfo = services_info_dict[service.name]
  ?>
  init_rqst_type("<!upper(comp.name())!>_<!upper(service.name)!>", <!upper(comp.name())!>_<!upper(service.name)!>_RQST,
<?
  if serviceInfo.inputFlag: ?>
               (Encode_Func_Proto)pu_encode_genom_<!type_proto_prefix(serviceInfo.inputType)!>, sizeof(<!MapTypeToC(serviceInfo.inputType)!>),
<?
  else:?> 
               null_decode, 0,
<?
  if serviceInfo.outputFlag: ?>
               (Decode_Func_Proto)pu_decode_genom_<!type_proto_prefix(serviceInfo.outputType)!>, sizeof(<!MapTypeToC(serviceInfo.outputType)!>));
<?
  else: ?>
               null_decode, 0);
<?
?>
  init_rqst_type("<!upper(comp.name())!>_ABORT", <!upper(comp.name())!>_ABORT_RQST,
               (Encode_Func_Proto)pu_encode_genom_int, sizeof(int),
               null_decode, 0);
}

/*
 * ATOM declaration (reports + enum)
 */
void init_<!comp.name()!>_module ()
{
  /* Enums */
<?
for t in typesVect: 
  if t.kind() != IdlKind.Enum:
    continue
  e = t.asEnumType()
  for x in e.enumerators(): ?>
  PU_DECLARE_ENUM_ATOM(<!x!>);
<?
?>

  /* Reports */
<?
for e in errorList:?>
  PU_DECLARE_ENUM_ATOM(S_viam_<!e!>);
<?
?>
}

/*
 * Posters declaration
 */
void init_<!comp.name()!>_posters ()
{
    void *x;
<?
for port in outports: 
  if is_dynamic_port(port):
    continue
  ?>
   declare_poster_function("|<!comp.name()!><!port.name!>|",
                 "<!upper(comp.name())!>_<!upper(port.name)!>",
                 (Decode_Func_Proto)pu_decode_genom_<!type_proto_prefix(port.idlType)!>,
                 (Encode_Func_Proto)pu_encode_genom_<!type_proto_prefix(port.idlType)!>,
                 0);
<?
?>
}


