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

#include <iostream>
#include <tcl.h>

#include "utils/ast.h"

using namespace G3nom;
using namespace std;

int Genom_SafeInit(Tcl_Interp *);

TclInterpreter* TclInterpreter::m_instance = 0;

Component* getCurrentComponent()
{
	TclInterpreter *i = TclInterpreter::getInstance();
	return i->component();
}

void tclWriteStdout(const char *text)
{
	TclInterpreter::getInstance()->writeStdout(text);
}

// void debug(Component *c)
// {
// 	c->debug();
// }

// string tasksList(Component *c)
// {
// 	string out;
// 
// 	Task::Map map = c->tasksMap();
// 	Task::Map::const_iterator it;
// 	for (it = map.begin(); it != map.end(); ++it) {
// 		out.append(" \"").append(it->first).append("\"");
// 		out.append(" \"").append(it->first).append("\"");
// 	}
// 
// 	return out;
// }

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
		Tcl_Interp *interp;
		std::string outbuf; // buffer stdout is redirected to
};
}

TclInterpreter::TclInterpreter()
		: d(new TclInterpreterPrivate())
{
	d->interp = Tcl_CreateInterp();

	if (TCL_OK != Tcl_Init(d->interp))
		cout << "Tcl_Init error: " << Tcl_GetStringResult(d->interp) << endl;
	if(TCL_OK != Genom_SafeInit(d->interp))
		cout << "Genom_Init error: " << Tcl_GetStringResult(d->interp) << endl;
}

TclInterpreter::~TclInterpreter()
{
	Tcl_DeleteInterp(d->interp);
}

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
	d->outbuf = "";
	int cc = Tcl_Eval(d->interp, s.c_str());
	if (cc != TCL_OK) {
		cout << "Tcl Interpreter error:  " << Tcl_GetStringResult(d->interp) << endl;
		return "";
	}

	return d->outbuf;
}

std::string TclInterpreter::eval(const std::string& s)
{
  	int cc = Tcl_Eval(d->interp, s.c_str());
	if (cc != TCL_OK) {
		cout << "Tcl Interpreter error:  " << Tcl_GetStringResult(d->interp) << endl;
		return "";
	}

	Tcl_Obj *obj = Tcl_GetObjResult(d->interp);
	return Tcl_GetString(obj);
}

void TclInterpreter::exportVar(const std::string &name, const std::string &value)
{
	interpret("set " + name + " " + value);
}

std::string TclInterpreter::printString(const std::string &s)
{
	return s;
}

std::string TclInterpreter::evalString(const std::string &s)
{
	return "puts " + s  + ";";
}

void TclInterpreter::writeStdout(string text)
{
	d->outbuf.append(text);
}

// kate: indent-mode cstyle; replace-tabs off; tab-width 4; 
