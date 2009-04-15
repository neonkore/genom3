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

#include "tclinterpreter.h"
#include "ast.h"

#include <iostream>

using namespace G3nom;
using namespace Tcl;
using namespace std;

TclInterpreter* TclInterpreter::m_instance = 0;

Component* getCurrentComponent()
{
    TclInterpreter *i = TclInterpreter::getInstance();
    return i->component();
}

void debug(Component *c)
{
    c->debug();
}

TclInterpreter::TclInterpreter() 
{
    m_interpreter.def("getComponent", &getCurrentComponent, factory("Component"));
    m_interpreter.def("debugComp", &debug);

    m_interpreter.class_<Component>("Component")
	    .def("task", &Component::task, factory("Task"))
	    .def("debug", &Component::debug);

    m_interpreter.class_<Task>("Task")
           .def("debug", &Task::debug);
}

TclInterpreter* TclInterpreter::getInstance()
{
    if(!m_instance)
	m_instance = new TclInterpreter();
    return m_instance;
}

void TclInterpreter::start(G3nom::Component* c) 
{
    m_component = c;
    m_interpreter.eval("set comp [getComponent]");
}

void G3nom::TclInterpreter::interpret(const std::string& s) 
{
    try {
	m_interpreter.eval(s);
    } catch (std::exception const &e) {
	cerr << "Error: " << e.what() << endl;
    }
}
