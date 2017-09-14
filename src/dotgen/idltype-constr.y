/*
 * Copyright (c) 2009-2014,2017 LAAS/CNRS
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
 * IDL Constructed types
 * ---------------------
 *
 * `structs`, `unions` and `enums` are the constructed types. Their syntax is
 * presented in this section:
 *
 * ****
 * [horizontal]
 * <dotgen-rule-constructed-type.adoc
 * ****
 */

constructed_type_spec: constructed_type;
constructed_type: struct_type | union_type | enum_type;

/*/
 * === Structures
 *
 * ****
 * [horizontal]
 * <dotgen-rule-struct-type.adoc
 * <dotgen-rule-struct-name.adoc
 * <dotgen-rule-member-list.adoc
 * <dotgen-rule-member.adoc
 * ****
 *
 * The `identiﬁer` in `struct-type` defines a new legal type. Structure types
 * may also be named using a typedef declaration.
 *
 * Name scoping rules require that the member declarators in a particular
 * structure be unique.
 *
 * ==== Example
 *
 * Here is a small structure definition with one integer and one double
 * precision value:
 *
 * [source,C]
 * ----
 * struct foo {
 *   long a;
 *   double x;
 * };
 * ----
 */

struct_type: STRUCT struct_name '{' member_list '}'
  {
    scope_s s = scope_detach(scope_pop());
    assert(s == $2);
    $$ = NULL;

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
  | STRUCT struct_name error '}'
  {
    scope_s s = scope_detach(scope_pop());
    assert(s == $2);
    $$ = NULL;

    if (scope_name(s)[0] == '&') {
      /* there was an error during the creation of the scope. */
      parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
    } else
      parserror(@1, "dropped declaration for '%s'", scope_name(s));

    scope_destroy(s);
  }
;

member_list: member semicolon | member_list member semicolon;

member:
  type_spec declarator
  {
    $$ = $1; if (!$2) break;

    if ($1) $1 = dcl_settype($2, $1);
    if (!$1 || !type_newmember(@2, dcl_name($2), $1))
      parserror(@2, "dropped declaration for '%s'", dcl_name($2));

    dcl_destroy($2);
  }
  | member ',' declarator
  {
    $$ = $1; if (!$3) break;

    if ($1) $1 = dcl_settype($3, $1);
    if (!$1 || !type_newmember(@3, dcl_name($3), $1))
      parserror(@3, "dropped declaration for '%s'", dcl_name($3));

    dcl_destroy($3);
  }
;


/*/
 * === Discriminated unions
 *
 * ****
 * [horizontal]
 * <dotgen-rule-union-type.adoc
 * <dotgen-rule-union-name.adoc
 * <dotgen-rule-switch-type-spec.adoc
 * <dotgen-rule-switch-body.adoc
 * <dotgen-rule-case.adoc
 * <dotgen-rule-case-label-list.adoc
 * <dotgen-rule-case-label.adoc
 * ****
 *
 * Unions are a cross between the C union and switch statements. Unions must be
 * discriminated; that is, the union header must specify a typed tag field that
 * determines which union member to use for the current instance of a call. The
 * `identiﬁer` following the union keyword defines a new legal type. Union
 * types may also be named using a typedef declaration. The `const_exp` in a
 * `case_label` must be consistent with the `switch_type_spec`. A default case
 * can appear at most once. The `named-type` in the `switch-type-spec`
 * production must be a previously defined integer, char, boolean or enum type.
 *
 * Case labels must match or be automatically castable to the defined type of
 * the discriminator. Name scoping rules require that the element declarators
 * in a particular union be unique. It is not required that all possible values
 * of the union discriminator be listed in the `switch-body`.
 *
 * ==== Example
 *
 * [source,C]
 * ----
 * union foo switch(long) {
 *   case 0: long a;
 *   default: double x;
 * };
 * ----
 */

union_type:
  UNION union_name SWITCH '(' switch_type_spec ')' '{' switch_body '}'
  {
    scope_s s = scope_detach(scope_pop());
    assert(s == $2);
    $$ = NULL;

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
  | UNION union_name error '}'
  {
    scope_s s = scope_detach(scope_pop());
    assert(s == $2);
    $$ = NULL;

    if (scope_name(s)[0] == '&') {
      /* there was an error during the creation of the scope. */
      parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
    } else
      parserror(@1, "dropped declaration for '%s'", scope_name(s));

    scope_destroy(s);
  }
;

switch_type_spec:
  integer_type | char_type | boolean_type | enum_type | named_type
;

switch_body: case | switch_body case
  {
    $$ = $1 ? $1 : $2;
  }
;

case: case_label_list type_spec declarator semicolon
  {
    $$ = NULL;
    if (!$1 || !$2 || !$3) {
      if ($2 && !type_name($2)) type_destroy($2);
      if ($3) parserror(@3, "dropped declaration for '%s'", dcl_name($3));
      dcl_destroy($3);
      break;
    }

    $2 = dcl_settype($3, $2);
    if ($2) $$ = type_newcase(@3, dcl_name($3), $2, $1);
    if (!$2 || !$$)
      parserror(@3, "dropped declaration for '%s'", dcl_name($3));
    dcl_destroy($3);
  }
;

case_label_list:
  case_label
  {
    $$ = clist_append(NULL, $1, 1/*unique*/);
  }
  | case_label_list case_label
  {
    $$ = clist_append($1, $2, 1/*unique*/);
    if (!$$) {
      parsewarning(@2, "ignoring duplicate %s label",
		   $2.k == CST_VOID?"default":"case");
      $$ = $1;
    }
  }
;

case_label:
  CASE const_expr ':'
  {
    $$ = $2;
  }
  | DEFAULT ':'
  {
    $$.k = CST_VOID;
    $$.u = -1UL;
  }
;


/*/
 * === Enumerations
 *
 * Enumerated types consist of ordered lists of identifiers. The syntax is:
 *
 * ****
 * [horizontal]
 * <dotgen-rule-enum-type.adoc
 * <dotgen-rule-enumerator-list.adoc
 * <dotgen-rule-enumerator.adoc
 * ****
 *
 * A maximum of 2^32^ identifiers may be specified in an enumeration. The order
 * in which the identifiers are named in the specification of an enumeration
 * defines the relative order of the identifiers. The
 * `identiﬁer` following the enum keyword defines a new legal type. Enumerated
 * types may also be named using a typedef declaration.
 */

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

enumerator_list:
  enumerator
  {
    $$ = hash_create("enumerator list", 3);
    if (!$$) break;

    if (!$1) break;
    if (hash_insert($$, type_name($1), $1, NULL))
      type_destroy($1);
  }
  | enumerator_list ',' enumerator
  {
    $$ = $1;
    if (!$3) break;
    if (hash_insert($$, type_name($3), $3, NULL))
      type_destroy($3);
  }
;

enumerator: identifier
  {
    $$ = type_newenumerator(@1, $1);
  }
;


/*/
 * === Forward declarations
 *
 * ****
 * [horizontal]
 * <dotgen-rule-forward-dcl.adoc
 * ****
 *
 */

forward_dcl:
  STRUCT identifier
  {
    $$ = type_newforward(@1, $2, IDL_FORWARD_STRUCT);
  }
  | UNION identifier
  {
    $$ = type_newforward(@1, $2, IDL_FORWARD_UNION);
  }
;
