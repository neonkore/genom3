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

/*/ @node Port declaration
 * @section Port declaration
 * @cindex port, declaration
 * @cindex declaration, port
 *
 * @ruleinclude port
 * @ruleinclude opt_multiple
 * @ruleinclude port_dir
 *
 * Ports implement the data flow between components as a publish/subscribe
 * model. Ports have a name and a type and can be either @code{out} (for
 * publishing data) or @code{in} (for subscribing to a sibling @code{out}
 * port).
 *
 * The optional @code{multiple} qualifier defines a dynamic list of ports of
 * the given type, indexed by strings. In this case, ports are created or
 * destroyed dynamically be the codels.
 */
port:
  PORT opt_multiple port_dir type_spec identifier semicolon
  {
    if (!$5) { parserror(@1, "dropped port"); break; }
    if (!$4) { parserror(@1, "dropped '%s' port", $5); break; }
    if (!port_create(@1, $3, $2, $5, $4))
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
