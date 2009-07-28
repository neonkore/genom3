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
#include "utils/cppvisitor.h"
#include "utils/idlvisitor.h"
#include "utils/corbacppvisitor.h"

using namespace G3nom;
using namespace Idl;
using namespace boost::python;

std::string MapTypeToC(IdlType::Ptr t, bool declOnly=false)
{
	return CVisitor::mapTypeToC(t, declOnly);
}

std::string MapTypeToCpp(IdlType::Ptr t, bool declOnly=false)
{
	return CppVisitor::mapTypeToCpp(t, declOnly);
}  

std::string MapTypeToCorbaCpp(IdlType::Ptr t, bool declOnly=false, bool isOutType = false)
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
	return CppVisitor::mapValueToCpp(v);
} 

std::string MapValueToCorbaCpp(ConstValue *v)
{
	return CorbaCppVisitor::mapValueToCpp(v);
} 

BOOST_PYTHON_FUNCTION_OVERLOADS(MapTypeToCOverloads, MapTypeToC, 1,2)
BOOST_PYTHON_FUNCTION_OVERLOADS(MapTypeToCppOverloads, MapTypeToCpp, 1,2)
BOOST_PYTHON_FUNCTION_OVERLOADS(MapTypeToCorbaCppOverloads, MapTypeToCorbaCpp, 1,3)

void export_idl()
{
	def("MapTypeToC", &MapTypeToC, MapTypeToCOverloads());
	def("MapTypeToCpp", &MapTypeToCpp, MapTypeToCppOverloads());
	def("MapValueToC", &MapValueToC);
	def("MapTypeToCorbaCpp", &MapTypeToCorbaCpp, MapTypeToCorbaCppOverloads());
	def("MapValueToCpp", &MapValueToCpp);
	def("MapValueToCorbaCpp", &MapValueToCorbaCpp);
	def("MapTypeToIdl", &MapTypeToIdl);

	class_<Literal>("Literal")
	.def("toString", &Literal::print)
	.def("kind", &Literal::kind)
	.def("members", &Literal::members, return_value_policy<reference_existing_object>())
	.def("isEmpty", &Literal::isEmpty);

	class_<ConstValue>("ConstValue")
	.def("identifier", &ConstValue::identifier);
  {
	class_<IdlType, IdlType::Ptr> IdlType_exposer = class_<IdlType, IdlType::Ptr>("IdlType", init<>());
	scope IdlType_scope(IdlType_exposer);
	enum_< G3nom::Idl::IdlType::Kind>("IdlKind")
	.value("Null", G3nom::Idl::IdlType::Null)
	.value("Void", G3nom::Idl::IdlType::Void)
	.value("Short", G3nom::Idl::IdlType::Short)
	.value("Long", G3nom::Idl::IdlType::Long)
	.value("LongLong", G3nom::Idl::IdlType::LongLong)
	.value("UShort", G3nom::Idl::IdlType::UShort)
	.value("ULong", G3nom::Idl::IdlType::ULong)
	.value("ULongLong", G3nom::Idl::IdlType::ULongLong)
	.value("Float", G3nom::Idl::IdlType::Float)
	.value("Double", G3nom::Idl::IdlType::Double)
	.value("LongDouble", G3nom::Idl::IdlType::LongDouble)
	.value("Fixed", G3nom::Idl::IdlType::Fixed)
	.value("Boolean", G3nom::Idl::IdlType::Boolean)
	.value("Char", G3nom::Idl::IdlType::Char)
	.value("WChar", G3nom::Idl::IdlType::WChar)
	.value("Octet", G3nom::Idl::IdlType::Octet)
	.value("String", G3nom::Idl::IdlType::String)
	.value("WString", G3nom::Idl::IdlType::WString)
	.value("Any", G3nom::Idl::IdlType::Any)
	.value("Struct", G3nom::Idl::IdlType::Struct)
	.value("Union", G3nom::Idl::IdlType::Union)
	.value("Enum", G3nom::Idl::IdlType::Enum)
	.value("Sequence", G3nom::Idl::IdlType::Sequence)
	.value("Typedef", G3nom::Idl::IdlType::Typedef)
	.value("Array", G3nom::Idl::IdlType::Array)
	.value("Named", G3nom::Idl::IdlType::Named)
	.export_values();

	IdlType_exposer
	.def("asStructType", &IdlType::asType<StructType>, return_value_policy<reference_existing_object>())
	.def("asEnumType", &IdlType::asType<EnumType>, return_value_policy<reference_existing_object>())
	.def("asNamedType", &IdlType::asType<NamedType>, return_value_policy<reference_existing_object>())
	.def("asTypedefType", &IdlType::asType<TypedefType>, return_value_policy<reference_existing_object>())
	.def("asStringType", &IdlType::asType<StringType>, return_value_policy<reference_existing_object>())
	.def("asArrayType", &IdlType::asType<ArrayType>, return_value_policy<reference_existing_object>())
	.def("asSequenceType", &IdlType::asType<SequenceType>, return_value_policy<reference_existing_object>())
	.def("unalias", &IdlType::unalias)
	.def("identifier", &IdlType::identifier)
	.def("isNative", &IdlType::isNative)
	.def("kind", &IdlType::kind);
	implicitly_convertible< G3nom::Idl::IdlType::Kind, G3nom::Idl::IdlType >();
  }

	class_<BaseType, bases<IdlType> > BaseType_exposer = class_<BaseType, bases<IdlType> >("BaseType", init<IdlType::Kind>());
	BaseType_exposer.def_readwrite( "anyType", G3nom::Idl::BaseType::anyType );
	BaseType_exposer.def_readwrite( "booleanType", G3nom::Idl::BaseType::booleanType );
	BaseType_exposer.def_readwrite( "charType", G3nom::Idl::BaseType::charType );
	BaseType_exposer.def_readwrite( "doubleType", G3nom::Idl::BaseType::doubleType );
	BaseType_exposer.def_readwrite( "floatType", G3nom::Idl::BaseType::floatType );
	BaseType_exposer.def_readwrite( "longType", G3nom::Idl::BaseType::longType );
	BaseType_exposer.def_readwrite( "longdoubleType", G3nom::Idl::BaseType::longdoubleType );
	BaseType_exposer.def_readwrite( "longlongType", G3nom::Idl::BaseType::longlongType );
	BaseType_exposer.def_readwrite( "nullType", G3nom::Idl::BaseType::nullType );
	BaseType_exposer.def_readwrite( "octetType", G3nom::Idl::BaseType::octetType );
	BaseType_exposer.def_readwrite( "shortType", G3nom::Idl::BaseType::shortType );
	BaseType_exposer.def_readwrite( "ulongType", G3nom::Idl::BaseType::ulongType );
	BaseType_exposer.def_readwrite( "ulonglongType", G3nom::Idl::BaseType::ulonglongType );
	BaseType_exposer.def_readwrite( "ushortType", G3nom::Idl::BaseType::ushortType );
	BaseType_exposer.def_readwrite( "voidType", G3nom::Idl::BaseType::voidType );
	BaseType_exposer.def_readwrite( "wcharType", G3nom::Idl::BaseType::wcharType );

	class_<StringType, bases<IdlType> >("StringType", init<int>())
	.def("bound", &StringType::bound)
	.def_readwrite("unboundedStringType", StringType::unboundedStringType);

	void (StructType::*AddMemberStr)(IdlType::Ptr,const std::string &) = &StructType::addMember;
	class_<StructType, bases<IdlType> >("StructType")
	.def("member", &StructType::member)
	.def("addMember", AddMemberStr)
	.def("setIdentifier", &StructType::setIdentifier)
	.def("members", &StructType::members, return_value_policy<reference_existing_object>());

	class_<EnumType, bases<IdlType> >("EnumType")
	.def("enumerators", &EnumType::enumerators, return_value_policy<reference_existing_object>());

	class_<TypedefType, bases<IdlType> >("TypedefType", init<IdlType::Ptr, const std::string &>())
	.def("aliasType", &TypedefType::aliasType);

	class_<NamedType, bases<IdlType> >("NamedType", init<const std::string &, IdlType::Ptr>())
	.def("type", &NamedType::type);

	class_<ArrayType, bases<IdlType> >("ArrayType", init<IdlType::Ptr, int>())
	.def("type", &ArrayType::type)
	.def("bounds", &ArrayType::bounds, return_value_policy<reference_existing_object>())
	.def("printBounds", &ArrayType::printBounds);

	class_<SequenceType, bases<IdlType> >("SequenceType", init<IdlType::Ptr, int>())
	.def("seqType", &SequenceType::seqType)
	.def("bound", &SequenceType::bound);

}
