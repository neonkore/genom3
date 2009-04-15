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
#include <Python.h>

#include "ast.h"

using namespace G3nom;
using namespace std;
using namespace boost::python;

PythonInterpreter* PythonInterpreter::m_instance = 0;

/********************** Python module definition ********/

Component* getCurrentComponent()
{
    PythonInterpreter *i = PythonInterpreter::getInstance();
    return i->component();
}

void debug(Component *c)
{
    c->debug();
}

BOOST_PYTHON_MODULE_INIT(G3nom)
{
    def("getComponent", &getCurrentComponent, return_value_policy<reference_existing_object>()/*, factory("Component")*/);
    def("debugComp", &debug);

    class_<Component>("Component")
      .def("task", &Component::task, return_value_policy<reference_existing_object>()/*, factory("Task")*/)
      .def("debug", &Component::debug);

    class_<Task>("Task")
      .def("debug", &Task::debug)
      .def_readwrite("priority", &Task::priority);
}

/********************** Python interpreter ********/

// allocates a new string using new [], and copies contents into it..
static char* newString( const char* contents )
{
  char* ret = new char[strlen( contents ) + 1];
  strcpy( ret, contents );
  return ret;
}

// See Kig source (part of KDE EDU module) for an example
// of how to use embedded python (or Kalzium for more advanced stuff)   
PythonInterpreter::PythonInterpreter() 
{
    char *s = newString("G3nom"); // we can't delete this string
    PyImport_AppendInittab(s, initG3nom);
    Py_Initialize();

    // create global dict object
    object main = import("__main__"); 
    m_pydict = main.attr("__dict__"); 
    // import our module
    interpret("import G3nom; from G3nom import *;");
}

PythonInterpreter* PythonInterpreter::getInstance()
{
    if(!m_instance)
	m_instance = new PythonInterpreter();
    return m_instance;
}

void PythonInterpreter::start(G3nom::Component* c) 
{
    m_component = c;
    interpret("comp = getComponent()");
}

Component* PythonInterpreter::component() 
{
    return m_component;
}

void PythonInterpreter::interpret(const std::string& s) 
{
    try {
	exec(str(s), m_pydict, m_pydict);
    } catch (error_already_set const &) {
	cerr << "Error in python interpreter: ";
	PyErr_Print();
	cerr << endl;
    }
}
