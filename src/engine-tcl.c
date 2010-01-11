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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
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


static int	genalltypes(hash_s all, FILE *out);
static char *	gentype(idltype_s t);
static char *	gentyperef(idltype_s t);


/* --- gendotgen ----------------------------------------------------------- */

/** generate a description of the dotgen file
 */
static int
gendotgen(comp_s c, FILE *out)
{
  struct timeval t;
  int s;

  /* header information */
  gettimeofday(&t, NULL);
  fprintf(out,
	  "# %s - " PACKAGE_STRING "\n# generated from %s\n# %s#\n",
	  comp_name(c), runopt.input, ctime(&t.tv_sec));
  fprintf(out, "lappend auto_path {%s}\n", runopt.sysdir);
  fprintf(out, "if [catch {package require -exact " PACKAGE_NAME "-sys "
	  PACKAGE_VERSION "} s] {puts stderr $s; exit 2}\n");

  /* generate type dictionary */
  fprintf(out, "\n# data types\n");
  s = genalltypes(type_all(), out);
  if (s) return s;

  /* execute template */
  fprintf(out,
	  "\n# invoke template code\n"
	  "if [catch {source [file join {%s} {" TMPL_SPECIAL_FILE "%s}]} s] "
	  "{ puts %s; exit 2}\n",
	  runopt.tmpl, eng_tcl.name,
	  runopt.verbose ? "$::errorInfo" : "stderr $s");
  return 0;
}


/* --- genalltypes --------------------------------------------------------- */

/** Generate Tcl representation for all types.
 */
static int
genalltypes(hash_s all, FILE *out)
{
  hiter i;

  fprintf(out,
	  "namespace eval dotgen {\n"
	  "  variable types [dict create]\n");

  for(hash_first(all, &i); i.current; hash_next(&i)) {
    assert(type_fullname(i.value));

    fprintf(out, "  dict set types {%s} %s\n",
	    type_fullname(i.value), gentype(i.value));
  }

  fprintf(out,"}\n");
  return 0;
}


/* --- gentype ------------------------------------------------------------- */

/** Generate Tcl representation of a type. The returned string evaluates into a
 * Tcl dictionary.
 */
static char *
gentype(idltype_s t)
{
  char *b = NULL;
  tloc l;
  assert(t);

  bufcat(&b, "[dict create");
  if (type_name(t)) bufcat(&b, " name {%s}", type_name(t));
  if (type_fullname(t)) bufcat(&b, " fullname {%s}", type_fullname(t));
  bufcat(&b, " kind {%s}", type_strkind(type_kind(t)));

  switch(type_kind(t)) {
    case IDL_BOOL: case IDL_USHORT: case IDL_SHORT: case IDL_ULONG:
    case IDL_LONG: case IDL_FLOAT: case IDL_DOUBLE: case IDL_CHAR:
    case IDL_OCTET: case IDL_ANY: case IDL_ENUMERATOR:
      /* builtins */
      break;

    case IDL_CONST: {
      cval c = type_constvalue(t);
      char *e = gentyperef(type_final(t));

      bufcat(&b, " type %s", e);
      free(e);
      bufcat(&b, " value [dict create kind {%s} value {%s}]",
	     const_strkind(c.k), const_strval(c));
      break;
    }

    case IDL_ARRAY:
    case IDL_SEQUENCE: {
      char *e = gentyperef(type_elemtype(t, NULL));
      bufcat(&b, " element %s", e);
      free(e);
      /*FALLTHROUGH*/
    }
    case IDL_STRING: {
      unsigned long l = type_length(t);
      if (l != -1U) bufcat(&b, " length {%lu}", l);
      break;
    }


    case IDL_UNION: {
      char *d = gentyperef(type_discriminator(t));
      bufcat(&b, " discriminator %s", d);
      free(d);
      /*FALLTHROUGH*/
    }
    case IDL_STRUCT:
    case IDL_ENUM: {
      idltype_s m;
      char *r;
      hiter i;

      bufcat(&b, " members [list");
      for(m = type_first(t, &i); m; m = type_next(&i)) {
	r = gentyperef(m);
	bufcat(&b, " %s", r);
	free(r);
      }
      bufcat(&b, " ]");
      break;
    }

    case IDL_CASE: {
      citer i;
      clist_s l = type_casevalues(t);

      bufcat(&b, " cases [list");
      for(clist_first(l, &i); i.current; clist_next(&i)) {
	bufcat(&b, " [dict create kind {%s} value {%s}]",
	       const_strkind(i.value->k), const_strval(*i.value));
      }
      bufcat(&b, " ]");
      /*FALLTHROUGH*/
    }
    case IDL_MEMBER:
    case IDL_TYPEDEF: {
      char *e = gentyperef(type_final(t));

      bufcat(&b, " type %s", e);
      free(e);
      break;
    }

    case IDL_FORWARD_STRUCT:
    case IDL_FORWARD_UNION:
      /* there should not be any such types */
      assert(0); break;
  }

  l = type_loc(t);
  if (l->file)
    bufcat(&b, " loc [list {%s} {%d} {%d}]", l.file, l.line, l.col);

  bufcat(&b, "]");

  if (type_fullname(t))
    xwarnx("generated %s %s", type_strkind(type_kind(t)), type_fullname(t));
  return b;
}


/* --- gentyperef ---------------------------------------------------------- */

/** Generate Tcl representation of a reference to a type. The returned string
 * evaluates into a Tcl dictionary.
 */
static char *
gentyperef(idltype_s t)
{
  char *b = NULL;
  assert(t);

  if (type_fullname(t))
    bufcat(&b, "[dict create ref {%s}]", type_fullname(t));
  else
    b = gentype(t);

  return b;
}

#endif /* TCLSH */
