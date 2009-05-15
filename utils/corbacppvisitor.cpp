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
#include "corbacppvisitor.h"

#include <sstream>

#include "idltype.h"
#include "idlvalues.h"

using namespace G3nom;
using namespace Idl;
using namespace std;

std::string CorbaCppVisitor::mapTypeToCpp(IdlType::Ptr t, bool declOnly, bool isOutType)
{
	std::string s;
	ostringstream oss(s);
	CorbaCppVisitor visitor(oss, declOnly, isOutType);
	t->accept(visitor);
	return oss.str();
}

std::string CorbaCppVisitor::mapValueToCpp(ConstValue *v)
{
	std::string s;
	ostringstream oss(s);
	CorbaCppVisitor visitor(oss, true, false);
	v->accept(visitor);
	return oss.str();
}

void CorbaCppVisitor::visitBaseType(BaseType *base)
{
	switch (base->kind()) {
		case IdlType::Null:
			m_out << "null"; break;
		case IdlType::Void:
			m_out << "void"; break;
		case IdlType::Short:
			m_out << "CORBA::Short"; break;
		case IdlType::Long:
			m_out << "CORBA::Long"; break;
		case IdlType::LongLong:
			m_out << "CORBA::LongLong"; break;
		case IdlType::UShort:
			m_out << "CORBA::UShort"; break;
		case IdlType::ULong:
			m_out << "CORBA::ULong"; break;
		case IdlType::ULongLong:
			m_out << "CORBA::ULongLong"; break;
		case IdlType::Float:
			m_out << "CORBA::Float"; break;
		case IdlType::Double:
			m_out << "CORBA::Double"; break;
		case IdlType::LongDouble:
			m_out << "CORBA::LongDouble"; break;
		case IdlType::Boolean:
			m_out << "CORBA::Boolean"; break;
		case IdlType::Char:
			m_out << "CORBA::Char"; break;
		case IdlType::Octet:
			m_out << "CORBA::Octet"; break;
		case IdlType::WChar:
			m_out << "CORBA::WChar"; break;
		default:
			m_out << "unknown";
	}
}

void CorbaCppVisitor::visitStringType(StringType *s)
{
	m_out << "String_var";
}

void CorbaCppVisitor::visitWStringType(WStringType *ws)
{
	m_out << "WString_var";
}

void CorbaCppVisitor::visitSequenceType(SequenceType *seq)
{
	/// todo
}

void CorbaCppVisitor::visitFixedType(FixedType *fixed)
{
	/// todo
}

void CorbaCppVisitor::visitStructType(StructType *s)
{
	if(m_declOnly) {
		m_out << s->identifier();
		return;
	}

	m_out << "struct " + s->identifier();
	m_out << "{" << endl;
	IdlType::Map::const_iterator it = s->members().begin();
	for(; it != s->members().end(); ++it) {
		if(it->second->kind() == IdlType::String) {
			StringType *st = it->second->asStringType();
			if(st)
				m_out << "   char " << it->first << "[" << st->bound() <<  "];" << endl;
			continue;
		}
		m_out  << "   ";
		it->second->accept(*this); // no need to set declOnly because of NamedType
		m_out << " " << it->first;

		//print array if existing
		if(it->second->kind() == IdlType::Array) {
			ArrayType *a = it->second->asType<ArrayType>();
			std::vector<int>::const_iterator it3 = a->bounds().begin();
			for (; it3 != a->bounds().end(); ++it3)
				m_out << "[" << *it3 << "]";
		}
		m_out << ";" << endl;
	}
	m_out << "}";
}

void CorbaCppVisitor::visitTypedefType(TypedefType *t)
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

void CorbaCppVisitor::visitEnumType(EnumType *e)
{
	if(m_declOnly) {
		if(m_isOutType)
			m_out << "CORBA::Long";
		else
			m_out << e->identifier();
		return;
	}

	m_out << "enum " << e->identifier();
	m_out << "{" << endl;
	bool first = true;
	std::vector<std::string>::const_iterator it;
	for (it = e->enumerators().begin(); it != e->enumerators().end(); ++it) {
		if (!first)
			m_out << ", ";
		else
			first = false;

		m_out << *it;
	}
	m_out << "}" << endl;
}

void CorbaCppVisitor::visitArrayType(ArrayType *a)
{
	/// todo
}

void CorbaCppVisitor::visitNamedType(NamedType *n)
{
	bool savedDeclOnly = m_declOnly;
	m_declOnly = true;
	n->type()->accept(*this);
	m_declOnly = savedDeclOnly;
}

void CorbaCppVisitor::visitConstValue(ConstValue *v) 
{
	m_out << "const " << CorbaCppVisitor::mapTypeToCpp(v->type(), true) << v->identifier() << " = " << v->value().print() << ";" << endl;
}

// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;  replace-tabs off;
