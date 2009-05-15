#ifndef <!upper(comp.name())!>_STRUCT_H
#define <!upper(comp.name())!>_STRUCT_H

<?
for t in comp.typesVect():
    if t.identifier() != IDSType.identifier():
	print "typedef " + MapTypeToC(t) + "  " + t.identifier() + ";\n"
for v in comp.valuesMap():
    print MapValueToC(v.data())
?>

#endif /* <!upper(comp.name())!>_STRUCT_H */
