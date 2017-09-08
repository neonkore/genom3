/*
 * Copyright (c) 2009-2012,2014,2017 LAAS/CNRS
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
 * Port declaration
 * ----------------
 *
 * Ports implement the data flow between components as a publish/subscribe
 * model. Ports have a name and a type. They can be either `out` ports (for
 * publishing data) or `in` ports (for subscribing to a sibling `out` port).
 *
 * <dotgen-rule-port.adoc
 * <dotgen-rule-opt-multiple.adoc
 * <dotgen-rule-port-dir.adoc
 *
 * Ports are by default `single` ports, i.e. they define one type of exported
 * data. The optional `multiple` qualifier can be used to define a 'dynamic
 * list' of ports of the given type, instead of a single slot. Suchs ports are
 * accessed as an array indexed by strings. In this case, ports are created or
 * destroyed dynamically be the codels. The
 * link:../mappings/index{outfilesuffix}[IDL mappings] section for ports
 * explains how this is done.
 */
port:
  PORT opt_multiple port_dir type_spec identifier opt_properties semicolon
  {
    if (!$5 || !$4) {
      if (!$5) parserror(@1, "dropped port");
      if (!$4) parserror(@1, "dropped '%s' port", $5);
      if ($6) hash_destroy($6, 1);
      break;
    }
    if (!port_create(@1, $3, $2, $5, $4, $6))
      parserror(@1, "dropped '%s' port", $5);
  }
;

port_dir:
    IN	{ $$ = PORT_IN; }
  | OUT	{ $$ = PORT_OUT; }
;

opt_multiple:
  /* empty */	{ $$ = PORT_SIMPLE; }
  | MULTIPLE	{ $$ = PORT_MULTIPLE; }
;
