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
#include "cvisitor.h"

#include <iostream>
#include <sstream>

#include "idltype.h"
#include "idlvalues.h"

#define INDENT_QUANTUM "    "

using namespace G3nom;
using namespace Idl;
using namespace std;

std::string CVisitor::mapTypeToC(IdlType::Ptr t, bool declOnly)
{
	if(t.get() == 0) {
		cerr << "Trying to map a null type" << endl;
		return "<NULL TYPE>";
	}

	std::string s;
	ostringstream oss(s);
	CVisitor visitor(oss, declOnly);
	t->accept(visitor);
	return oss.str();
}

std::string CVisitor::mapValueToC(ConstValue *v)
{
	std::string s;
	ostringstream oss(s);
	CVisitor visitor(oss, true);
	v->accept(visitor);
	return oss.str();
}

void CVisitor::visitBaseType(BaseType *base)
{
	switch (base->kind()) {
		case IdlType::Null:
			m_out << "null"; break;
		case IdlType::Void:
			m_out << "void"; break;
		case IdlType::Short:
			m_out << "short"; break;
		case IdlType::Long:
			m_out << "int"; break;
		case IdlType::LongLong:
			m_out << "long"; break;
		case IdlType::UShort:
			m_out << "unsigned short"; break;
		case IdlType::ULong:
			m_out << "unsigned int"; break;
		case IdlType::ULongLong:
			m_out << "unsigned long"; break;
		case IdlType::Float:
			m_out << "float"; break;
		case IdlType::Double:
		case IdlType::LongDouble:
			m_out << "double"; break;
		case IdlType::Boolean:
			m_out << "unsigned char"; break;
		case IdlType::Char:
		case IdlType::Octet:
			m_out << "char"; break;
		case IdlType::WChar:
			m_out << "wchar_t"; break;
		default:
			m_out << "unknown";
	}
}

void CVisitor::visitStringType(StringType *s)
{
	m_out << "char *";
}

void CVisitor::visitWStringType(WStringType *ws)
{
	m_out << "char *";
}

void CVisitor::visitSequenceType(SequenceType *seq)
{
	m_out << "struct {" << endl;
	string oldIndent = m_indent;
	m_indent += INDENT_QUANTUM;

	m_out << m_indent << "int length;" << endl;
	m_out << m_indent << "int size;" << endl;
	m_out << m_indent;
	seq->seqType()->accept(*this);
	m_out << " *data;" << endl;

	m_indent = oldIndent;

	m_out << m_indent << "}";
}

void CVisitor::visitFixedType(FixedType *fixed)
{
	/// todo
}

void CVisitor::visitStructType(StructType *s)
{
	if(m_declOnly) {
		m_out << s->identifier();
		return;
	}

	m_out << "typedef struct " + s->identifier() << "{" << endl;
	string oldIndent = m_indent;
	m_indent += INDENT_QUANTUM;

	IdlType::Map::const_iterator it = s->members().begin();
	for(; it != s->members().end(); ++it) {
		m_out << m_indent;
		if(it->second->kind() == IdlType::String) {
			StringType *st = it->second->asType<StringType>();
			if(st)
				m_out << "char " << it->first << "[" << st->bound() <<  "];" << endl;
			continue;
		} 

		it->second->accept(*this); // no need to set declOnly because of NamedType
		m_out << " " << it->first;
		//print array if existing
		if(it->second->kind() == IdlType::Array)
			m_out << it->second->asType<ArrayType>()->printBounds();

		m_out << ";" << endl;
	}
	m_indent = oldIndent;
	m_out << m_indent << "} " << s->identifier();
}

void CVisitor::visitTypedefType(TypedefType *t)
{
	string id = t->declarators()->at(0)->identifier(); 
	if(m_declOnly) {
		m_out << id;
		return;
	}
	m_out << "typedef ";
	t->aliasType()->accept(*this);
	m_out << " " << id;
}

void CVisitor::visitEnumType(EnumType *e)
{
	if(m_declOnly) {
		m_out << e->identifier();
		return;
	}

	m_out << " typedef enum " << e->identifier();
	m_out << "{" << endl << m_indent << INDENT_QUANTUM;
	bool first = true;
	std::vector<std::string>::const_iterator it;
	for (it = e->enumerators().begin(); it != e->enumerators().end(); ++it) {
		if (!first)
			m_out << ", \n" << m_indent << INDENT_QUANTUM;
		else
			first = false;

		m_out << *it;
	}
	m_out << endl << m_indent << "} " << e->identifier() << endl;
}

void CVisitor::visitArrayType(ArrayType *a)
{
	a->type()->accept(*this);
}

void CVisitor::visitNamedType(NamedType *n)
{
	bool savedDeclOnly = m_declOnly;
	m_declOnly = true;
	n->type()->accept(*this);
	m_declOnly = savedDeclOnly;
}

void CVisitor::visitConstValue(ConstValue *v) 
{
	m_out << "#define " << v->identifier() << " (" << v->value().print() << ")" << endl;
}

// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;  replace-tabs off;
