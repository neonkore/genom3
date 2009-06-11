
#ifndef <!upperCompName!>_STRUCT_HPP
#define <!upperCompName!>_STRUCT_HPP

<?
for t in typesVect:
  if t.identifier() != IDSType.identifier():
    print MapTypeToCpp(t) + ";\n"

for v in comp.valuesMap():
    print MapValueToC(v.data())

print MapTypeToCpp(IDSType) + ";\n"
?>

#endif
 
