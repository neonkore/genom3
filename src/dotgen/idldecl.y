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


/* --- type definion ------------------------------------------------------- */

/*/
 * Type definition
 * ---------------
 *
 * A type definition is the description of a type. It can be used in a
 * `typedef` construct or anywhere a typed value is expected.
 *
 * <dotgen-rule-type-spec.adoc
 * <dotgen-rule-simple-type-spec.adoc
 * <dotgen-rule-base-type-spec.adoc
 * <dotgen-rule-template-type-spec.adoc
 * <dotgen-rule-constructed-type-spec.adoc
 * <dotgen-rule-named-type.adoc
 * <dotgen-rule-scoped-name.adoc
 * <dotgen-rule-declarator.adoc
 * <dotgen-rule-simple-declarator.adoc
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
constructed_type_spec: constructed_type;

named_type: scoped_name
  {
    $$ = type_find($1);
    if (!$$) parserror(@1, "unknown type %s", $1);
  }
;

base_type_spec:
  boolean_type | integer_type | floating_pt_type | char_type | octet_type |
  any_type;
template_type_spec: sequence_type | optional_type | string_type | fixed_type;

integer_type: signed_int | unsigned_int;
floating_pt_type: float_type | double_type;

signed_int: signed_longlong_int | signed_long_int | signed_short_int;
unsigned_int: unsigned_longlong_int | unsigned_long_int | unsigned_short_int;

unsigned_short_int: UNSIGNED SHORT
  { $$ = type_newbasic(@1, NULL, IDL_USHORT); };
unsigned_long_int: UNSIGNED LONG
  { $$ = type_newbasic(@1, NULL, IDL_ULONG); };
unsigned_longlong_int: UNSIGNED LONG LONG
  { $$ = type_newbasic(@1, NULL, IDL_ULONGLONG); };
signed_short_int: SHORT
  { $$ = type_newbasic(@1, NULL, IDL_SHORT); };
signed_long_int: LONG
  { $$ = type_newbasic(@1, NULL, IDL_LONG); };
signed_longlong_int: LONG LONG
  { $$ = type_newbasic(@1, NULL, IDL_LONGLONG); };
float_type: FLOAT
  { $$ = type_newbasic(@1, NULL, IDL_FLOAT); };
double_type: DOUBLE
  { $$ = type_newbasic(@1, NULL, IDL_DOUBLE); };
char_type: CHAR
  { $$ = type_newbasic(@1, NULL, IDL_CHAR); };
boolean_type: BOOLEAN
  { $$ = type_newbasic(@1, NULL, IDL_BOOL); };
octet_type: OCTET
  { $$ = type_newbasic(@1, NULL, IDL_OCTET); };
any_type: ANY
  { $$ = type_newbasic(@1, NULL, IDL_ANY); };

string_type:
  STRING '<' positive_int_const '>'
  {
    assert($3.k == CST_UINT);
    $$ = type_newstring(@1, NULL, $3.u);
  }
  | STRING
  {
    $$ = type_newstring(@1, NULL, -1U);
  }
;

sequence_type:
  SEQUENCE '<' simple_type_spec ',' positive_int_const '>'
  {
    assert($5.k == CST_UINT);
    $$ = $3 ? type_newsequence(@1, NULL, $3, $5.u) : NULL;
  }
  | SEQUENCE '<' simple_type_spec '>'
  {
    $$ = $3 ? type_newsequence(@1, NULL, $3, -1U) : NULL;
  }
;

optional_type:
  OPTIONAL '<' simple_type_spec '>'
  {
    $$ = $3 ? type_newoptional(@1, NULL, $3) : NULL;
  }
;

fixed_type:
  FIXED '<' positive_int_const ',' positive_int_const '>'
  {
    $$ = NULL;
    parserror(@1, "fixed point types not implemented");
  }
  | FIXED
  {
    $$ = NULL;
    parserror(@1, "fixed point types not implemented");
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

opt_member_list:
  /* empty */ { $$ = NULL; }
  | '{' '}' { $$ = NULL; }
  | '{' member_list '}' { $$ = $2; }
  | '{' error '}' { $$ = NULL; }
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
