/* 
 * Copyright (c) 2009 LAAS/CNRS                      
 * All rights reserved.
 *
 * Redistribution and use  in source  and binary  forms,  with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   1. Redistributions of  source  code must retain the  above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice,  this list of  conditions and the following disclaimer in
 *      the  documentation  and/or  other   materials provided  with  the
 *      distribution.
 *
 * THIS  SOFTWARE IS PROVIDED BY  THE  COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND  ANY  EXPRESS OR IMPLIED  WARRANTIES,  INCLUDING,  BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES  OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR  PURPOSE ARE DISCLAIMED. IN  NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR      CONTRIBUTORS  BE LIABLE FOR   ANY    DIRECT, INDIRECT,
 * INCIDENTAL,  SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE   OF THIS SOFTWARE, EVEN   IF ADVISED OF   THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#include <string>
#include <iostream>

#include "pythoninterpreter.h"
#include "driver.h"

using namespace std;
using namespace G3nom;

int main(int argc, char* argv[])
{
      string s = "task main { "
		  "priority: 100;"
		  "period: 5;\n"
		  "stack:  100;"
// 		  "codel start:	tstart(inout param);\n"
// 		  "codel main:	tmain();\n"
		  "};";

      Driver d;
//      d.setDebug(true);
      d.parseString(s);
//       d.component().debug();

      PythonInterpreter *i = PythonInterpreter::getInstance();
      i->start(&(d.component()));
//       i->interpret("set comp2 [Component];");
//       i->interpret("debugComp $comp");
//       i->interpret("debugComp $comp2");
//       i->interpret("$comp debug");
// 	  i->interpret("print 3");
      i->interpret("task = comp.task(\"main\")");
      i->interpret("task.debug()");
}
