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
#include "cpptcl.h"

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

string tasksList(Component *c)
{
	string out;

	Task::Map map = c->tasksMap();
	Task::Map::const_iterator it;
	for (it = map.begin(); it != map.end(); ++it) {
		out.append(" \"").append(it->first).append("\"");
		out.append(" \"").append(it->first).append("\"");
	}

	return out;
}

// Tcl::object tasksList(interpreter &i, Component *c)
// {
//     object tab;
//
//     Task::Map map = c->tasksMap();
//     Task::Map::const_iterator it;
//     for(it = map.begin(); it != map.end(); ++it)
// 	tab.append(i, object(it->first));
//
//     return tab;
// }

namespace G3nom
{
class TclInterpreterPrivate
{
	public:
		Tcl::interpreter interpreter;
};
}

TclInterpreter::TclInterpreter()
		: d(new TclInterpreterPrivate())
{
	d->interpreter.def("getComponent", &getCurrentComponent, factory("Component"));
	d->interpreter.def("debugComp", &debug);
	d->interpreter.def("tasksList", &tasksList);

	d->interpreter.class_<Component>("Component")
	.def("task", &Component::task, factory("Task"))
	.def("debug", &Component::debug);

	d->interpreter.class_<Task>("Task")
	.def("debug", &Task::debug);
}

TclInterpreter::~TclInterpreter()
{}

TclInterpreter* TclInterpreter::getInstance()
{
	if (!m_instance)
		m_instance = new TclInterpreter();
	return m_instance;
}

void TclInterpreter::start(G3nom::Component* c)
{
	m_component = c;
	interpret("set comp [getComponent]");

	// create list of tasks
//     object o("set taskList");
//     o.append(d->interpreter, tasksList(d->interpreter, c));
//     d->interpreter.eval(o);
}

std::string TclInterpreter::interpret(const std::string& s)
{
	try {
		d->interpreter.eval(s);
		return "";
	} catch (std::exception const &e) {
		cerr << "Error: " << e.what() << endl;
	}
}

std::string TclInterpreter::eval(const std::string& s)
{
	try {
		d->interpreter.eval(s);
		return "";
	} catch (std::exception const &e) {
		cerr << "Error: " << e.what() << endl;
	}
}

void TclInterpreter::exportVar(const std::string &name, const std::string &value)
{
	interpret("set " + name + " " + value);
}


// kate: indent-mode cstyle; replace-tabs off; tab-width 4; 
