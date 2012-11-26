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

/*/ @node Component declaration
 * @section Component declaration
 * @cindex component, declaration
 * @cindex declaration, component
 *
 * @ruleinclude component
 * @ruleinclude component_name
 * @ruleinclude component_body
 * @sp 1
 * @ruleinclude exports
 * @ruleinclude export
 * @sp 1
 * @ruleinclude component_property
 *
 * A component declaration describes a instance of the @genom{} component
 * model. It is defined by a unique name (an identifier) that also defines an
 * IDL scope for any embedded types.
 *
 * Components export objects from the @genom{} component model, namely: IDS
 * (@pxref{IDS declaration}), tasks (@pxref{Task declaration}), ports
 * (@pxref{Port declaration}), attributes (@pxref{Attribute declaration}) or
 * services (@pxref{Service declaration}).
 *
 * Components may also define new types @emph{via} IDL statements. These types
 * will be defined within the component scope.
 *
 */
component: COMPONENT component_name component_body semicolon
  {
    comp_s c = comp_pop();
    if (c) assert(c == $2);
  }
;

component_name: identifier
  {
    if (!$1) { parserror(@1, "dropped component"); YYABORT; }
    $$ = comp_push(@1, $1, COMP_REGULAR);
    if (!$$) {
      parserror(@1, "dropped component '%s'", $1);
      YYABORT;
    }
  }
;

component_body: /* empty */ | '{' exports '}';

exports:
  /* empty */		{ $$ = 0; }
  | exports export	{ assert(!task_p); }
;

export:
    idl_statement
  | property
  {
    $$ = 0; if (!$1) break;
    if (comp_addprop(@1, $1)) {
      parsenoerror(@1, "dropped %s declaration", prop_name($1));
      prop_destroy($1);
    }
  }
  | ids
  | task
  | port
  | attribute
  | service
  | error ';' { $$ = 0; }
;

/*/
 * A number of properties can be attached to a component:
 * @table @code
 * @item doc
 * A string that describes the functionality of the component.
 *
 * @item version
 * The component version number, as a string
 *
 * @item lang
 * The programming language of the codels interface.
 *
 * @item email
 * A string containing the e-mail address of the author of the component.
 *
 * @item requires
 * A list of dependencies of the component (@pxref{#pragma requires}). Each
 * string should contain a package name in @code{pkg-config} format.
 *
 * @item codels-requires
 * A list of dependencies of the codels. Each string should contain a package
 * name in @code{pkg-config} format.
 *
 * @item clock-rate
 * The period of the internal component clock. It is usually not necessary to
 * define it explicitly. If the component defines periodic task, the component
 * clock period will be automatically computed as the greatest common divisor
 * of the period of all periodic tasks.
 *
 * @item provides
 * A list of interfaces (@pxref{Interface declaration}) that the component
 * implements. All objects from the interface are imported as-is into
 * the component description. Ports and services may be further refined
 * once imported, typically by defining codels (@pxref{Codel declaration}) that
 * implement the services.
 *
 * @item uses
 * A list of interfaces (@pxref{Interface declaration}) that the component
 * uses. Ports are imported in the opposite direction (e.g. a @code{port out}
 * is imported as a @code{port in}. Services are imported as @code{remote}
 * objects that can be accessed @emph{via} codel parameters (@pxref{Codel
 * declaration}). Other objects are imported as-is.
 * @end table
 */
component_property:
  DOC string_literals semicolon
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newstring(@1, PROP_DOC, $2);
  }
  | VERSION string_literals semicolon
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newstring(@1, PROP_VERSION, $2);
  }
  | LANG string_literals semicolon
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newstring(@1, PROP_LANG, $2);
  }
  | EMAIL string_literals semicolon
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newstring(@1, PROP_EMAIL, $2);
  }
  | REQUIRE string_list semicolon
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newrequire(@1, PROP_REQUIRE, $2);
  }
  | CODELSREQUIRE string_list semicolon
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newrequire(@1, PROP_CODELS_REQUIRE, $2);
  }
  | CLOCKRATE const_expr time_unit semicolon
  {
    if (const_binaryop(&$2, '*', $3)) {
      parserror(@3, "invalid numeric constant");
      break;
    }
    $$ = prop_newvalue(@1, PROP_CLOCKRATE, $2);
  }
  | PROVIDES interface_list semicolon
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newhash(@1, PROP_PROVIDES, $2);
  }
  | USES interface_list semicolon
  {
    if (!$2) { parserror(@1, "dropped '%s' property", $1); $$ = NULL; break; }
    $$ = prop_newhash(@1, PROP_USES, $2);
  }
;

throw_property:
  THROWS identifier_list semicolon
  {
    $$ = $2 ? prop_newhash(@1, PROP_THROWS, $2) : NULL;
  }
;
