
/* 
 * Copyright (c) 2003 LAAS/CNRS
 * Sara Fleury
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

#ifndef <!upper(comp.name())!>_PRINTXML_H
#define <!upper(comp.name())!>_PRINTXML_H

#include "genom/printScan.h"
#include "genom/printXMLProto.h"

/* Structures definies par d'autres modules */
<?
for i in comp.importedComponents():
    print "#include \"server/" + i + "PrintXML.h\""
?>

#include "<!comp.name()!>PosterLib.h"

/* Prototypes */
#ifdef __cplusplus
extern "C" {
#endif

<?
for t in typesVect:
    prefix = type_proto_prefix(t)
    if not prefix:
      continue
    ?>
extern void printXML_<!prefix!> ( FILE *out, char *name,
     <!MapTypeToC(t,True)!> *x, int indent, int nDim, int *dims, FILE *in );
<?
?>

#ifdef __cplusplus
}
#endif

#endif /* <!upper(comp.name())!>_PRINTXML_H */
