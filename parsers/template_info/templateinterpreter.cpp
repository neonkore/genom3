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

#include "templateinterpreter.h"

#include <fstream>
#include <iostream>
#include <sstream>
// #include <cstdio>
#include <auto_ptr.h>

#include "utils/interpreter.h"
#include "utils/ast.h"
#include "parsers/template_info/lexer.h"
#include "parsers/template_info/parser.hpp"

using namespace G3nom;
using namespace std;

TemplateInterpreter::TemplateInterpreter()
: m_interpreter(0), m_component(0), m_verboseLexing(false), m_verboseParsing(false)
{}

bool TemplateInterpreter::parseInfoStream(std::istream& in, const std::string& sname)
{
	m_streamName = sname;

	m_lexer = new TemplateInfoLexer(&in);
	m_lexer->setDebug(m_verboseLexing);

	TemplateInfoParser parser(*this);
	parser.set_debug_level(m_verboseParsing);
	bool b = (parser.parse() == 0);

	delete m_lexer;
	m_lexer = 0;
	return b;
}

bool TemplateInterpreter::parseInfoFile(const std::string &filename)
{
	std::ifstream in(filename.c_str());
	if (!in.good())
		return false;
	return parseInfoStream(in, filename);
}

void TemplateInterpreter::error(const class location& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}

void TemplateInterpreter::error(const std::string& m)
{
	std::cerr << m << std::endl;
}

void TemplateInterpreter::setDebug(bool verbose)
{
	m_verboseLexing = true;
	m_verboseParsing = true;
}

string readFile(const std::string &inFile)
{
	ifstream in(inFile.c_str());
	if (!in.is_open()) {
		cerr << "Error opening file: " << inFile << endl;
		return string();
	}

	string str;
	stringstream ss(str);
	ss << in.rdbuf();
	return ss.str();
}

void TemplateInterpreter::executeFile(const std::string& infile)
{
	string s;
	if(infile.at(0) == '/')
		s = readFile(infile);
	else 
		s = readFile(m_source_dir + infile);
	m_interpreter->interpret(s);
}


void TemplateInterpreter::interpretFileInternal(const std::string &infile, const std::string &outfile)
{
	ofstream out(outfile.c_str());
	string s;
// 	auto_ptr<char> tmpFile(new char[L_tmpnam]);
// 	tmpnam(tmpFile.get());

	if (!out.is_open()) {
		cerr << "Error opening file for writing: " << outfile << endl;
		return;
	}
	
	if(!m_interpreter) {
		cerr << "No interpreter. Aborting " << endl;
		return;
	}

	// read whole file
	string str = readFile(infile);

	unsigned int idx = 0, pos = 0, pos2 = 0;
	while (pos != string::npos && pos < str.length()) {
		pos = str.find("<?", idx);
		pos2 = str.find("<!", idx);
		bool interpret = (pos <= pos2);
		if(!interpret)
			pos = pos2;

		s.append(m_interpreter->printString(str.substr(idx, pos - idx)));
// 		out << str.substr(idx, pos - idx);

		if (pos == string::npos)
			break;

		idx = pos + 2;

		if(interpret)
			pos = str.find("?>", idx);
		else
			pos = str.find("!>", idx);

		if(interpret)
			s.append(str.substr(idx, pos - idx));
		else
			s.append(m_interpreter->evalString(str.substr(idx, pos - idx)));

		idx = pos + 2;
	}
 
// 	cout << s << endl;
 	out << m_interpreter->interpret(s);
}

void TemplateInterpreter::interpretFile(const std::string &infile, std::string outfile)
{
	if(outfile.empty())
		outfile = infile;

	string o = outfile;
	uint idx = outfile.find("$$");
	if(idx != string::npos)
		o = o.replace(idx, 2, m_component->name());

	cout << "Interpreting file from " << m_source_dir << infile << " to " << m_out_dir << o << endl;
	interpretFileInternal(m_source_dir + infile, m_out_dir + o);
}

void TemplateInterpreter::interpretServiceFile(const std::string &infile, std::string outfile)
{
	if(!m_component)
		return;

	// find the $$ to be replaced with task name
	uint idx = outfile.find("$$");
	if(idx == string::npos)
		return;

	Service::Map::const_iterator it = m_component->servicesMap().begin();
	for(; it != m_component->servicesMap().end(); ++it) {
		if(m_interpreter)
			m_interpreter->exportVar("currentServiceName", it->first);

		string o = outfile.replace(idx, 2, it->first);
		interpretFile(infile, o);
	}
}

void TemplateInterpreter::interpretTaskFile(const std::string &infile, std::string outfile)
{
	if(!m_component)
		return;

	string o = outfile;
	uint idx2 = outfile.find("$component$");
	if(idx2 != string::npos)
		 o = o.replace(idx2, 11, m_component->name());

	// find the $$ to be replaced with task name
	uint idx = o.find("$task$");
	if(idx == string::npos)
		return;

	Task::Map::const_iterator it = m_component->tasksMap().begin();
	for(; it != m_component->tasksMap().end(); ++it) {
		if(m_interpreter)
			m_interpreter->exportVar("currentTaskName", it->first);

		interpretFile(infile, o.replace(idx, 6, it->first));
	}
}

void TemplateInterpreter::setInterpreter(G3nom::Interpreter* i)
{
	m_interpreter = i;
}

void TemplateInterpreter::setComponent(Component *c) 
{
	m_component = c;
	if(m_interpreter)
		m_interpreter->start(c);
}

// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;
