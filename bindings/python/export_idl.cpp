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
#include "utils/idlvalues.h"
#include "utils/cvisitor.h"
#include "utils/idlvisitor.h"
#include "utils/corbacppvisitor.h"

using namespace G3nom;
using namespace Idl;
using namespace boost::python;

std::string MapTypeToC(IdlType::Ptr t, bool declOnly=false)
{
	return CVisitor::mapTypeToC(t, declOnly);
}

std::string MapTypeToCpp(IdlType::Ptr t, bool declOnly=false, bool isOutType = false)
{
	return CorbaCppVisitor::mapTypeToCpp(t, declOnly, isOutType);
}  

std::string MapTypeToIdl(IdlType::Ptr t)
{
	return IdlVisitor::mapTypeToIdl(t);
}

std::string MapValueToC(ConstValue *v)
{
	return CVisitor::mapValueToC(v);
} 

std::string MapValueToCpp(ConstValue *v)
{
	return CorbaCppVisitor::mapValueToCpp(v);
} 

BOOST_PYTHON_FUNCTION_OVERLOADS(MapTypeToCOverloads, MapTypeToC, 1,2)
BOOST_PYTHON_FUNCTION_OVERLOADS(MapTypeToCppOverloads, MapTypeToCpp, 1,3)

void export_idl()
{
	def("MapTypeToC", &MapTypeToC, MapTypeToCOverloads());
	def("MapValueToC", &MapValueToC);
	def("MapTypeToCpp", &MapTypeToCpp, MapTypeToCppOverloads());
	def("MapValueToCpp", &MapValueToCpp);
	def("MapTypeToIdl", &MapTypeToIdl);

	class_<Literal>("Literal")
	.def("print", &Literal::print)
	.def("isEmpty", &Literal::isEmpty);

	class_<ConstValue>("ConstValue")
	.def("identifier", &ConstValue::identifier);

	class_<IdlType, IdlType::Ptr>("IdlType")
	.def("asStructType", &IdlType::asStructType, return_value_policy<reference_existing_object>())
	.def("asEnumType", &IdlType::asEnumType, return_value_policy<reference_existing_object>())
	.def("asNamedType", &IdlType::asNamedType, return_value_policy<reference_existing_object>())
	.def("asTypedefType", &IdlType::asTypedefType, return_value_policy<reference_existing_object>())
	.def("asStringType", &IdlType::asStringType, return_value_policy<reference_existing_object>())
	.def("asArrayType", &IdlType::asArrayType, return_value_policy<reference_existing_object>())
	.def("unalias", &IdlType::unalias)
	.def("identifier", &IdlType::identifier)
	.def("kind", &IdlType::kind);

	object baseTypeClass = class_<BaseType, bases<IdlType> >("BaseType", init<IdlType::Kind>());
	baseTypeClass.attr("voidType") = &BaseType::voidType;
	baseTypeClass.attr("longType") = &BaseType::longType;

	void (StructType::*AddMemberStr)(IdlType::Ptr,const std::string &) = &StructType::addMember;
	object stringTypeClass = class_<StringType, bases<IdlType> >("StringType", init<int>())
	.def("bound", &StringType::bound);
	stringTypeClass.attr("unboundedStringType") = &StringType::unboundedStringType;
// 	.add_static_property("unboundedStringType", &StringType::unboundedStringType);

	class_<StructType, bases<IdlType> >("StructType")
	.def("member", &StructType::member)
	.def("addMember", AddMemberStr)
	.def("setIdentifier", &StructType::setIdentifier)
	.def("members", &StructType::members, return_value_policy<reference_existing_object>());

	class_<EnumType, bases<IdlType> >("EnumType")
	.def("enumerators", &EnumType::enumerators, return_value_policy<reference_existing_object>());

	class_<TypedefType, bases<IdlType> >("TypedefType")
	.def("aliasType", &TypedefType::aliasType);

	class_<NamedType, bases<IdlType> >("NamedType")
	.def("type", &NamedType::type);

	class_<ArrayType, bases<IdlType> >("ArrayType")
	.def("type", &ArrayType::type)
	.def("bounds", &ArrayType::bounds, return_value_policy<reference_existing_object>())
	.def("printBounds", &ArrayType::printBounds);

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
	.value("Double", IdlType::Double)
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
	.value("Typedef", IdlType::Typedef)
	.value("Named", IdlType::Named)
	.value("Array", IdlType::Array);
}
