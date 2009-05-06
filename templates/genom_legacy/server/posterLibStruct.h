/*------------------  Fichier généré automatiquement ------------------*/
/*------------------  Ne pas éditer manuellement !!! ------------------*/

#ifndef <!comp.name()!>_POSTER_LIB_STRUCT_H
#define <!comp.name()!>_POSTER_LIB_STRUCT_H

<?
for p in comp.portsMap():
    port = p.data()
    print port.idlType.toCType() + ";";
    print "typedef " + port.idlType.toCType(True) + " " + upper(comp.name()) + "_" + upper(port.name) + "_POSTER_STR;"
?>

/*-------------------- Fin de chargement du fichier -----------------------*/
#endif
