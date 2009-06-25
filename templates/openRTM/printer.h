
#ifndef <!upper(comp.name())!>_STRUCT_H
#define <!upper(comp.name())!>_STRUCT_H

<?
def classDeclaration(typeName): 
  ?>	
template<> class Printer<<!typeName!>> {
public:
  static void print (const <!typeName!>& v);
};

<?
classDeclaration("int")
classDeclaration("unsigned int")
classDeclaration("char")
classDeclaration("float")
classDeclaration("double")
classDeclaration("std::string")

for t in comp.typesVect():
  classDeclaration(t.identifier() + "_Corba")
?>

#endif