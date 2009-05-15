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
#include "dumptypes.h"

#include <sstream>

#include "idltype.h"

using namespace G3nom;
using namespace Idl;
using namespace std;

std::string DumpType::dumpType(IdlType::Ptr t)
{
	std::string s;
	ostringstream oss(s);
	DumpType visitor(oss);
	t->accept(visitor);
	return oss.str();
}

std::string DumpType::dumpConstValue(ConstValue &v)
{
	std::string s;
	ostringstream oss(s);
	DumpType visitor(oss);
	v.accept(visitor);
	return oss.str();
}

void DumpType::visitBaseType(BaseType *base)
{
	m_out << base->kindAsString();
}

void DumpType::visitStringType(StringType *s)
{
	if (s->bound() == 0)
		m_out << s->kindAsString();
	else
		m_out << "string<" << s->bound() << ">";
}

void DumpType::visitWStringType(WStringType *ws)
{
	if (ws->bound() == 0)
		m_out << ws->kindAsString();
	else
		m_out << "wstring<" << ws->bound() << ">";
}

void DumpType::visitSequenceType(SequenceType *seq)
{
	m_out << "sequence<";
	seq->seqType()->accept(*this);
	if (seq->bound() == 0)
		m_out << "> ";
	else
		m_out << ", " << seq->bound() << "> ";
}

void DumpType::visitFixedType(FixedType *fixed)
{
	m_out << "Fixed<" << fixed->digits() << "," << fixed->scale() << "> ";
}

void printDeclaratorVectString(ostream &m_out, Declarator::VectPtr v)
{
	if (!v)
		return;

	bool first = true;
	Declarator::Vect::const_iterator it = v->begin();
	for (; it != v->end(); ++it) {
		if (!first)
			m_out << ", ";
		else
			first = false;
		m_out << (*it)->identifier();

		//print array if existing
		std::vector<int>::const_iterator it2 = (*it)->bounds().begin();
		for (; it2 != (*it)->bounds().end(); ++it2)
			m_out << "[" << *it2 << "]";
	}
}

void DumpType::visitStructType(StructType *s)
{
	m_out << "Struct " << s->identifier() << " {";
	IdlType::Map::const_iterator it;
	for (it = s->members().begin(); it != s->members().end(); ++it) {
		m_out << "\n\t";
		it->second->accept(*this);
		m_out << " " << it->first << ";";
	}
	m_out << "\n}";
}

void DumpType::visitTypedefType(TypedefType *t)
{
	m_out << "Typedef ";
	t->aliasType()->accept(*this);
	m_out << " ";
	printDeclaratorVectString(m_out, t->declarators());
}

void DumpType::visitEnumType(EnumType *e)
{
	m_out << "Enum " << e->identifier() << " { ";

	bool first = true;
	std::vector<std::string>::const_iterator it;
	for (it = e->enumerators().begin(); it != e->enumerators().end(); ++it) {
		if (!first)
			m_out << ", ";
		else
			first = false;

		m_out << *it;
	}
	m_out << "} ";
}

void DumpType::visitArrayType(ArrayType *a)
{
	m_out << "array" << a->printBounds() << " of " ;
	a->type()->accept(*this);
}

void DumpType::visitNamedType(NamedType *n)
{
	m_out << "named:" << n->identifier();
}

void DumpType::visitConstValue(ConstValue *v) 
{
	m_out << "const " << v->type()->identifier();
	m_out << " " << v->identifier() << " = " << v->value().print();
}

// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;  replace-tabs off;
