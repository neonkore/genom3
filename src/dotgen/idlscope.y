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

/* --- IDL modules --------------------------------------------------------- */

/*/ @node Module declaration
 * @section Module declaration
 * @cindex module, declaration
 *
 * A module definition satisfies the following syntax:
 *
 * @ruleinclude module
 * @ruleinclude module_name
 * @ruleinclude module_body
 * @ruleinclude idl_statements
 *
 * The only effect of a module is to scope @acronym{IDL} identifiers. It is
 * similar to a @acronym{C++} or Java namespace; it is considered good practice
 * to enclose your type definitions inside a module definition to prevent name
 * clashes between components.
 */
module:
  MODULE module_name '{' module_body '}' semicolon
  {
    scope_s s = scope_pop();

    assert(s == $2);
    if (scope_name(s)[0] == '&') {
      /* there was an error during the creation of the scope. */
      parserror(@1, "dropped declaration for '%s'", &scope_name(s)[1]);
      scope_destroy(s);
    }
  }
;

module_name: identifier
  {
    $$ = scope_push(@1, $1, SCOPE_MODULE);
    if (!$$) {
      /* on error, still create a scope to continue the parsing
       * but with a special name -- it will be deleted afterwards */
      $$ = scope_push(@1, strings("&", $1, NULL), SCOPE_MODULE);
      if (!$$) { /* still failed, just resign */ YYABORT;
      }
    }
  }
;

module_body: /* empty */ { $$ = 0; } | idl_statements;

/* --- scopes -------------------------------------------------------------- */

/* scopes are created as a side effect of certain declarations (modules,
 * components, interfaces, ...) or types (structures, unions, ...) */

scope_push_struct: identifier
  {
    $$ = scope_push(@1, $1, SCOPE_STRUCT);
    if (!$$) {
      /* on error, still create a scope to continue the parsing
       * but with a special name -- it will be deleted afterwards */
      $$ = scope_push(@1, strings("&", $1, NULL), SCOPE_STRUCT);
      if (!$$) { /* still failed, just resign */ YYABORT;
      }
    }
  }
;

scope_push_union: identifier
  {
    $$ = scope_push(@1, $1, SCOPE_UNION);
    if (!$$) {
      /* on error, still create a scope to continue the parsing
       * but with a special name -- it will be deleted afterwards */
      $$ = scope_push(@1, strings("&", $1, NULL), SCOPE_UNION);
      if (!$$) { /* still failed, just resign */ YYABORT; }
    }
  }
;

scoped_name:
  identifier
  | COLONCOLON identifier
  {
    $$ = strings("::", $2, NULL);
  }
  | scoped_name COLONCOLON identifier
  {
    $$ = strings($1, "::", $3, NULL);
  }
;
