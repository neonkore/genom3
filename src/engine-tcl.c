/*
 * Copyright (c) 2010 LAAS/CNRS
 * All rights reserved.
 *
 * Redistribution  and  use  in  source  and binary  forms,  with  or  without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of  source  code must retain the  above copyright
 *      notice and this list of conditions.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice and  this list of  conditions in the  documentation and/or
 *      other materials provided with the distribution.
 *
 * THE SOFTWARE  IS PROVIDED "AS IS"  AND THE AUTHOR  DISCLAIMS ALL WARRANTIES
 * WITH  REGARD   TO  THIS  SOFTWARE  INCLUDING  ALL   IMPLIED  WARRANTIES  OF
 * MERCHANTABILITY AND  FITNESS.  IN NO EVENT  SHALL THE AUTHOR  BE LIABLE FOR
 * ANY  SPECIAL, DIRECT,  INDIRECT, OR  CONSEQUENTIAL DAMAGES  OR  ANY DAMAGES
 * WHATSOEVER  RESULTING FROM  LOSS OF  USE, DATA  OR PROFITS,  WHETHER  IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR  OTHER TORTIOUS ACTION, ARISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *                                           Anthony Mallet on Thu Jan  7 2010
 */
#include "acgenom.h"
#ifdef TCLSH

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "genom.h"


/* --- local data ---------------------------------------------------------- */

static int	gendotgen(comp_s c, FILE *out);

const engdescr eng_tcl = {
  .name =	"tcl",
  .interp =	TCLSH,
  .gendotgen =	gendotgen
};


/* --- gendotgen ----------------------------------------------------------- */

/** generate a description of the dotgen file
 */
static int
gendotgen(comp_s c, FILE *out)
{
  struct timeval t;

  /* header information */
  gettimeofday(&t, NULL);
  fprintf(out,
	  "# %s - " PACKAGE_STRING "\n" "# generated from %s\n" "# %s",
	  comp_name(c), runopt.input, ctime(&t.tv_sec));
  fprintf(out, "package require Tcl 8.5\n");

  return 0;
}

#endif /* TCLSH */
