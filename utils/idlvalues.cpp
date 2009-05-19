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

#include "idlvalues.h"

#include <string>
#include <sstream>

using namespace G3nom;
using namespace Idl;
using namespace std;

/******** Literal ***************/

std::string Literal::print() const
{
	string s;
	stringstream ss;
	switch(m_kind) {
		case Struct: {
			ss << "{";
			bool first = true;
			for(Literal::Vector::const_iterator it = m_members.begin(); it != m_members.end(); ++it) {
				if(first)
					first = false;
				else
					ss << ", ";
				ss << it->print();
			}
			ss << "}";
			break;
		} case Plus: {
			ss << m_members.front().print();
			ss << " + " << m_members.back().print();
			break;
		} case Minus: {
			ss << m_members.front().print();
			ss << " - " << m_members.back().print();
			break;
		} case Times: {
			ss << m_members.front().print();
			ss << " * " << m_members.back().print();
			break;
		} case Divide: {
			ss << m_members.front().print();
			ss << " / " << m_members.back().print();
			break;
		} case Mod: {
			ss << m_members.front().print();
			ss << " % " << m_members.back().print();
			break;
		} case And: {
			ss << m_members.front().print();
			ss << " & " << m_members.back().print();
			break;
		} case Or: {
			ss << m_members.front().print();
			ss << " | " << m_members.back().print();
			break;
		} case Xor: {
			ss << m_members.front().print();
			ss << " ^ " << m_members.back().print();
			break;
		} case LShift: {
			ss << m_members.front().print();
			ss << " << " << m_members.back().print();
			break;
		} case RShift: {
			ss << m_members.front().print();
			ss << " >> " << m_members.back().print();
			break;
		}
		default:
			ss << m_value;
	}
	return ss.str();
}

void Literal::addMember(const Literal &l)
{
	m_isEmpty = false;
	m_members.push_back(l);
}

// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;  replace-tabs off;
