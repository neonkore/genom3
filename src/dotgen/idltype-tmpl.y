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
 * IDL Template types
 * ------------------
 *
 * The template types are:
 * ****
 * [horizontal]
 * <dotgen-rule-template-type-spec.adoc
 * ****
 */
template_type_spec: sequence_type | optional_type | string_type | fixed_type;

/*/
 * === Sequences
 *
 * A sequence is a one-dimensional array with two characteristics: a maximum
 * size (which is fixed at compile time) and a length (which is determined at
 * run time).
 *
 * ****
 * [horizontal]
 * <dotgen-rule-sequence-type.adoc
 * ****
 *
 * The second parameter in a sequence declaration indicates the maximum size of
 * the sequence. If a positive integer constant is specified for the maximum
 * size, the sequence is termed a bounded sequence. If no maximum size is
 * specified, size of the sequence is unspecified (unbounded).
 *
 * Prior to passing a bounded or unbounded sequence as a function argument (or
 * as a field in a structure or union), the length of the sequence must be set
 * in a language- mapping dependent manner. After receiving a sequence as an
 * input parameter, the length of the returned sequence will have been set;
 * this value may be obtained in a language-mapping dependent manner.
 *
 * A sequence type may be used as the type parameter for another sequence
 * type. For example, the following: declares an “unbounded sequence of
 * unbounded sequence of long”.
 *
 * ----
 * sequence< sequence<long> >
 * ----
 *
 * Note that for nested sequence declarations, white space must be used to
 * separate the two “>” tokens ending the declaration so they are not parsed as
 * a single “>>”
 */
sequence_type:
  SEQUENCE '<' simple_type_spec '>'
  {
    $$ = $3 ? type_newsequence(@1, NULL, $3, -1U) : NULL;
  }
  | SEQUENCE '<' simple_type_spec ',' positive_int_const '>'
  {
    assert($5.k == CST_UINT);
    $$ = $3 ? type_newsequence(@1, NULL, $3, $5.u) : NULL;
  }
;

/*/
 * === Optional
 *
 * Optional types represent a value that may or may not be present.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-optional-type.adoc
 * ****
 *
 * The parameter of the 'optional' declaration describes the data type of the
 * value.
 */
optional_type:
  OPTIONAL '<' simple_type_spec '>'
  {
    $$ = $3 ? type_newoptional(@1, NULL, $3) : NULL;
  }
;

/*/
 * === Strings
 *
 * A string is similar to a sequence of char.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-string-type.adoc
 * ****
 *
 * The argument to the string declaration is the maximum size of the string. If
 * a positive integer maximum size is specified, the string is termed a bounded
 * string; if no maximum size is specified, the string is termed an unbounded
 * string.  Strings are singled out as a separate type because many languages
 * have special built-in functions or standard library functions for string
 * manipulation. A separate string type may permit substantial optimization in
 * the handling of strings compared to what can be done with sequences of
 * general types.
 */
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

/*/
 * === Fixed type
 *
 * The ﬁxed data type represents a fixed-point decimal number
 *
 * ****
 * [horizontal]
 * <dotgen-rule-fixed-type.adoc
 * ****
 *
 * [CAUTION]
 * Fixed types are not properly handled in existing genom
 * templates. You must not use them at the moment.
 */

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
