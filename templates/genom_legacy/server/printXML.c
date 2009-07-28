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

#include <stdio.h>
#include <stdlib.h>

#include <portLib.h>
#include <csLib.h>
#include <posterLib.h>
#include "<!comp.name()!>PrintXML.h"

<?
for t in typesVect:
    prefix = type_proto_prefix(t)
    if not prefix:
      continue
    ?>
void printXML_<!prefix!>( FILE *out, char *name,
     <!MapTypeToC(t,True)!> *x, int indent, int nDim, int *dims, FILE *in )
{
  char *indstr;
  indstr=strdup(indentStr(nDim?++indent:indent));
  indent++;
  FOR_NB_elt(nDim,dims) {
    fprintfBuf(out, "%s<%s%s>\n", indstr, name, getIndexesStr2(nDim, dims, elt));

<?
    if t.kind() == IdlType.Struct:
	s = t.asStructType()
	for m in s.members():
	    if m.data.kind() == IdlType.Array:
		a = m.data.asArrayType()
		dims = ""
		for n in a.bounds():
		  dims += str(n) + ", "
		?>
    { int dims[<!len(a.bounds())!>] = {<!dims[:-2]!>};
      printXML_<!type_proto_prefix(a.type())!>(out, "<!m.key!>", (<!MapTypeToC(a.type(), True)!>*)((x+elt)-><!m.key!>), indent, 1, dims, in); }
<?
	    elif m.data.kind() == IdlType.Sequence:
		seq = m.data.asSequenceType()
		?>
    { int dims[1] = {<!seq.bound()!>};
      printXML_<!type_proto_prefix(seq.seqType())!>(out, "<!m.key!>", (<!MapTypeToC(seq.seqType(), True)!>*)((x+elt)-><!m.key!>.data), indent, 1, dims, in); }
    printXML_int(out, "<!m.key!>.length" ,&((x+elt)-><!m.key!>.length), indent, 0, NULL, in);
    printXML_int(out, "<!m.key!>.size" ,&((x+elt)-><!m.key!>.size), indent, 0, NULL, in);

<?
	    else:
		?> 
    printXML_<!type_proto_prefix(m.data)!>(out, "<!m.key!>" ,&((x+elt)-><!m.key!>), indent, 0, NULL, in);
<? 
    elif t.kind() == IdlType.Enum:
	e = t.asEnumType()
	?>
    /* Affiche l'ancienne valeur */ 
    switch (*(x+elt)) { <?
	for x in e.enumerators():
	    ?>
	case <!x!>:
	    fprintfBuf(out, "<!x!>"); break;<?
	?>
	default:
	    fprintf(out, "unknown enum value %d\n", *(x+elt));
    } /* switch */
<?
    ?>

    fprintfBuf(out, "%s</%s%s>\n", indstr, name, getIndexesStr2(nDim, dims, elt));
  } END_FOR
  free(indstr);
}
<?

?>