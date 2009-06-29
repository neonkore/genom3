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

#include "pythoninterpreter.h"

#include <iostream>
#include <sstream>
#include <Python.h>
#include <boost/python.hpp>

#include "utils/ast.h"
#include "utils/idltype.h"

using namespace G3nom;
using namespace Idl;
using namespace std;
using namespace boost::python;

PythonInterpreter* PythonInterpreter::m_instance = 0;


void writeStdout(const char *text)
{
	PythonInterpreter::getInstance()->writeStdout(text);
}

// The Logger modules is used to redirect stdout to 
// a string. The Logger.write function replaces builtin
// sys.stdout.write.
BOOST_PYTHON_MODULE_INIT(Logger)
{
  def("write", &writeStdout);
}

/********************** Python module definition ********/

Component* pygetCurrentComponent()
{
	PythonInterpreter *i = PythonInterpreter::getInstance();
	return i->component();
}


void export_idl();
void export_ast();
void export_containers();

BOOST_PYTHON_MODULE_INIT(G3nom)
{
	def("getComponent", &pygetCurrentComponent, return_value_policy<reference_existing_object>());

	export_ast();
	export_idl();
	export_containers();
}

/********************** Python interpreter ********/

// allocates a new string using new [], and copies contents into it..
static char* newString(const char* contents)
{
	char* ret = new char[strlen(contents) + 1];
	strcpy(ret, contents);
	return ret;
}

namespace G3nom
{
class PythonInterpreterPrivate
{
	public:
		boost::python::object pydict; // current dict of defined objects
		std::string outbuf; // buffer stdout is redirected to
};
}

// See Kig source (part of KDE EDU module) for an example
// of how to use embedded python (or Kalzium for more advanced stuff)
PythonInterpreter::PythonInterpreter()
		: d(new PythonInterpreterPrivate())
{
	char *s = newString("G3nom"); // we can't delete this string
	char *ss = newString("Logger"); // we can't delete this string

	// import G3nom and Logger modules init function
	PyImport_AppendInittab(s, initG3nom);
	PyImport_AppendInittab(ss, initLogger);
	// initialize the interpreter
	Py_Initialize();

	// create global dict object
	object main = import("__main__");
	d->pydict = main.attr("__dict__");
	// import our module
	interpret("import G3nom;\nfrom G3nom import *;\n");
	interpret("import Logger;\nfrom Logger import *");
	interpret("import sys\nsys.stdout = Logger");
}

PythonInterpreter::~PythonInterpreter()
{}

PythonInterpreter* PythonInterpreter::getInstance()
{
	if (!m_instance)
		m_instance = new PythonInterpreter();
	return m_instance;
}

void PythonInterpreter::start(G3nom::Component* c)
{
	m_component = c;
	interpret("comp = getComponent()\n");
}

std::string PythonInterpreter::interpret(const std::string& s)
{
	try {
		d->outbuf.erase();
		exec(str(s), d->pydict, d->pydict);
// 		return d->outbuf.substr(0, d->outbuf.length() - 1);
		return d->outbuf;
	} catch (error_already_set const &) {
		cerr << "Error in python interpreter: ";
		PyErr_Print();
		cerr << endl;
		exit(1);
	}
	return "";
}

std::string PythonInterpreter::eval(const std::string& s)
{
	return interpret("print " + s);
}

void PythonInterpreter::exportVar(const std::string &name, const std::string &value)
{
	interpret(name + " = \"" + value + "\"");
}

void PythonInterpreter::writeStdout(string text)
{
	d->outbuf.append(text);
}

std::string PythonInterpreter::evalString(const std::string &s)
{
	return "sys.stdout.write(str(" + s + "));";
}

/** Replaces all occurrences of \a pattern in the string \a s with \a replaceWith
*/
std::string replaceAllOccurrences(std::string s, const std::string &pattern, const std::string &replaceWith)
{
	uint idx = 0;
	while((idx = s.find(pattern, idx)) != string::npos) {
		s = s.replace(idx, pattern.length(), replaceWith);
		idx = idx + 3;
	}
	return s;
}

std::string PythonInterpreter::printString(const std::string &s)
{
	return "sys.stdout.write('''" + replaceAllOccurrences(s, "\\", "\\\\") + "'''); ";
// 	string res = "print \"";
// 	stringstream ss(s);

// 	while(!ss.eof()) {
// 		char buf[5000];
// 		ss.getline(buf, 5000);
// 		res.append(string(buf));
// 		if(!ss.eof())
// 			 res.append("\\n\\\n");
// 	}
// 	res.append("\"\n");
// 	return res;
}


// kate: indent-mode cstyle; replace-tabs off; tab-width 4; 
