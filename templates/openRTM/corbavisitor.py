# -*- coding: utf-8 -*-

class CorbaCppVisitor(TypeVisitor):
  CorbaSuffix = "_Corba"
  IndentQuantum = "  "
  declOnly = False
  isOutType = False
  out = ""
  indent = ""

  def __init__(self, declOnly, isOutType):
    TypeVisitor.__init__(self) # do not forget to call this
    self.out = ""
    self.declOnly = declOnly
    self.isOutType = isOutType
    self.indent = ""

  def visitBaseType(self, t):
    if t.kind() == IdlType.Null:
      self.out += "null"
    elif t.kind() == IdlType.Void:
      self.out += "void"
    elif t.kind() == IdlType.Short:
      self.out += "CORBA::Short"
    elif t.kind() == IdlType.Long:
      self.out += "CORBA::Long"
    elif t.kind() == IdlType.LongLong:
      self.out += "CORBA::LongLong"
    elif t.kind() == IdlType.UShort:
      self.out += "CORBA::UShort"
    elif t.kind() == IdlType.ULong:
      self.out += "CORBA::ULong"
    elif t.kind() == IdlType.ULongLong:
      self.out += "CORBA::ULongLong"
    elif t.kind() == IdlType.Float:
      self.out += "CORBA::Float"
    elif t.kind() == IdlType.Double:
      self.out += "CORBA::Double"
    elif t.kind() == IdlType.LongDouble:
      self.out += "CORBA::LongDouble"
    elif t.kind() == IdlType.Boolean:
      self.out += "CORBA::Boolean"
    elif t.kind() == IdlType.Char:
      self.out += "CORBA::Char"
    elif t.kind() == IdlType.WChar:
      self.out += "CORBA::WChar"
    elif t.kind() == IdlType.Octet:
      self.out += "CORBA::Octet"

  def visitStringType(self, t):
    self.out += "char *"

  def visitStructType(self, s):
    if self.declOnly:
      self.out += s.identifier() + self.CorbaSuffix
      return
    
    self.out += "struct " + s.identifier() + self.CorbaSuffix + "{\n"
    oldIndent = self.indent
    self.indent += self.IndentQuantum

    for m in s.members():
      self.out += self.indent
      if m.data.kind() == IdlType.String:
	self.out += "char " + m.key + "[" + m.data.asStringType().bound() + "];\n"
	continue

      m.data.accept(self)
      self.out += " " + m.key
      if m.data.kind() == IdlType.Array:
	self.out += m.data.asArrayType().printBounds()
      self.out += ";\n"

    self.indent = oldIndent
    self.out += self.indent + "}"


  def visitTypedefType(self, t):
    id = t.identifier()
    if self.declOnly:
      self.out += id + self.CorbaSuffix
      return

    self.out += "typedef "
    m.data.accept(self)
    self.out += " " + id + self.CorbaSuffix

  def visitEnumType(self, e):
    if self.declOnly:
      if self.isOutType:
	self.out += "CORBA::Long"
      else:
	self.out += e.identifier() + self.CorbaSuffix
      return

    self.out += "enum " + e.identifier() + self.CorbaSuffix + " {\n" + self.indent
    first = True
    for x in e.enumerators():
      if first:
	self.out += ", \n" + self.indent + self.IndentQuantum
      else:
	first = False
      self.out += x

    self.out += "}"

  def visitNamedType(self, n):
    oldDeclOnly = self.declOnly
    self.declOnly = True
    n.type().accept(self)
    self.declOnly = oldDeclOnly

  def visitConstValue(self, v):
    self.out += "const "
    v.type().accept(this) 
    self.out += v.identifier() + " = " + v.value().toString() + ";\n"

  def visitType(self, t):
    t.accept(self)

def MapTypeToCorbaCpp(t, declOnly = False, isOutType = False):
  visitor = CorbaCppVisitor(declOnly, isOutType)
  t.accept(visitor)
  return visitor.out

def MapValueToCorbaCpp(v):
  visitor = CorbaCppVisitor(True, False)
  visitor.visitConstValue(v)
  return visitor.out
