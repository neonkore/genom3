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

/*/ @node IDS declaration
 * @section IDS declaration
 * @cindex ids, declaration
 * @cindex declaration, ids
 *
 * @ruleinclude ids
 * @ruleinclude ids_name
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
