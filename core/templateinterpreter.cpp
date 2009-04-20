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
#include <cstdio>
#include <auto_ptr.h>

#include "utils/interpreter.h"

using namespace G3nom;
using namespace std;

TemplateInterpreter::TemplateInterpreter()
{
}

string readFile(const char *inFile)
{
	ifstream in(inFile);
	if (!in.is_open()) {
		cerr << "Error opening file: " << inFile << endl;
		return string();
	}

	string str;
	stringstream ss(str);
	ss << in.rdbuf();
	return ss.str();
}

void TemplateInterpreter::parseFile(const char *infile, const char *outfile)
{
	ofstream out(outfile);
	auto_ptr<char> tmpFile(new char[L_tmpnam]);
	tmpnam(tmpFile.get());

	if (!out.is_open()) {
		cerr << "Error opening file for writing: " << outfile << endl;
		return;
	}

	// read whole file
	string str = readFile(infile);

	unsigned int idx = 0, pos = 0;
	while (pos != string::npos && pos < str.length()) {
		pos = str.find("<?", idx);
		out << str.substr(idx, pos - idx);

		if (pos == string::npos) {
//  	  out << str.substr(idx, str.length());
			break;
		}
		idx = pos + 2;

		pos = str.find("?>", idx);
		if (m_interpreter) {
			//redirect stdout to file
			freopen(tmpFile.get(), "w", stdout);
			//launch interpreter
			m_interpreter->interpret(str.substr(idx, pos - idx));
			// revert stdout
			freopen("CON", "w", stdout);

			// read file and output it
			string s = readFile(tmpFile.get());
			cout << "res: " << s << endl;
			out << s;
		}
//       out << str.substr(idx, pos-idx);
		idx = pos + 2;
	}
}

void TemplateInterpreter::setInterpreter(G3nom::Interpreter* i)
{
	m_interpreter = i;
}

// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;
