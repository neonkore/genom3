/*
 * Copyright (c) 2009-2012 LAAS/CNRS
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
 *                                           Anthony Mallet on Mon Apr 20 2009
 */

/*/ @node Constant declaration
 * @section Constant declaration
 * @cindex Constant, declaration
 *
 * @ruleinclude const_dcl
 * @ruleinclude const_type
 */

const_dcl:
  CONST const_type identifier '=' const_expr semicolon
  {
    assert($3);
    $$ = $2 ? type_newconst(@1, $3, $2, $5) : NULL;
    if (!$$) {
      if ($2 && !type_name($2)) type_destroy($2);
      parserror(@1, "dropped declaration for '%s'", $3);
    }
  }
;

const_type:
  integer_type | char_type | boolean_type | floating_pt_type | octet_type
  | string_type
  | named_type
  {
    $$ = $1; if (!$$) break;

    switch(type_kind(type_final($$))) {
      case IDL_BOOL: case IDL_USHORT: case IDL_SHORT: case IDL_ULONG:
      case IDL_LONG: case IDL_ULONGLONG: case IDL_LONGLONG: case IDL_FLOAT:
      case IDL_DOUBLE: case IDL_CHAR: case IDL_OCTET: case IDL_STRING:
      case IDL_ENUM:
	break;

      case IDL_ANY: case IDL_ENUMERATOR: case IDL_ARRAY: case IDL_SEQUENCE:
      case IDL_STRUCT: case IDL_UNION: case IDL_FORWARD_STRUCT:
      case IDL_FORWARD_UNION: case IDL_PORT: case IDL_REMOTE: case IDL_NATIVE:
	parserror(@1, "%s %s is not a valid constant type",
		  type_strkind(type_kind($$)), $1);
	parsenoerror(type_loc($$), "  %s %s declared here",
		     type_strkind(type_kind($$)), $1);
	$$ = NULL;
	break;

      case IDL_CASE: case IDL_MEMBER: case IDL_CONST: case IDL_TYPEDEF:
	/* not a valid return from type_final() */
	assert(0); break;
    }
  }
;


/* --- IDL type definitions ------------------------------------------------ */

/*/ @node Type declaration
 * @section Type declaration
 * @cindex Type, declaration
 *
 * Type declarations define new data types and associate a name (an identifier)
 * with it. The @code{typedef} keyword can be used to name an existing
 * type. The constructed types @code{struct}, @code{union} and @code{enum} also
 * name the type they define. The syntax is the following:
 *
 * @ruleinclude type_dcl
 * @ruleinclude constructed_type
 * @ruleinclude alias_list
 */

type_dcl:
  constructed_type semicolon
  | TYPEDEF alias_list semicolon
  {
    $$ = $2;
  }
  | NATIVE identifier semicolon
  {
    $$ = type_newbasic(@1, $2, IDL_NATIVE);
  }
  | forward_dcl
;

constructed_type: struct_type | union_type | enum_type;

alias_list:
  type_spec declarator
  {
    $$ = $1; if (!$2) break;

    if ($1) $1 = dcl_settype($2, $1);
    if (!$1 || !type_newalias(@2, dcl_name($2), $1))
      parserror(@2, "dropped declaration for '%s'", dcl_name($2));

    dcl_destroy($2);
  }
  | alias_list ',' declarator
  {
    $$ = $1; if (!$3) break;

    if ($1) $1 = dcl_settype($3, $1);
    if (!$1 || !type_newalias(@3, dcl_name($3), $1))
      parserror(@3, "dropped declaration for '%s'", dcl_name($3));

    dcl_destroy($3);
  }
;

struct_type: STRUCT scope_push_struct '{' member_list '}'
  {
    scope_s s = scope_detach(scope_pop());
    assert(s == $2);

    if (scope_name(s)[0] == '&') {
      /* there was an error during the creation of the scope. */
      parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
      scope_destroy(s);
      break;
    }

    $$ = type_newstruct(@1, scope_name(s), s);
    if (!$$) {
      parserror(@1, "dropped declaration for '%s'", scope_name(s));
      scope_destroy(s);
    }
  }
  | STRUCT scope_push_struct error '}'
  {
    scope_s s = scope_detach(scope_pop());
    assert(s == $2);

    if (scope_name(s)[0] == '&') {
      /* there was an error during the creation of the scope. */
      parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
    } else
      parserror(@1, "dropped declaration for '%s'", scope_name(s));

    scope_destroy(s);
  }
;

union_type:
  UNION scope_push_union SWITCH '(' switch_type_spec ')' '{' switch_body '}'
  {
    scope_s s = scope_detach(scope_pop());
    assert(s == $2);

    if (scope_name(s)[0] == '&') {
      /* there was an error during the creation of the scope. */
      parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
      if ($5 && !type_name($5)) type_destroy($5);
      scope_destroy(s);
      break;
    }
    if (!$8 || !$5) {
      parserror(@1, "dropped declaration for '%s'", scope_name(s));
      if ($5 && !type_name($5)) type_destroy($5);
      scope_destroy(s);
      break;
    }

    $$ = type_newunion(@1, scope_name(s), $5, s);
    if (!$$) {
      parserror(@1, "dropped declaration for '%s'", scope_name(s));
      if ($5 && !type_name($5)) type_destroy($5);
      scope_destroy(s);
    }
  }
  | UNION scope_push_union error '}'
  {
    scope_s s = scope_detach(scope_pop());
    assert(s == $2);

    if (scope_name(s)[0] == '&') {
      /* there was an error during the creation of the scope. */
      parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
    } else
      parserror(@1, "dropped declaration for '%s'", scope_name(s));

    scope_destroy(s);
  }
;

enum_type: ENUM identifier '{' enumerator_list '}'
  {
    $$ = $4 ? type_newenum(@2, $2, $4) : NULL;
    if (!$$) {
      if ($4) {
	hiter i;
	for(hash_first($4, &i); i.current; hash_next(&i))
	  type_destroy(i.value);
	hash_destroy($4, 1);
      }
      parserror(@1, "dropped declaration for '%s'", $2);
    }
  }
;

forward_dcl:
  STRUCT identifier ';'
  {
    $$ = type_newforward(@1, $2, IDL_FORWARD_STRUCT);
  }
  | UNION identifier ';'
  {
    $$ = type_newforward(@1, $2, IDL_FORWARD_UNION);
  }
;
