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

#ifndef G3NOM_IDLVALUES_H
#define G3NOM_IDLVALUES_H

#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>

#include "idltype.h"
#include "typevisitor.h"

namespace G3nom
{

namespace Idl {

class Literal {
	public:
		enum Kind { None, Bool, Char, Int, Double, String, Struct, Plus, Minus, Times,
		  Divide, Mod, And, Or, Xor, LShift, RShift, Neg, LNeg};
		typedef std::map<std::string, Literal> Map;
		typedef std::vector<Literal> Vector;

		Literal(Kind k = None) : m_kind(k) , m_isEmpty(true) {}
		Literal(char c) : m_kind(Char), m_value(c), m_isEmpty(false)  {}
		Literal(int i) : m_kind(Int), m_value(i), m_isEmpty(false)  {}
		Literal(std::string s) : m_kind(String), m_value(s), m_isEmpty(false)  {}
		Literal(double d) : m_kind(Double), m_value(d), m_isEmpty(false)  {}
		Literal(bool b) : m_kind(Bool), m_value(b), m_isEmpty(false)  {}

		void addMember(const Literal &l);
		Literal::Vector& members() { return m_members; }

		bool isEmpty() const { return m_isEmpty; }
		std::string print() const;

		int asBool() const { return boost::get<bool>(m_value); }
		char asChar() const { return (char) boost::get<int>(m_value); }
		int asInt() const { return boost::get<int>(m_value); }
		double asDouble() const  { return boost::get<double>(m_value); }
		std::string asString() const  { return boost::get<std::string>(m_value); }

	private:
		Kind m_kind;
		boost::variant<bool, int, double, std::string> m_value;
		bool m_isEmpty;
		Literal::Vector m_members;
};

class ConstValue {
	public:
		typedef std::map<std::string, ConstValue> Map;

		ConstValue() {}
		ConstValue(const std::string &name, IdlType::Ptr type, Literal value)
		: m_identifier(name), m_type(type), m_value(value)
		{}

		std::string identifier() const { return m_identifier; }
		const Literal& value() const { return m_value; }
		IdlType::Ptr type() const { return m_type; }

		void accept(TypeVisitor& visitor) {
			visitor.visitConstValue(this);
		}

	private:
		std::string m_identifier;
		IdlType::Ptr m_type;
		Literal m_value;
};

}

}

#endif
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;  replace-tabs off;
