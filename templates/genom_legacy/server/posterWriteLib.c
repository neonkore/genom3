/* 
 * Copyright (c) 1993-2005 LAAS/CNRS
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

/*--------------  file automaticaly generated with GenoM -------------*/
/*-----------------------  DO NOT EDIT BY HAND -----------------------*/
 
/****************************************************************************/
/*   LABORATOIRE D'AUTOMATIQUE ET D'ANALYSE DE SYSTEMES - LAAS / CNRS       */
/*   PROJET HILARE II - INTERACTION PAR POSTER AVEC LE MODULE  <!upper(comp.name())!>     */
/*   FICHIER SOURCE : "<!comp.name()!>PosterWriteLib.c"                                 */
/****************************************************************************/

/* DESCRIPTION :
   Bibliotheque de fonctions qui permettent l'écriture
   du contenu des posters du module
*/

#include <stdio.h>
#include <stdlib.h>

#include <portLib.h>
#include <posterLib.h>
#include "<!comp.name()!>Print.h"

<?
for p in outports:
    ?>
/* --  <!p.name!>  ------------------------------------------------- */

STATUS <!comp.name()!><!p.name!>PosterWrite(POSTER_ID pid, <!upper(comp.name())!>_<!upper(p.name)!>_POSTER_STR *x)
{
  return (posterWrite(pid, 0, (char *)x, sizeof(*x))
            == sizeof(*x) ? OK : ERROR);
}
<?
?>
