#ifndef <!upper(comp.name())!>_STRUCT_H
#define <!upper(comp.name())!>_STRUCT_H

<?
for port in outports:
  if isDynamicPort(port):
    print "typedef " + MapTypeToC(port.idlType) + "  " + port.idlType.identifier() + ";\n"
#for t in comp.typesVect():
#    if t.identifier() != IDSType.identifier():
#	print "typedef " + MapTypeToC(t) + "  " + t.identifier() + ";\n"
?>

#endif /* <!upper(comp.name())!>_STRUCT_H */
