#ifndef <!upper(comp.name())!>_STRUCT_H
#define <!upper(comp.name())!>_STRUCT_H

#include "ControlTaskSkel.h"
#include "<!comp.name()!>UserStruct.h"

<?
#for t in comp.typesVect():
#  if t.identifier() and t.identifier() != IDSType.identifier() and not t.isNative():
#    print "typedef " + t.identifier() + "_Corba " + t.identifier() + ";"
?>

<?
for t in comp.typesVect():
  if needsConversionFun(t) and t.identifier() != IDSType.identifier():
    cppType = MapTypeToCpp(t, True)
    corbaType = MapTypeToCorbaCpp(t, True)

    print "void convertFromCorba_" + t.identifier() + "(const " + corbaType + "*in, " + cppType + "* out);"
    print "void convertFromCorbaReverse_" + t.identifier() + "(const " + cppType + "*in, " + corbaType + "* out);"
?>

#endif /* <!upper(comp.name())!>_STRUCT_H */
