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

#include <boost/python.hpp>

#include "utils/idltype.h"

using namespace G3nom;
using namespace Idl;
using namespace boost::python;

void export_idl()
{
	class_<IdlType, IdlType::Ptr>("IdlType")
	.def("asStructType", &IdlType::asStructType, return_value_policy<reference_existing_object>())
	.def("asEnumType", &IdlType::asEnumType, return_value_policy<reference_existing_object>())
	.def("asTypedefType", &IdlType::asTypedefType, return_value_policy<reference_existing_object>())
	.def("toCType", &IdlType::toCType)
	.def("identifier", &IdlType::identifier)
	.def("kind", &IdlType::kind);

	class_<StructType>("StructType")
	.def("member", &StructType::member);

	class_<EnumType>("EnumType")
	.def("enumerators", &EnumType::enumerators, return_value_policy<reference_existing_object>());

	class_<TypedefType>("TypedefType")
	.def("aliasType", &TypedefType::aliasType);

	enum_<IdlType::Kind>("IdlKind")
	.value("Null", IdlType::Null)
	.value("Void", IdlType::Void)
	.value("Short", IdlType::Short)
	.value("Long", IdlType::Long)
	.value("LongLong", IdlType::LongLong)
	.value("UShort", IdlType::UShort)
	.value("ULong", IdlType::ULong)
	.value("ULongLong", IdlType::ULongLong)
	.value("Float", IdlType::Float)
	.value("Fixed", IdlType::Fixed)
	.value("Boolean", IdlType::Boolean)
	.value("Char", IdlType::Char)
	.value("WChar", IdlType::WChar)
	.value("Octet", IdlType::Octet)
	.value("String", IdlType::String)
	.value("WString", IdlType::WString)
	.value("Any", IdlType::Any)
	.value("Struct", IdlType::Struct)
	.value("Union", IdlType::Union)
	.value("Enum", IdlType::Enum)
	.value("Sequence", IdlType::Sequence)
	.value("Typedef", IdlType::Typedef);
}
