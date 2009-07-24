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
#include "cppvisitor.h"

#include <iostream>
#include <sstream>

#include "idltype.h"
#include "idlvalues.h"

#define INDENT_QUANTUM "    "

using namespace G3nom;
using namespace Idl;
using namespace std;

std::string CppVisitor::mapTypeToCpp(IdlType::Ptr t, bool declOnly)
{
	if(!t) {
		cerr << "Trying to map a null type" << endl;
		return "<NULL TYPE>";
	}

	std::string s;
	ostringstream oss(s);
	CppVisitor visitor(oss, declOnly);
	t->accept(visitor);
	return oss.str();
}

std::string CppVisitor::mapValueToCpp(ConstValue *v)
{
	std::string s;
	ostringstream oss(s);
	CppVisitor visitor(oss, true);
	v->accept(visitor);
	return oss.str();
}

// void CppVisitor::visitStringType(StringType *s)
// {
// 	m_out << "std::string";
// }
// 
// void CppVisitor::visitWStringType(WStringType *ws)
// {
// 	m_out << "std::string";
// }

void CppVisitor::visitConstValue(ConstValue *v) 
{
	m_out << "const ";
	v->type()->accept(*this);
	m_out << " " << v->identifier() << " = " << v->value().print() << ";" << endl;
}

// void CppVisitor::visitStructType(StructType *s)
// {
// 	m_out << "struct " + s->identifier();
// 	if(m_declOnly)
// 		return;
// 
// 	m_out << "{" << endl;
// 	string oldIndent = m_indent;
// 	m_indent += INDENT_QUANTUM;
// 
// 	IdlType::Map::const_iterator it = s->members().begin();
// 	for(; it != s->members().end(); ++it) {
// 		m_out << m_indent;
// 
// 		it->second->accept(*this); // no need to set declOnly because of NamedType
// 		m_out << " " << it->first;
// 		//print array if existing
// 		if(it->second->kind() == IdlType::Array)
// 			m_out << it->second->asType<ArrayType>()->printBounds();
// 
// 		m_out << ";" << endl;
// 	}
// 	m_indent = oldIndent;
// 	m_out << m_indent << "}";
// }

// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;  replace-tabs off;
