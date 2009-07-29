#ifndef <!upper(comp.name())!>_USERSTRUCT_H
#define <!upper(comp.name())!>_USERSTRUCT_H

typedef int return_t;

<?
for i in comp.nativeTypesIncludes():
  print "#include <" + i + ">"
?>
<?
#for port in outports:
#  if is_dynamic_port(port):
#    ty = port.idlType.asNamedType()
#    print MapTypeToC(ty.type()) + ";\n"

for t in comp.typesVect():
    if t.identifier() != IDSType.identifier() and not t.isNative():
      print MapTypeToC(t) + ";\n"

for v in comp.valuesMap():
    print MapValueToC(v.data())
?>

#endif /* <!upper(comp.name())!>_STRUCT_H */
