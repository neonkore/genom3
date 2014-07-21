/*
 * Copyright (c) 2009-2014 LAAS/CNRS
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

/*/
 * Data type and constant definitions
 * ----------------------------------
 *
 * All data types manipulated by Genom are described with a subset of the OMG
 * IDL syntax. Those types map into traditional programming languages types
 * using the specific mappings described in the
 * link:../mappings/index{outfilesuffix}[GenoM IDL mappings] section.
 */

/*/
 * === Type specification
 *
 * The following set of type specifiers to represent
 * typed values is available:
 *
 * <dotgen-rule-type-spec.adoc
 * <dotgen-rule-simple-type-spec.adoc
 * <dotgen-rule-constructed-type-spec.adoc
 *
 * ==== Subtopics
 *
 * * link:idltype-base{outfilesuffix}[IDL base types]:
 *   integers, floating point values ...
 * * link:idltype-tmpl{outfilesuffix}[IDL template types]:
 *   sequences, strings ...
 * * link:idltype-constr{outfilesuffix}[IDL constructed types]:
 *   structs, unions, enums, ...
 */

type_spec: simple_type_spec
  {
    $$ = $1; if (!$1) break;

    /* forward declarations are an invalid type specification */
    if (type_kind($1) == IDL_FORWARD_STRUCT ||
        type_kind($1) == IDL_FORWARD_UNION) {
      parserror(@1, "cannot use %s %s before it is fully defined",
                type_strkind(type_kind($1)), type_name($1));
      $$ = NULL;
      break;
    }

    /* void exceptions are an invalid type specification */
    if (type_kind($1) == IDL_EXCEPTION) {
      hiter i;
      int empty = 1;
      for(hash_first(type_members($1), &i); i.current; hash_next(&i))
        if (type_kind(i.value) == IDL_MEMBER) {
          empty = 0;
          break;
        }
      if (empty) {
        parserror(@1, "cannot use void %s %s as a regular type",
                  type_strkind(type_kind($1)), type_name($1));
        $$ = NULL;
        break;
      }
    }
  }
  | constructed_type_spec;

simple_type_spec: base_type_spec | template_type_spec | named_type;

/*/
 * === Type declaration
 *
 * Type defined with the following syntax can then be used in the component
 * interface definition and anywhere a type value is expected.
 *
 * <dotgen-rule-type-dcl.adoc
 * <dotgen-rule-constructed-type.adoc
 * <dotgen-rule-type-declarator.adoc
 *
 * ==== Typedef
 *
 * Types are most often given a unique name thanks to a `typedef` declaration;
 * a name is also associated with a data type via the `struct`, `union`,
 * `enum`, `native` and `exception` declarations.
 *
 * ==== Native types
 *
 * It is possible to hide the actual definition of a data type from Genom (and
 * its IDL parser) when it will never be directly used in the interface of the
 * component, but can nevertheless be needed by codels. These types can be
 * declared using the `native` attribute.
 *
 * For example: `native foo;`
 */

type_dcl:
  TYPEDEF type_declarator semicolon
  {
    $$ = $2;
  }
  | NATIVE identifier semicolon
  {
    $$ = type_newbasic(@1, $2, IDL_NATIVE);
  }
  | EXCEPTION exception_list semicolon
  {
    $$ = $2;
  }
  | constructed_type semicolon
  | forward_dcl ';'
;

type_declarator:
  type_spec declarator
  {
    $$ = $1; if (!$2) break;

    if ($1) $1 = dcl_settype($2, $1);
    if (!$1 || !type_newalias(@2, dcl_name($2), $1))
      parserror(@2, "dropped declaration for '%s'", dcl_name($2));

    dcl_destroy($2);
  }
  | type_declarator ',' declarator
  {
    $$ = $1; if (!$3) break;

    if ($1) $1 = dcl_settype($3, $1);
    if (!$1 || !type_newalias(@3, dcl_name($3), $1))
      parserror(@3, "dropped declaration for '%s'", dcl_name($3));

    dcl_destroy($3);
  }
;


/*/
 *
 * ==== Exceptions
 *
 * Exceptions are thrown by services in case of error. The `exception` keyword
 * declares an exception, which is an identifier with an optional associated
 * data type.
 *
 * <dotgen-rule-exception-list.adoc
 * <dotgen-rule-exception-dcl.adoc
 * <dotgen-rule-opt-member-list.adoc
 *
 * A simple exception declaration would be :
 *
 * ----
 * exception INVALID_SPEED;
 * ----
 *
 * or with an associated type (to store the actual value for instance):
 *
 * ----
 * exception INVALID_SPEED {
 *   double speed;
 * };
 * ----
 */

exception_list: exception_dcl | exception_list ',' exception_dcl;

exception_dcl: exception_name opt_member_list
  {
    scope_s s = scope_detach(scope_pop());
    assert(s == $1);
    $$ = NULL;

    if (scope_name(s)[0] == '&') {
      /* there was an error during the creation of the scope. */
      parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
      scope_destroy(s);
      break;
    }

    $$ = type_newexception(@1, scope_name(s), s);
    if (!$$) {
      parserror(@1, "dropped declaration for '%s'", scope_name(s));
      scope_destroy(s);
    }
  }
;

opt_member_list:
  /* empty */ { $$ = NULL; }
  | '{' '}' { $$ = NULL; }
  | '{' member_list '}' { $$ = $2; }
  | '{' error '}' { $$ = NULL; }
;



named_type: scoped_name
  {
    $$ = type_find($1);
    if (!$$) parserror(@1, "unknown type %s", $1);
  }
;


/* --- type declarators ---------------------------------------------------- */

declarator: simple_declarator | array_declarator;

simple_declarator: identifier
  {
    $$ = dcl_create(@1, $1);
  }
;

array_declarator:
  simple_declarator fixed_array_size
  {
    assert($2.k == CST_UINT);
    $$ = $1 ? dcl_adddim($1, $2.u) : NULL;
  }
  | array_declarator fixed_array_size
  {
    assert($2.k == CST_UINT);
    $$ = $1 ? dcl_adddim($1, $2.u) : NULL;
  }
;

fixed_array_size: '[' positive_int_const ']'
  {
    $$ = $2;
  }
;


/*/
 * === Constant declaration
 *
 * Constants define read-only values that are used by the component, like the
 * maximal speed allowed for a motor or the physical bounds of a process that
 * will be controlled. The syntax is again very similar to `C` or `C++`:
 *
 * <dotgen-rule-const-dcl.adoc
 * <dotgen-rule-const-type.adoc
 *
 * For example: `const double max_speed = 72.0;`
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
  | string_type | named_type
;
