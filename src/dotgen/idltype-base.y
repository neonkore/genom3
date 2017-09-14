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
 * IDL Base types
 * --------------
 *
 * The base types of IDL are boolean values, binary values, signed or unsigned
 * integers, floating point and characters. The chapter on
 * link:../mappings/index{outfilesuffix}[GenoM IDL mappings] explains how they
 * map to the actual programming language.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-base-type-spec.adoc
 * ****
 */

base_type_spec:
  boolean_type | integer_type | floating_pt_type | char_type | octet_type |
  any_type;

/*/
 * === Integer types
 *
 * They represent integer values in the range indicated below.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-integer-type.adoc
 * <dotgen-rule-signed-int.adoc
 * <dotgen-rule-unsigned-int.adoc
 * <dotgen-rule-signed-longlong-int.adoc
 * <dotgen-rule-signed-long-int.adoc
 * <dotgen-rule-signed-short-int.adoc
 * <dotgen-rule-unsigned-longlong-int.adoc
 * <dotgen-rule-unsigned-long-int.adoc
 * <dotgen-rule-unsigned-short-int.adoc
 * ****
 *
 * .Range of integer types
 * [width="60%",cols=",2",frame="topbot"]
 * |========================================================================
 * |short              | -2^15^ .. 2^15^-1
 * |long               | -2^31^ .. 2^31^-1
 * |long long          | -2^63^ .. 2^63^-1
 * |unsigned short     | 0 .. 2^16^-1
 * |unsigned long      | 0 .. 2^32^-1
 * |unsigned long long | 0 .. 2^64^-1
 * |========================================================================
 */

integer_type: signed_int | unsigned_int;
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


/*/
 * === Floating point types
 *
 * They represent floating point values. The `float` type represents IEEE
 * single-precision floating point numbers; the `double` type represents
 * IEEE double-precision floating point numbers.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-floating-pt-type.adoc
 * <dotgen-rule-float-type.adoc
 * <dotgen-rule-double-type.adoc
 * ****
 */
floating_pt_type: float_type | double_type;

float_type: FLOAT
  { $$ = type_newbasic(@1, NULL, IDL_FLOAT); };
double_type: DOUBLE
  { $$ = type_newbasic(@1, NULL, IDL_DOUBLE); };


/*/
 * === Char types
 *
 * A char data type that is an 8-bit quantity that encodes a single-byte
 * character.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-char-type.adoc
 * ****
 */

char_type: CHAR
  { $$ = type_newbasic(@1, NULL, IDL_CHAR); };


/*/
 * === Boolean Types
 *
 * The boolean data type is used to denote a data item that can only take one
 * of the values TRUE and FALSE.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-boolean-type.adoc
 * ****
 */

boolean_type: BOOLEAN
  { $$ = type_newbasic(@1, NULL, IDL_BOOL); };


/*/
 * === Octet Types
 *
 * The octet type represents an 8 bit quantity.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-octet-type.adoc
 * ****
 */

octet_type: OCTET
  { $$ = type_newbasic(@1, NULL, IDL_OCTET); };


/*/
 * === Any Types
 *
 * [CAUTION]
 * The `any` type is a generic type that can contain any other type. This is
 * not yet implemented in genom, so you must not use it.
 *
 * ****
 * [horizontal]
 * <dotgen-rule-any-type.adoc
 * ****
 */

any_type: ANY
  { $$ = type_newbasic(@1, NULL, IDL_ANY); };
