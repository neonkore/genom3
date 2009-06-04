
#ifndef <!upperCompName!>_STRUCT_HPP
#define <!upperCompName!>_STRUCT_HPP

<?
for t in typesVect:
  if t.identifier() != IDSType.identifier():
    print MapTypeToCpp(t) + ";\n"

print MapTypeToCpp(IDSType) + ";\n"
?>

#endif
 
