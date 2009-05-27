/*------------------  Fichier généré automatiquement ------------------*/
/*------------------  Ne pas éditer manuellement !!! ------------------*/

#ifndef <!comp.name()!>_POSTER_LIB_STRUCT_H
#define <!comp.name()!>_POSTER_LIB_STRUCT_H

<?
for p in comp.portsMap():
    port = p.data()
    if isDynamicPort(port):
      t = dynamicPortType(port)
#       print MapTypeToC(t) + ";";
      print "typedef " + MapTypeToC(t, True) + " " + upper(comp.name()) + "_" + upper(port.name) + "_POSTER_STR;"      
    else:
#      print MapTypeToC(port.idlType) + ";";
      print "typedef " + MapTypeToC(port.idlType, True) + " " + upper(comp.name()) + "_" + upper(port.name) + "_POSTER_STR;"
?>

/*-------------------- Fin de chargement du fichier -----------------------*/
#endif
