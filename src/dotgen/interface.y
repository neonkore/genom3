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

/*/ @node Interface declaration
 * @section Interface declaration
 * @cindex interface, declaration
 * @cindex declaration, interface
 *
 * @ruleinclude interface
 * @ruleinclude interface_scope
 * @sp 1
 * @ruleinclude interface_property
 *
 * An interface declaration is basically the same as a component declaration
 * (@pxref{Component declaration}) but is meant to be shared between several
 * components. Although any object can be defined in an interface, it will
 * typically only declare service prototypes and ports that are to be
 * @code{provide}d or @code{use}d by components.
 */
interface: INTERFACE interface_scope component_body semicolon
  {
    comp_s c = comp_pop();
    if (c) assert(c == $2);
  }
;

interface_scope: identifier
  {
    if (!$1) { parserror(@1, "dropped interface"); YYABORT; }
    $$ = comp_push(@1, $1, COMP_IFACE);
    if (!$$) {
      parserror(@1, "dropped interface '%s'", $1);
      YYABORT;
    }
  }
;

interface_name: identifier
  {
    if (!$1) { $$ = NULL; break; }
    $$ = comp_find($1);
    if (!$$) { parserror(@1, "no such interface '%s'", $1); break; }
    if (comp_kind($$) != COMP_IFACE) {
      parserror(@1, "expected interface but '%s' is a %s",
                $1, comp_strkind(comp_kind($$)));
      parsenoerror(comp_loc($$), " %s declared here", $1);
      $$ = NULL;
      break;
    }
  }
;

/*/
 * In addition to regular component properties, an interface can also define
 * the following properties:
 * @table @code
 * @item extends
 * A list of interfaces that are imported as-is into the current one. All
 * objects from the extended interfaces appear as if they had been defined in
 * the extending interface.
 * @end table
 */
interface_property:
  EXTENDS interface_list semicolon
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newhash(@1, PROP_EXTENDS, $2);
  }

interface_list:
  interface_name
  {
    $$ = hash_create("interface list", 1); if (!$$ || !$1) break;
    switch(hash_insert($$, comp_name($1), $1, NULL)) {
      case 0:	break;
      default:	YYABORT;
    }
  }
  | interface_list ',' interface_name
  {
    $$ = $1; if (!$3) break;
    switch(hash_insert($$, comp_name($3), $3, NULL)) {
      case 0: break;
      case EEXIST:
        parserror(@3, "repeated interface '%s'", comp_name($3));
        break;
      default: YYABORT;
    }
  }
;
