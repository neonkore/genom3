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
#include "<!comp.name()!>Print.h"

<?
for t in comp.typesVect():
    ?>
int scan_<!typeProtoPrefix(t)!>( FILE *in, FILE *out,
       <!MapTypeToC(t,True)!> *x, int indent, int nDim, int *dims )
{
  char *indstr;
  indstr=strdup(indentStr(nDim?++indent:indent));
  indent++;
  FOR_NB_elt(nDim,dims) {
    if (nDim != 0)
      fprintf(out, "%s%s", indentStr(indent-2), getIndexesStr(nDim, dims, elt));

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
    fprintf(out, "%s<!m.key()!><!a.printBounds()!>:\n", indstr);
    { int dims[<!len(a.bounds())!>] = {<!dims[:-2]!>};
      if (scan_<!typeProtoPrefix(a.type())!>(in, out, (<!MapTypeToC(a.type(), True)!> *)(x+elt)-><!m.key()!>, 
                     indent, 1, dims) == ABORT) {
      free (indstr);
      return ABORT; } }
<?
	    elif m.data().kind() == IdlKind.Sequence:
		seq = m.data().asSequenceType()
		?>
    fprintf(out, "%s<!m.key()!>[<!seq.bound()!>]:\n", indstr);
    { int dims[1] = {<!seq.bound()!>};
      if (scan_<!typeProtoPrefix(seq.seqType())!>(in, out, (<!MapTypeToC(seq.seqType(), True)!> *)(x+elt)-><!m.key()!>, 
                     indent, 1, dims) == ABORT) {
      free (indstr);
      return ABORT; } }
<?
	    else:
		?>
    fprintf(out, "%s<!m.key()!>:\n", indstr);
    if(scan_<!typeProtoPrefix(m.data())!>(in, out, &((x+elt)-><!m.key()!>), indent, 0, NULL) == ABORT) {
       free (indstr);
       return ABORT;
    }
<?
    elif t.kind() == IdlKind.Enum:
	e = t.asEnumType()
	eqStr = ""
	?>
    /* Affiche l'ancienne valeur */ 
    switch (*(x+elt)) { <?
	for x in e.enumerators():
	    if eqStr != "":
		eqStr += " && "
	    eqStr += x + " != *(x+elt)"
	    ?>
	case <!x!>:
	    fprintf(out, "<!x!> =%d\n", <!x!>); break;<?
	?>
	default:
	    fprintf(out, "unknown enum value %d\n", *(x+elt));
    } /* switch */

    /* Scan l'enum */
    { int trouve;
      do {
        trouve = 1;
        if (scan_type(in, out, "%d", x+elt) == ABORT) {
          free (indstr);
          return ABORT; }
        if (<!eqStr!>) {
          fprintf(out, "Unknown value, select between:");
<?
	for x in e.enumerators():
	  print "          fprintf(out,\"\\n   " + x + "=%d   \", " + x + ");"
	?>
	  trouve = 0;
        }
      } while (! trouve);
    }
<?
    ?>

  } END_FOR
  free(indstr);
  return OK;
}
<?
?>


