<?
from string import upper
# create a list of out ports
outports = []
for p in comp.portsMap():
    if p.data().type == PortType.Outgoing:
	outports.append(p.data())

?>
/*------------------  Fichier généré automatiquement ------------------*/
/*------------------  Ne pas éditer manuellement !!! ------------------*/

#ifndef <!comp.name()!>_POSTER_LIB_STRUCT_H
#define <!comp.name()!>_POSTER_LIB_STRUCT_H

<?
for p in outports:
    print p.idlType.toCType() + ";";
    print "typedef " + p.idlType.toCType(True) + " " + upper(comp.name()) + "_" + upper(p.name) + "_POSTER_STR;"
?>

/*-------------------- Fin de chargement du fichier -----------------------*/
#endif
