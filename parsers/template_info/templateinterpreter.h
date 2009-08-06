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

#ifndef G3NOM_TEMPLATE_INTERPRETER_H
#define G3NOM_TEMPLATE_INTERPRETER_H

#include <string>
#include <utils/interpreter.h>

namespace G3nom
{

class Interpreter;
class Component;
class TemplateInfoLexer;

/** \short Template file interpreter

* This class parses a template.info file and interprets the files listed in it.
* It replaces text within '<!' and '!>' tags with the result of Interpreter::evalString
* and the text between a closing tag and the next opening tag with the result
* of Interpreter::printString. The code between '<?' and '?>' tags is kept.

* The result of these transformations is sent to the interpreter using Interpreter::interpret()
* and the result is written to the output file.
*/
class TemplateInterpreter
{
	public:
		enum State { Other, InsideMain, InsideTask, InsideService, InsideLanguage, InsideRequires };

		TemplateInterpreter(bool parseOnly = false);

		bool parseInfoFile(const std::string &filename);

		void setPrintGeneratedFile(bool b) { m_printGeneratedFile = b; }
		void setDebug(bool verbose);
		/** Error handling with associated line number. This can be modified to
		 * output the error e.g. to a dialog box. */
		void error(const class location& l, const std::string& m);
		/** General error handling. This can be modified to output the error
		 * e.g. to a dialog box. */
		void error(const std::string& m);
		std::string* streamNamePtr() {
			return &m_streamName;
		}
		TemplateInfoLexer* lexer() {
			return m_lexer;
		}

		void setInterpreter(Interpreter *i);
		void setLanguage(const std::string &lang);
		void setComponent(Component *c);

		void setState(State s) {
			m_state = s;
		}
		State state() {
			return m_state;
		}

		void setTemplateName(std::string name) {
			m_templateName = name;
		}
		std::string templateName() const { return m_templateName; }
		void setTemplateDoc(std::string doc) {
			m_templateDoc = doc;
		}
		std::string templateDoc() const { return m_templateDoc; }

		void setSourceDirectory(const std::string &dir);
		void setOutputDirectory(const std::string &dir);

		void interpretTaskFile(const std::string& infile, std::string outfile);
		void interpretServiceFile(const std::string& infile, std::string outfile);
		void interpretFile(const std::string& infile, std::string outfile = std::string());

		/* Pass the file to the interpreter before parsing any other file*/
		void executeFile(const std::string& infile);

	protected:
		bool parseInfoStream(std::istream& in, const std::string& sname);

		void interpretFileInternal(const std::string &infile, const std::string &outfile);

		std::string tmpFile(std::string outfile, std::string suffix = std::string());
		bool needsMerge(std::string outfile);

	private:
		Interpreter *m_interpreter;
		std::string m_language;
		Component *m_component;
		std::string m_out_dir, m_source_dir;
		std::string m_templateName, m_templateDoc;

		/// memebers related to parsing info files
		TemplateInfoLexer* m_lexer;
		/// stream name (file or input stream) used for error messages.
		std::string m_streamName;
		/// enable debug output in the flex scanner
		bool m_verboseLexing;
		/// enable debug output in the bison parser
		bool m_verboseParsing;
		bool m_printGeneratedFile;
		bool m_parseOnly;
		State m_state;
};

}

#endif
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;  replace-tabs off;
