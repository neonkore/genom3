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
 * Component declaration
 * ---------------------
 *
 * A component declaration describes a instance of the `genom` component
 * model. It is defined by a unique name (an identifier) that also defines an
 * IDL scope for any embedded types.
 *
 * Components export objects from the `genom` component model, namely:
 * link:ids{outfilesuffix}[IDS], link:task{outfilesuffix}[tasks],
 * link:port{outfilesuffix}[ports], link:attribute{outfilesuffix}[attributes] or
 * link:service[services].
 *
 * Components may also define new types via IDL statements. These types will
 * be defined within the component scope.
 *
 * <dotgen-rule-component.adoc
 * <dotgen-rule-component-name.adoc
 * <dotgen-rule-component-body.adoc
 *
 * <dotgen-rule-exports.adoc
 * <dotgen-rule-export.adoc
 *
 * <dotgen-rule-component-property.adoc
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
 *
 * +doc+::
 * A string that describes the functionality of the component.
 *
 * +version+::
 * The component version number, as a string
 *
 * +lang+::
 * The programming language of the codels interface.
 *
 * +email+::
 * A string containing the e-mail address of the author of the component.
 *
 * +requires+::
 * A list of dependencies of the component (see
 * link:pragma{outfilesuffix}#pragma_requires[#pragma requires]). Each
 * string should contain a package name in `pkg-config` format.
 *
 * +codels-requires+::
 * A list of dependencies of the codels. Each string should contain a package
 * name in `pkg-config` format.
 *
 * +clock-rate+::
 * The period of the internal component clock. It is usually not necessary to
 * define it explicitly. If the component defines periodic task, the component
 * clock period will be automatically computed as the greatest common divisor
 * of the period of all periodic tasks.
 *
 * +provides+::
 * A list of interfaces (see link:interface{outfilesuffix}[Interface
 * declaration]) that the component implements. All objects from the interface
 * are imported as-is into the component description. Ports and services may be
 * further refined once imported, typically by defining codels (see
 * link:codel{outfilesuffix}[Codel declaration]) that implement the services.
 *
 * +uses+::
 * A list of interfaces (see link:interface{outfilesuffix}[Interface
 * declaration]) that the component uses. Ports are imported in the opposite
 * direction (e.g. a `port out` is imported as a `port in`). Services are
 * imported as `remote` objects that can be accessed via codel parameters
 * (see link:codel{outfilesuffix}[Codel declaration]). Other objects are
 * imported as-is.
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
  THROWS throw_list semicolon
  {
    $$ = $2 ? prop_newhash(@1, PROP_THROWS, $2) : NULL;
  }
;

throw_list:
  named_type
  {
    if ($1 && type_kind($1) != IDL_EXCEPTION) {
      parserror(@1, "%s%s%s is not an exception",
                type_strkind(type_kind($1)),
                type_name($1)?" ":"", type_name($1)?type_name($1):"");
      parsenoerror(type_loc($1), " %s%s%s declared here",
                   type_strkind(type_kind($1)),
                   type_name($1)?" ":"", type_name($1)?type_name($1):"");
      $1 = NULL;
    }
    $$ = hash_create("exception list", 2); if (!$$ || !$1) break;
    switch(hash_insert($$, type_fullname($1), $1, NULL)) {
      case EEXIST:
        parserror(@1, "duplicate exception '%s'", type_fullname($1)); break;
    }
  }
  | throw_list ',' named_type
  {
    $$ = $1; if (!$3) break;
    if ($3 && type_kind($3) != IDL_EXCEPTION) {
      parserror(@3, "%s%s%s is not an exception",
                type_strkind(type_kind($3)),
                type_name($3)?" ":"", type_name($3)?type_name($3):"");
      parsenoerror(type_loc($3), " %s%s%s declared here",
                   type_strkind(type_kind($3)),
                   type_name($3)?" ":"", type_name($3)?type_name($3):"");
      break;
    }
    switch(hash_insert($$, type_fullname($3), $3, NULL)) {
      case EEXIST:
        parserror(@3, "duplicate exception '%s'", type_fullname($3)); break;
    }
  }
;
