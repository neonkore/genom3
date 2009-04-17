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

#include "typevisitor.h"

// Note on Type object memory management
//
// All type objects which can be are statically allocated; all others
// are allocated on the heap. When Decl objects receive a pointer to a
// type object, they should call its shouldDelete() function to see if
// they are responsible for deleting the object. This only returns
// true for those heap-allocated objects which do not have an
// associated Decl object. Type objects created by Decl constructors
// must be deleted by the Decls' destructors.

namespace G3nom
{

class IdlType
{
	public:
		enum Kind {
			Null, Void, Short, Long, LongLong, UShort, ULong, ULongLong,
			Float, Double, LongDouble, Fixed, Boolean, Char, WChar, Octet, String,
			WString, Any, Struct, Union, Enum, Sequence, Typedef
		};
		typedef std::map<std::string,IdlType*> Map;
		typedef std::vector<IdlType*> Vector;

		IdlType(Kind k);
		virtual ~IdlType() {};

		Kind kind() const {
			return m_kind;
		}
		std::string kindAsString() const;

		/// \return an equivalent IdlType object with aliases stripped.
		IdlType* unalias();

		virtual bool shouldDelete() = 0;

		virtual void accept(TypeVisitor& visitor) = 0;

		static void init();

	private:
		Kind m_kind;
};

/* Simple declarator (eg int a;) or arrray declarator (eg int a[10][10])*/
class Declarator
{
	public:
		Declarator(std::string identifier)
				: m_identifier(identifier) {}
		~Declarator() {}

		typedef std::map<std::string, Declarator*> Map;

		const std::string & identifier() const {
			return m_identifier;
		}
		IdlType*   type()  const {
			return m_type;
		}
		bool isArray() const;
		void addBound(int b);
		std::vector<int> & bounds() { return m_bounds; }

	private:
		std::string m_identifier;
		IdlType*    m_type;
		std::vector<int> m_bounds;
};

// typedef std::vector<Declarator*> DeclaratorVect;

class DeclaratorVect : public std::vector<Declarator*> {
	public:
		DeclaratorVect()
		: std::vector<Declarator*>()
		{}
};

typedef std::pair<IdlType*, DeclaratorVect*> TypeDeclarator;

/* Basic Types */

class BaseType : public IdlType
{
	public:
		BaseType(Kind k) : IdlType(k) {}
		virtual ~BaseType() {}

		bool shouldDelete() {
			return false;
		}

		void accept(TypeVisitor& visitor) {
			visitor.visitBaseType(this);
		}

		// Static base type singletons
		static BaseType* nullType;
		static BaseType* voidType;
		static BaseType* shortType;
		static BaseType* longType;
		static BaseType* ushortType;
		static BaseType* ulongType;
		static BaseType* floatType;
		static BaseType* doubleType;
		static BaseType* booleanType;
		static BaseType* charType;
		static BaseType* octetType;
		static BaseType* anyType;
//   static BaseType* TypeCodeType;
//   static BaseType* PrincipalType;
		static BaseType* longlongType;
		static BaseType* ulonglongType;
		static BaseType* longdoubleType;
		static BaseType* wcharType;
};

class StringType : public IdlType
{
	public:

		StringType(int bound) : IdlType(String), m_bound(bound) { }
		virtual ~StringType() {}

		int bound() {
			return m_bound;
		}
		bool shouldDelete() {
			return m_bound > 0;
		}

		void accept(TypeVisitor& visitor) {
			visitor.visitStringType(this);
		}

		static StringType* unboundedStringType;

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
		bool shouldDelete() {
			return m_bound ? 1 : 0;
		}

		void accept(TypeVisitor& visitor) {
			visitor.visitWStringType(this);
		}

		static WStringType* unboundedWStringType;

	private:
		int m_bound;
};

class SequenceType : public IdlType
{
	public:
		SequenceType(IdlType* seqType, int bound) :
				IdlType(Sequence), m_seqType(seqType), m_bound(bound) {}
		virtual ~SequenceType() {}

		IdlType* seqType() {
			return m_seqType;
		}
		int bound() {
			return m_bound;
		}
		bool shouldDelete() {
			return true;
		}

		void accept(TypeVisitor& visitor) {
			visitor.visitSequenceType(this);
		}

	private:
		IdlType*     m_seqType;
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
		bool shouldDelete() {
			return true;
		}

		void accept(TypeVisitor& visitor) {
			visitor.visitFixedType(this);
		}

	private:
		int m_digits;
		int m_scale;
};

/* More complex types*/

class TypedefType : public IdlType
{
	public:
		TypedefType(IdlType* aliasType, DeclaratorVect *declarators)
				: IdlType(Typedef), m_aliasType(aliasType), m_declarators(declarators) {}
		virtual ~TypedefType() {}

		const std::string kindAsString() const;

		IdlType*  aliasType() const {
			return m_aliasType;
		}
		DeclaratorVect* declarators() const {
			return m_declarators;
		}

		void accept(TypeVisitor& visitor) {
			visitor.visitTypedefType(this);
		}
		bool shouldDelete() {
			return true;
		}

	private:
		IdlType* m_aliasType;
		DeclaratorVect *m_declarators;
};

class StructType : public IdlType
{
	public:
		StructType()
				: IdlType(Struct) {}
		virtual ~StructType() {}

		const std::string kindAsString() const;

		std::string identifier() const {
			return m_identifier;
		}
		void setIdentifier(const std::string &id) {
			m_identifier = id;
		}

		void addMember(IdlType *t, DeclaratorVect *declarators);
		const std::vector<TypeDeclarator>& members() const {
			return m_members;
		}

		bool isRecursive() const {
			return m_isRecursive;
		}
		void setRecursive() {
			m_isRecursive = true;
		}

		void accept(TypeVisitor& visitor) {
			visitor.visitStructType(this);
		}
		bool shouldDelete() {
			return true;
		}

	private:
		std::string m_identifier;
		std::vector<TypeDeclarator> m_members;
		bool m_isRecursive;
};

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
		bool shouldDelete() {
			return true;
		}

	private:
		std::string m_identifier;
		std::vector<std::string> m_enum;
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

#endif
// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;
