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

namespace G3nom
{

class Interpreter;
class Component;
class TemplateInfoLexer;

class TemplateInterpreter
{
	public:
		enum State { Other, InsideMain, InsideTask, InsideService, InsideLanguage };

		TemplateInterpreter();

		void setInterpreter(Interpreter *i);

		bool parseInfoFile(const std::string &filename);

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

		void setState(State s) {
			m_state = s;
		}
		State state() {
			return m_state;
		}

		void setSourceDirectory(const std::string &dir) {
			m_source_dir = dir;
		}
		void setOutputDirectory(const std::string &dir) {
			m_out_dir = dir;
		}
		void interpretTaskFile(const std::string& infile, std::string outfile);
		void interpretServiceFile(const std::string& infile, std::string outfile);
		void interpretFile(const std::string& infile, std::string outfile = std::string());

		void setLanguage(const std::string &lang) {
			m_language = lang;
		}
		void setComponent(Component *c);

	protected:
		bool parseInfoStream(std::istream& in, const std::string& sname);

		void interpretFileInternal(const std::string &infile, const std::string &outfile);

	private:
		Interpreter *m_interpreter;
		std::string m_language;
		Component *m_component;
		std::string m_out_dir, m_source_dir;

		/// memebers related to parsing info files
		TemplateInfoLexer* m_lexer;
		/// stream name (file or input stream) used for error messages.
		std::string m_streamName;
		/// enable debug output in the flex scanner
		bool m_verboseLexing;
		/// enable debug output in the bison parser
		bool m_verboseParsing;
		State m_state;
};

}

#endif
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;  replace-tabs off;
