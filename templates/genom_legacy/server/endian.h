<?
funProto = "extern void endianswap_%s(%s *x, int nDim, int *dims);"
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

#ifndef ENDIANSWAP_<!upper(comp.name())!>_H
#define ENDIANSWAP_<!upper(comp.name())!>_H

#include <stdio.h>
#include <stdlib.h>

#include <portLib.h>
#include <genom/printScan.h>
#include <genom/h2endian.h>
#include "<!comp.name()!>PosterLib.h"

<?
for i in comp.importedComponents():
    print "#include \"server/" + i + "Endian.h"
?>

/* Protoypes */

<?
for t in comp.typesVect():
    print funProto % (typeProtoPrefix(t), MapTypeToC(t,True))
?>

/* ======================== ENDIAN DES TYPEDEF ============================= */

<?
typeName = "%s_STR" % (upper(comp.name()))
print funProto % (typeName, typeName)

for p in outports:
    typeName = "%s_%s_POSTER_STR" % (upper(comp.name()), upper(p.name))
    print funProto % (typeName, typeName)
?>

#endif
