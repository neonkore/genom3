// -*- c++ -*-
//                          Package   : omniidl
// idltype.h                Created on: 1999/10/18
//			    Author    : Duncan Grisby (dpg1)
//
//    Copyright (C) 1999 AT&T Laboratories Cambridge
//
//  This file is part of omniidl.
//
//  omniidl is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.
//
// Description:
//   
//   Type objects

#ifndef G3NOM_DRIVER_H
#define G3NOM_DRIVER_H

// #include <idlutil.h>
// #include <idlvisitor.h>
// #include <idlscope.h>
#include <string>
#include <vector>
#include <list>
#include <map>

// Note on Type object memory management
//
// All type objects which can be are statically allocated; all others
// are allocated on the heap. When Decl objects receive a pointer to a
// type object, they should call its shouldDelete() function to see if
// they are responsible for deleting the object. This only returns
// true for those heap-allocated objects which do not have an
// associated Decl object. Type objects created by Decl constructors
// must be deleted by the Decls' destructors.

namespace G3nom {
namespace Idl {

class IdlType {
public:

  enum Kind {
    Null, Void, Short, Long, LongLong, UShort, ULong, ULongLong,
    Float, Double, LongDouble, Fixed, Boolean, Char, WChar,Octet, String,
    WString, Any, Struct, Union, Enum, Sequence, Typedef, Struct
  };

//     tk_TypeCode           = 12,
//     tk_Principal          = 13,
//     tk_objref             = 14,
//     tk_array              = 20,
//     tk_alias              = 21,
//     tk_except             = 22,
//    
//     tk_fixed              = 28,
//     tk_value              = 29,
//     tk_value_box          = 30,
//     tk_native             = 31,
//     tk_abstract_interface = 32,
//     tk_local_interface    = 33,
//   };

  IdlType(Kind k);
  virtual ~IdlType() {};

  Kind kind() const { return m_kind; }
  const std::string kindAsString() const;

  bool local() const { return m_local; }
  void setLocal() { m_local = true; }
  // True if this is a "local" type which must not be transmitted
  // across the network.

  IdlType* unalias();
  // Return an equivalent IdlType object with aliases stripped.

  virtual bool shouldDelete() = 0;

//   virtual void accept(TypeVisitor& visitor) = 0;

  // Find a type given a name. Marks the name used in current scope
//   static IdlType* scopedNameToType(const char* file, int line,
// 				   const ScopedName* sn);
  static void init();

private:
  Kind m_kind;
  bool m_local;
};


class BaseType : public IdlType {
public:
  BaseType(Kind k) : IdlType(k) {}
  virtual ~BaseType() {}

  bool shouldDelete() { return 0; }

//   void accept(TypeVisitor& visitor) { visitor.visitBaseType(this); }

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


//
// Strings can be used like base types without a declaration. eg:
//
//   void op(in string<10> s);
//
// therefore, the String type must include its bound here, rather than
// relying on looking at the corresponding declaration
//

class StringType : public IdlType {
public:

  StringType(int bound) : IdlType(String), m_bound(bound) { }
  virtual ~StringType() {}

  int   bound()        { return m_bound; }
  bool shouldDelete() { return m_bound ? 1 : 0; }

//   void accept(TypeVisitor& visitor) { visitor.visitStringType(this); }

  static StringType* unboundedStringType;

private:
  int m_bound;
};


class WStringType : public IdlType {
public:
  WStringType(int bound) : IdlType(WString), m_bound(bound) { }
  virtual ~WStringType() {}

  int   bound()        { return m_bound; }
  bool shouldDelete() { return m_bound ? 1 : 0; }

//   void accept(TypeVisitor& visitor) { visitor.visitWStringType(this); }

  static WStringType* unboundedWStringType;

private:
  int m_bound;
};

class SequenceType : public IdlType {
public:
  SequenceType(IdlType* seqType, int bound) :
    IdlType(Sequence), m_seqType(seqType), m_bound(bound)
  {
      if (seqType && seqType->local()) 
	  setLocal();
  }
  virtual ~SequenceType() {}

  IdlType* seqType() { return m_seqType; }
  int bound() { return m_bound; }
  bool shouldDelete() { return true; }

//   void accept(TypeVisitor& visitor) { visitor.visitSequenceType(this); }

private:
  IdlType*     m_seqType;
  int m_bound;
};

//
// Same goes for fixed
//

class FixedType : public IdlType {
public:
  FixedType(int digits, int scale) :
    IdlType(Fixed), m_digits(digits), m_scale(scale) { }

  virtual ~FixedType() {}

  int digits() { return m_digits; }
  int scale() { return m_scale; }
  bool shouldDelete() { return true; }

//   void accept(TypeVisitor& visitor) { visitor.visitFixedType(this); }

private:
  int m_digits;
  int m_scale;
};


//
// All other types must be declared, at least implicitly, so they have
// an associated declaration object
//

// class Decl;
// class DeclRepoId;
// 
// class DeclaredType : public IdlType {
// public:
//   DeclaredType(Kind k, Decl* decl, DeclRepoId* declRepoId)
//     : IdlType(k), m_decl(decl), m_declRepoId(declRepoId) {}
// 
//   virtual ~DeclaredType() {}
// 
//   Decl* decl() { return m_decl; }
// //   DeclRepoId* declRepoId() { return m_declRepoId; }
// 
//   bool shouldDelete() { return 0; }
// 
// //   void accept(TypeVisitor& visitor) { visitor.visitDeclaredType(this); }
// 
// //   static DeclaredType* corbaObjectType;
// 
// private:
//   Decl*       m_decl;
//   DeclRepoId* m_declRepoId;
// };

class TypedefType : public IdlType {
public:
  TypedefType(IdlType* aliasType, bool constrType,
	  Declarator* declarators)
  : IdlType(Typedef), m_isConstrType(constrType)
  {}

  virtual ~TypedefType();

  const std::string kindAsString() const;

  // Queries
  IdlType*       aliasType()   const { return m_aliasType; }
  bool    constrType()  const { return m_isConstrType; }
  Declarator*    declarators() const { return m_declarators; }

//   void accept(AstVisitor& visitor) { visitor.visitTypedef(this); }

private:
  IdlType* m_aliasType;
//   IDL_Boolean    delType_;
  bool m_isConstrType;
  Declarator* m_declarators;
};

class Member {
public:
  typedef std::map<std::string, Member> Map;

  Member(IdlType* memberType, bool constrType,
	 Declarator* declarators);
  virtual ~Member();

//   const char* kindAsString() const { return "member"; }

  // Queries
  IdlType* memberType()  const { return m_memberType; }
  bool constrType() const { return m_constrType; }
  Declarator*  declarators() const { return m_declarators; }

//   void accept(AstVisitor& visitor) { visitor.visitMember(this); }

private:
  IdlType* m_memberType;
//   IDL_Boolean    delType_;
  bool m_constrType;
  Declarator* m_declarators;
};

class StructType : public IdlType {
public:
  StructType(const std::string &identifier)
  : IdlType(Struct), m_identifier(identifier)
  {}
  virtual ~Struct();

  const std::string kindAsString() const;

  // Queries
  Member::Map& members() const { return m_members; }
  IdlType* thisType() const { return m_thisType; }
  bool isRecursive() const { return m_isRecursive; }
  bool isFinished() const { return m_isFinished; }

//   void accept(AstVisitor& visitor) { visitor.visitStruct(this); }

//   void finishConstruction(Member* members);

  void setRecursive() { m_isRecursive = true; }

private:
  std::string m_identifier;
  Member::Map m_members;
  IdlType* m_thisType;
  bool m_isRecursive;
  bool m_isFinished;
};

class UnionCase {
public:
  typedef std::map<std::string, Member> Map;

  UnionCase(IdlType* memberType, bool constrType,
	 Declarator* declarators);
  virtual ~UnionCase();

//   const char* kindAsString() const { return "member"; }

  // Queries
  IdlType* caseType()  const { return m_caseType; }
  bool constrType() const { return m_constrType; }
  Declarator*  declarators() const { return m_declarators; }

//   void accept(AstVisitor& visitor) { visitor.visitMember(this); }

private:
  IdlType* m_caseType;
//   IDL_Boolean    delType_;
  bool m_constrType;
  Declarator* m_declarators;
};

class UnionType : public IdlType {
public:
  UnionType(const std::string &identifier)
  : IdlType(Union), m_identifier(identifier)
  {}
  virtual ~UnionType();

  const std::string kindAsString() const;

  IdlType* switchType() const { return m_switchType; }
  bool isConstrType() const { return m_isConstrType; }
  UnionCase::Map& cases() const { return m_cases; }
  IdlType* thisType()   const { return m_thisType; }
  bool recursive() const { return m_isRecursive; }
  bool finished() const { return m_isFinished; }

//   void accept(AstVisitor& visitor) { visitor.visitUnion(this); }

//   void finishConstruction(IdlType* switchType, IDL_Boolean constrType,
// 			  UnionCase* cases);
  void setRecursive() { m_isRecursive = true; }

private:
  std::string m_identifier;
  IdlType* m_switchType;
  IdlType* m_thisType;

  bool m_isConstrType;
  UnionCase::Map m_cases;
  bool m_isRecursive;
  bool m_isFinished;
};

/*** Other classes that do not inherit Idltype */

// used only to know if the type is composed
// keep it ?
class TypeSpec {
public:
  TypeSpec(IdlType* type, bool constr)
    : m_type(type), m_constr(constr) {}
  ~TypeSpec() {}

  IdlType* type() const { return m_type; }
  bool constr() const { return m_constr; }

private:
  IdlType* m_type;
  bool m_constr;
};

class Declarator {
public:
  Declarator(std::string identifier, IdlType *type)
  : m_identifier(identifier), m_thisType(type)
  {}

  virtual ~Declarator();

  IdlType*   thisType()  const { return m_thisType; }

private:
  std::string m_identifier;
  IdlType*    m_thisType;
};


}
}

#endif 
