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

#ifndef G3NOM_IDLTYPE_H
#define G3NOM_IDLTYPE_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include <boost/shared_ptr.hpp>

#include "typevisitor.h"

namespace G3nom
{

/** Base namespace for classes related to IDL types */
namespace Idl {

/** \short The base class for all IDL types

* IdlType should only be stored using IDlType::Ptr (which is a shared ptr) to avoir memory leak.
* Casting can be done using #asType() that return a regular pointer that should never be stored or used to
* create a new shared ptr.
*/
class IdlType
{
	public:
		enum Kind {
			Null, Void, Short, Long, LongLong, UShort, ULong, ULongLong,
			Float, Double, LongDouble, Fixed, Boolean, Char, WChar, Octet, String,
			WString, Any, Struct, Union, Enum, Sequence, Typedef, Array, Named
		};
		/// A (shared) pointer to a IdlType
		typedef boost::shared_ptr<IdlType> Ptr;
		typedef std::map<std::string, Ptr> Map;
		/** Map that keeps the order of insertion. */
		typedef std::vector< std::pair<std::string, Ptr> > OrderedMap; 
		typedef std::vector<Ptr> Vector;

		IdlType() : m_kind(Null), m_isNative(false) {}
		IdlType(Kind k);
		virtual ~IdlType() {};

		Kind kind() const {
			return m_kind;
		}
		std::string kindAsString() const;

		virtual bool isNative() const { return m_isNative; }
		void setNative(bool isNative) { m_isNative = isNative; }

		// casting functions
		template<class T> T* asType() {
			return static_cast<T*>(this);
		}

		/** \return an equivalent IdlType object with aliases stripped 
		* or IdlType::Ptr() if the type is not an alias. 
		* This function cannot return a shared ptr to this because this would not work
		* every time #asType() is called. To be able to return a shared ptr, we neet to be 
		* absolutely sure that every time this function is called, the corresponding object
		* is hold by a shared ptr, which is not always the case. The solution avoids some
		* tricky bugs.*/
		IdlType::Ptr unalias();

		/** This function is used to implement the Visitor pattern together with
		* the TypeVisitor class.
		*/
		virtual void accept(TypeVisitor& visitor) {}

// 		virtual std::vector<std::string> identifiers() {
// 			return std::vector<std::string>();
// 		}

		virtual std::string identifier() const { return std::string(); }

	private:
		Kind m_kind;
		bool m_isNative;
};

typedef IdlType::Ptr IdlTypePtr;

/** \short A declarator (ie what is found after a type name in a type definition)

It can be either a simple declarator (eg int a;) or an array declarator (eg int a[10][10])
*/
class Declarator
{
	public:
		Declarator(std::string identifier)
				: m_identifier(identifier) {}
		~Declarator() {}

		typedef boost::shared_ptr<Declarator> Ptr;
		typedef std::map<std::string, Declarator::Ptr> Map;
		typedef std::vector<Declarator::Ptr> Vect;
		typedef boost::shared_ptr<Vect> VectPtr;

		const std::string & identifier() const {
			return m_identifier;
		}
		IdlType::Ptr type()  const {
			return m_type;
		}
		bool isArray() const;
		void addBound(int b);
		std::vector<int> & bounds() {
			return m_bounds;
		}

	private:
		std::string m_identifier;
		IdlType::Ptr    m_type;
		std::vector<int> m_bounds;
};

// class DeclaratorVect : public std::vector<Declarator*> {
// 	public:
// 		DeclaratorVect()
// 		: std::vector<Declarator*>()
// 		{}
// };

typedef std::pair<IdlType::Ptr, Declarator::VectPtr> TypeDeclarator;

/* Basic Types */

/** \short Idl base types

* These types are allocated once at the beginning of the program to avoid duplication.
*/
class BaseType : public IdlType
{
	public:
		BaseType(Kind k) : IdlType(k) {}
		virtual ~BaseType() {}

		void accept(TypeVisitor& visitor) {
			visitor.visitBaseType(this);
		}

		// Static base type singletons
		static IdlType::Ptr nullType;
		static IdlType::Ptr voidType;
		static IdlType::Ptr shortType;
		static IdlType::Ptr longType;
		static IdlType::Ptr ushortType;
		static IdlType::Ptr ulongType;
		static IdlType::Ptr floatType;
		static IdlType::Ptr doubleType;
		static IdlType::Ptr booleanType;
		static IdlType::Ptr charType;
		static IdlType::Ptr octetType;
		static IdlType::Ptr anyType;
		static IdlType::Ptr longlongType;
		static IdlType::Ptr ulonglongType;
		static IdlType::Ptr longdoubleType;
		static IdlType::Ptr wcharType;
};

class StringType : public IdlType
{
	public:

		StringType(int bound) : IdlType(String), m_bound(bound) { }
		virtual ~StringType() {}

		int bound() {
			return m_bound;
		}

		void accept(TypeVisitor& visitor) {
			visitor.visitStringType(this);
		}

		static StringType::Ptr unboundedStringType;

	private:
		int m_bound;
};


class WStringType : public IdlType
{
	public:
		WStringType(int bound) : IdlType(WString), m_bound(bound) { }
		virtual ~WStringType() {}

		int bound() {
			return m_bound;
		}

		void accept(TypeVisitor& visitor) {
			visitor.visitWStringType(this);
		}

		static WStringType::Ptr unboundedWStringType;

	private:
		int m_bound;
};

class SequenceType : public IdlType
{
	public:
		SequenceType(IdlType::Ptr seqType, int bound) :
				IdlType(Sequence), m_seqType(seqType), m_bound(bound) {}
		virtual ~SequenceType() {}

		IdlType::Ptr seqType() {
			return m_seqType;
		}
		int bound() {
			return m_bound;
		}

		void accept(TypeVisitor& visitor) {
			visitor.visitSequenceType(this);
		}

	private:
		IdlType::Ptr m_seqType;
		int m_bound;
};

class FixedType : public IdlType
{
	public:
		FixedType(int digits, int scale) :
				IdlType(Fixed), m_digits(digits), m_scale(scale) { }

		virtual ~FixedType() {}

		int digits() {
			return m_digits;
		}
		int scale() {
			return m_scale;
		}

		void accept(TypeVisitor& visitor) {
			visitor.visitFixedType(this);
		}

	private:
		int m_digits;
		int m_scale;
};

/* More complex types*/

/** \short A typedef
*/
class TypedefType : public IdlType
{
	public:
		TypedefType() : IdlType(Typedef) {}
		TypedefType(IdlType::Ptr aliasType, const std::string &identifier);
		TypedefType(IdlType::Ptr aliasType, Declarator::VectPtr declarators)
				: IdlType(Typedef), m_aliasType(aliasType), m_declarators(declarators) {}
		virtual ~TypedefType() {}

		const std::string kindAsString() const;
		virtual bool isNative() const;

		IdlType::Ptr  aliasType() const {
			return m_aliasType;
		}
		Declarator::VectPtr declarators() const {
			return m_declarators;
		}
		bool hasIdentifier(const std::string &name);
		virtual std::string identifier() const;

		virtual void accept(TypeVisitor& visitor) {
			visitor.visitTypedefType(this);
		}

	private:
		IdlType::Ptr m_aliasType;
		Declarator::VectPtr m_declarators;
};

class StructType : public IdlType
{
	public:
		StructType()
				: IdlType(Struct) {}
		virtual ~StructType() {}

		const std::string kindAsString() const;

		virtual std::string identifier() const {
			return m_identifier;
		}
		void setIdentifier(const std::string &id) {
			m_identifier = id;
		}

		/** This overload, using a Declarator::VectPtr argument, should only be used
		* by the parser. If you want to add a member by hand (eg from a script), use
		* the simpler version expecting an identifier and a type. 
		*/
		void addMember(IdlType::Ptr t, Declarator::VectPtr declarators);
		void addMember(IdlType::Ptr t, const std::string &name);
		const IdlType::OrderedMap& members() const {
			return m_members;
		}
		IdlType::Ptr member(const std::string &name) ;

		bool isRecursive() const {
			return m_isRecursive;
		}
		void setRecursive() {
			m_isRecursive = true;
		}

		virtual void accept(TypeVisitor& visitor) {
			visitor.visitStructType(this);
		}

	private:
		std::string m_identifier;
// 		std::vector<TypeDeclarator> m_members;
		IdlType::OrderedMap m_members;
		bool m_isRecursive;
};

/** \short An enumeration
* A value is simply a name (Idl does not allow to set the value like 
* eg in C)
*/
class EnumType : public IdlType
{
	public:
		EnumType()
				: IdlType(Enum) {}
		virtual ~EnumType() {}

		const std::string kindAsString() const;

		std::string identifier() const {
			return m_identifier;
		}
		void setIdentifier(const std::string &id) {
			m_identifier = id;
		}

		void addEnumerator(const std::string &e);
		const std::vector<std::string>& enumerators() const {
			return m_enum;
		}

		void accept(TypeVisitor& visitor) {
			visitor.visitEnumType(this);
		}

	private:
		std::string m_identifier;
		std::vector<std::string> m_enum;
};

/** \short A n-dimension array*/
class ArrayType : public IdlType {
	public:
		ArrayType() : IdlType(Array) {}
		ArrayType(IdlType::Ptr p, int bound);
		ArrayType(IdlType::Ptr p, const std::vector<int> &bounds)
		: IdlType(Array), m_type(p), m_bounds(bounds)
		{}

		std::vector<int> & bounds() {
			return m_bounds;
		}
		std::string printBounds() const;

		IdlType::Ptr type() const { return m_type; }

		virtual void accept(TypeVisitor& visitor) {
			visitor.visitArrayType(this);
		}

	private:
		IdlType::Ptr m_type;
		std::vector<int> m_bounds;
};

/** \short A pointer to a type with an identifier

* This type is the result of G3nom::Component::findByName(). It is used to differentiate
* between a type definition and its use (see example below).
\code
struct B {
  long y;
}

struct A {
  B b;
}

struct A_bis {
  struct B {
	long y;
  } b;
}
\endcode
*/
class NamedType : public IdlType {
	public:
		NamedType() : IdlType(Named) {}
		NamedType(const std::string &name, IdlType::Ptr type)
		: IdlType(Named), m_identifier(name), m_type(type)
		{}

		virtual bool isNative() const;

		IdlType::Ptr type() const { return m_type; }
		virtual std::string identifier() const {
			return m_identifier;
		}

		virtual void accept(TypeVisitor& visitor) {
			visitor.visitNamedType(this);
		}

	private:
		std::string m_identifier;
		IdlType::Ptr m_type;
};

// class UnionCase {
// public:
//   typedef std::map<std::string, Member> Map;
//
//   UnionCase(IdlType* memberType, Declarator* declarators);
//   virtual ~UnionCase();
//
// //   const char* kindAsString() const { return "member"; }
//
//   // Queries
//   IdlType* caseType()  const { return m_caseType; }
//   Declarator*  declarators() const { return m_declarators; }
//
// //   void accept(AstVisitor& visitor) { visitor.visitMember(this); }
//
// private:
//   IdlType* m_caseType;
// //   IDL_Boolean    delType_;
//   Declarator* m_declarators;
// };

// class UnionType : public IdlType {
// public:
//   UnionType(const std::string &identifier)
//   : IdlType(Union), m_identifier(identifier)
//   {}
//   virtual ~UnionType();
//
//   const std::string kindAsString() const;
//
//   IdlType* switchType() const { return m_switchType; }
//   UnionCase::Map& cases() const { return m_cases; }
//   IdlType* thisType()   const { return m_thisType; }
//   bool recursive() const { return m_isRecursive; }
//   bool finished() const { return m_isFinished; }
//
// //   void accept(AstVisitor& visitor) { visitor.visitUnion(this); }
//
// //   void finishConstruction(IdlType* switchType, IDL_Boolean constrType,
// // 			  UnionCase* cases);
//   void setRecursive() { m_isRecursive = true; }
//
// private:
//   std::string m_identifier;
//   IdlType* m_switchType;
//   IdlType* m_thisType;
//
//   Declarator::Map m_cases;
//   bool m_isRecursive;
//   bool m_isFinished;
// };

/*** Other classes that do not inherit Idltype */

// used only to know if the type is composed
// keep it ?
// class TypeSpec {
// public:
//   TypeSpec(IdlType* type, bool constr)
//     : m_type(type), m_constr(constr) {}
//   ~TypeSpec() {}
//
//   IdlType* type() const { return m_type; }
//   bool constr() const { return m_constr; }
//
// private:
//   IdlType* m_type;
//   bool m_constr;
// };

}

}

#endif
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;  replace-tabs off;
