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

#define PTR_MAP_FORWARD_DECL(ptrBaseType) int foreach_##ptrBaseType##_map(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj* CONST objv[])
#define PTR_MAP_EXPORT(ptrBaseType) Tcl_CreateObjCommand(d->interp, "foreach_" #ptrBaseType "_map", foreach_##ptrBaseType##_map, (ClientData) NULL, NULL)

using namespace G3nom;
using namespace std;

int Genom_SafeInit(Tcl_Interp *);

PTR_MAP_FORWARD_DECL(Service);
PTR_MAP_FORWARD_DECL(Task);
PTR_MAP_FORWARD_DECL(Codel);
PTR_MAP_FORWARD_DECL(Port);
PTR_MAP_FORWARD_DECL(IdlType);
int foreach_ConstValue_map(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj* CONST objv[]);
int foreach_int_vect(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj* CONST objv[]);
int foreach_string_vect(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj* CONST objv[]);
int foreach_ServiceInput_vect(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj* CONST objv[]);
int foreach_IdlType_vect(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj* CONST objv[]);

TclInterpreter* TclInterpreter::m_instance = 0;

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

	PTR_MAP_EXPORT(Service);
	PTR_MAP_EXPORT(Task);
	PTR_MAP_EXPORT(Codel);
	PTR_MAP_EXPORT(Port);
	PTR_MAP_EXPORT(IdlType);
	Tcl_CreateObjCommand(d->interp, "foreach_ConstValue_map", foreach_ConstValue_map, (ClientData) NULL, NULL);
	Tcl_CreateObjCommand(d->interp, "foreach_int_vect", foreach_string_vect, (ClientData) NULL, NULL);
	Tcl_CreateObjCommand(d->interp, "foreach_string_vect", foreach_string_vect, (ClientData) NULL, NULL);
	Tcl_CreateObjCommand(d->interp, "foreach_ServiceInput_vect", foreach_ServiceInput_vect, (ClientData) NULL, NULL);
	Tcl_CreateObjCommand(d->interp, "foreach_IdlType_vect", foreach_IdlType_vect, (ClientData) NULL, NULL);

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
	interpret("set comp [getCurrentComponent]");

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

std::string treplaceAllOccurrences(std::string s, const std::string &pattern, const std::string &replaceWith)
{
	uint idx = 0;
	while((idx = s.find(pattern, idx)) != string::npos) {
		s = s.replace(idx, pattern.length(), replaceWith);
		idx = idx + replaceWith.length();
	}
	return s;
}

std::string TclInterpreter::printString(const std::string &s)
{
	string ss = treplaceAllOccurrences(s, "{", "\\{");
	ss = treplaceAllOccurrences(ss, "}", "\\}");
	ss = treplaceAllOccurrences(s, "\"", "\\\"");
	return "gputs \"" + ss + "\";\n";
}

std::string TclInterpreter::evalString(const std::string &s)
{
	return "gputs " + s  + ";\n";
}

void TclInterpreter::writeStdout(string text)
{
	d->outbuf.append(text);
}

// kate: indent-mode cstyle; replace-tabs off; tab-width 4; 
