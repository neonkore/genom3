
#ifndef <!upper(comp.name())!>_PRINTER_H
#define <!upper(comp.name())!>_PRINTER_H

#include <string>

#include "<!comp.name()!>Struct.hpp"

<?
def class_declaration(typeName): 
  ?>	
template<> class Printer<<!typeName!>> {
public:
  static void print (const <!typeName!>& v);
};

<?
?>

template<class T_STRUCT>
class Printer{
public:
  static void  print  (const T_STRUCT& v);
};

<?
class_declaration("int")
class_declaration("unsigned int")
class_declaration("char")
class_declaration("unsigned char")
class_declaration("float")
class_declaration("double")

for t in comp.typesVect():
  if t.identifier() and t.identifier() != IDSType.identifier():
    class_declaration(t.identifier() + "_Corba")
?>

template<> class Printer<std::string> {
public:
  static void print (const std::string& v);
  static void print (const CORBA::String_member& v);
};

#endif