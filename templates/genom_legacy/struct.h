#ifndef <!upper(comp.name())!>_STRUCT_H
#define <!upper(comp.name())!>_STRUCT_H

<?
for t in comp.typesVect():
    if t.identifier() != comp.IDSType.identifier():
	print "typedef " + t.toCType() + "  " + t.identifier() + ";\n"
?>

#endif /* <!upper(comp.name())!>_STRUCT_H */
