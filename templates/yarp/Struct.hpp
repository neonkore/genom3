
#ifndef <!upperCompName!>_STRUCT_HPP
#define <!upperCompName!>_STRUCT_HPP

<?
for t in typesVect:
  if t.identifier() != IDSType.identifier():
    print MapTypeToC(t) + ";\n"

print MapTypeToC(IDSType) + ";\n"
?>

#endif
 
