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

#ifndef G3NOM_INTERPRETER_H
#define G3NOM_INTERPRETER_H

#include <string>

namespace G3nom
{

class Component;

/** \short A template file interpreter 

* \sa TemplateInterpreter
*/
class Interpreter
{
	public:
		Interpreter();
		virtual ~Interpreter();

		/** Interpret \a s .
		* \return the output of the code (for instance the text that was written to stdout)
		*/
		virtual std::string interpret(const std::string &s) = 0;
		/** Print the value of \a s.*/
		virtual std::string eval(const std::string &s) = 0;

		/** Defines the var \a name in the interpreter and assign it \a value.*/
		virtual void exportVar(const std::string &name, const std::string &value) = 0;

		/** \return the code to print the string \a s (for instance to stdout if it is redirected).
		* This code will be inserted in the template file 
		* between '?>' and '<?' tags.
		*/
		virtual std::string printString(const std::string &s) = 0;
		/** \return the code to print the evaluate \a s and print its value (for instance to stdout if it is redirected).
		* This code will be inserted in the template file 
		* between '<!' and '!>' tags.
		*/
		virtual std::string evalString(const std::string &s) = 0;

		/** \return The currently active component. */
		Component* component();

	protected:
		Component *m_component;
};

}

#endif
// kate: indent-mode cstyle; replace-tabs off; tab-width 4; 
