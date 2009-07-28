#ifndef <!upper(comp.name())!>_USERSTRUCT_H
#define <!upper(comp.name())!>_USERSTRUCT_H

<?
for i in comp.nativeTypesIncludes():
  print "#include <" + i + ">"
?>

typedef int return_t;

<?
for t in comp.typesVect():
  if not t.isNative() and t.identifier() and t.identifier() != IDSType.identifier():
    print MapTypeToCpp(t) + ";"
?>

<?
def copyTypeCpp(t):
  if not is_dynamic(t):
    return t
  elif t.kind() == IdlType.Named:
    n = t.asNamedType()
    return NamedType(n.identifier() + "Cpp", copyTypeCpp(n.type()))
  elif t.kind() == IdlType.Typedef:
    ty = t.asTypedefType()
    return TypedefType(copyTypeCpp(ty.aliasType()), t.identifier() + "Cpp")
  elif t.kind() == IdlType.Sequence:
    s = t.asSequenceType()
    return SequenceType(copyTypeCpp(s.seqType()), s.bound())
  elif t.kind() == IdlType.Struct:
    s = t.asStructType()
    res = StructType()
    res.setIdentifier(s.identifier() + "Cpp")
    for m in s.members():
      res.addMember(copyTypeCpp(m.data), m.key)
    return res
  else:
    return t # standard type

#for t in comp.typesVect():
#  if is_dynamic(t):
#    print MapTypeToCpp(copyTypeCpp(t)) + ";\n"
?>

#endif /* <!upper(comp.name())!>_USERSTRUCT_H */
