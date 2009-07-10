
#ifndef <!upperCompName!>_STRUCT_HPP
#define <!upperCompName!>_STRUCT_HPP

<?
for i in comp.nativeTypesIncludes():
  print "#include <" + i + ">"
?>

<?
for t in typesVect:
  if t.identifier() != IDSType.identifier() and not t.isNative():
    print MapTypeToCpp(t) + ";\n"

for v in comp.valuesMap():
    print MapValueToC(v.data())

print MapTypeToCpp(IDSType) + ";\n"
?>

#endif
 
