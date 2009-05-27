<?
funHeader = "void endianswap_%s(%s *x, int nDim, int *dims)\n{\n"
"  FOR_EACH_elt(nDim,dims) {";
?>
/*
 * Copyright (c) 2009 LAAS/CNRS
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
#include "<!comp.name()!>Endian.h"

<?
for t in comp.typesVect():
    prefix = typeProtoPrefix(t)
    if not prefix:
      continue
    ?>
void endianswap_<!prefix!>(<!MapTypeToC(t,True)!> *x, int nDim, int *dims)
{
  FOR_EACH_elt(nDim,dims) {
<?
    if t.kind() == IdlKind.Struct:
	s = t.asStructType()
	for m in s.members():
	  if m.data().kind() == IdlKind.Array:
		a = m.data().asArrayType()
		dims = ""
		for n in a.bounds():
		  dims += str(n) + ", "
		?>  
    { int dims[<!len(a.bounds())!>] = {<!dims[:-2]!>};
      endianswap_<!typeProtoPrefix(a.type())!>((<!MapTypeToC(a.type(), True)!> *)((x+elt)-><!m.key()!>), 1, dims); }
<?
	  elif m.data().kind() == IdlKind.Sequence:
		seq = m.data().asSequenceType()
		?>
    { int dims[1] = {<!seq.bound()!>};
      endianswap_<!typeProtoPrefix(seq.seqType())!>((<!MapTypeToC(seq.seqType(), True)!> *)((x+elt)-><!m.key()!>), 1, dims); }
<?
	  else:
	    print "   endianswap_" + typeProtoPrefix(m.data()) + "(&((x+elt)->" + m.key() + "), 0, NULL);"
    elif t.kind() == IdlKind.Enum:
	print "    endianswap_int((int *)x+elt, 0, NULL);"
    ?>
  } END_FOR
}
<?

?>

/* ======================== ENDIAN DES TYPEDEF ============================= */

<?
typeName = "%s_STR" % (upper(comp.name()))
?>
void endianswap_<!typeName!>(<!typeName!> *x, int nDim, int *dims)
{
  FOR_EACH_elt(nDim,dims) {
     endianswap_<!typeProtoPrefix(comp.IDSType)!>(&(*(x+elt)), 0, NULL);
  } END_FOR
}

<?
for p in outports:
    typeName = "%s_%s_POSTER_STR" % (upper(comp.name()), upper(p.name))
    if isDynamicPort(port):
      t = dynamicPortType(port)
    else:
      t = port.idlType
    ?>
void endianswap_<!typeName!>(<!typeName!> *x, int nDim, int *dims)
{
  FOR_EACH_elt(nDim,dims) {
     endianswap_<!typeProtoPrefix(t)!>(&(*(x+elt)), 0, NULL);
  } END_FOR
}

<?
?>
