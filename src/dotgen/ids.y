/*
 * Copyright (c) 2009-2012,2014 LAAS/CNRS
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
 * IDS declaration
 * ---------------
 *
 * The `ids` declaration declares a private, internal data structure.
 *
 * The internal data structure stores global data shared by the services.
 * Access to this structure is controlled by Genom, that takes care of the
 * proper synchronization between concurrent access.
 *
 * An IDS definition is similar to a `struct` definition, and follows this
 * syntax:
 *
 * <dotgen-rule-ids.adoc
 * <dotgen-rule-ids-name.adoc
 * <dotgen-rule-member-list.adoc
 * <dotgen-rule-member.adoc
 *
 * The major difference with a regular `struct` is that an `ids` declaration
 * can be reopened. Thus, several `ids` statements can be used to define an
 * internal structure by independent chunks. This is useful for instance in
 * case of included files.
 *
 * === Example
 *
 * This would define the `ids` for the component `foo`, using two separate
 * declarations:
 *
 * [source,C]
 * ----
 * component foo {
 *   ids {
 *     long state;
 *   };
 *
 *   ids {
 *     double position;
 *     double velocity;
 *   };
 * };
 * ----
 */
ids:
  ids_name '{' member_list '}' semicolon
  {
    scope_s s = scope_pop(); assert(s == $1);
    if (!comp_addids(@1, s)) parserror(@1, "dropped ids declaration");
  }
  | ids_name error semicolon
  {
    scope_s s = scope_pop(); assert(s == $1);
    parserror(@1, "dropped ids declaration");
  }
;

ids_name: IDS
  {
    $$ = comp_idsscope(comp_active());
    scope_set($$);
  };
