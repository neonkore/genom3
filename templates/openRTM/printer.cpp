#include "<!comp.name()!>Printer.h"

#include <iostream>

using namespace std;

<?
def simple_type_print_function(type):
  ?>
void Printer<<!type!>>::print(const <!type!>& v)
{
    std::cout << v;
}

<?
simple_type_print_function("int")
simple_type_print_function("unsigned int")
simple_type_print_function("char")
simple_type_print_function("unsigned char")
simple_type_print_function("float")
simple_type_print_function("double")
simple_type_print_function("std::string")
?>

void Printer<string>::print(const CORBA::String_member& v)
{
  std::cout << v;
}

<?
def print_simple_type(type, name):
  ?>
      std::cout << "<!name!> ";
      Printer<<!type!>>::print(<!name!>);
      std::cout << std::endl; 
<?

def print_type(t, name):
  if t.kind() == IdlType.Named or t.kind() == IdlType.Typedef:
    print_type(t.unalias(), name)
  elif t.kind() == IdlType.Short or t.kind() == IdlType.Long or t.kind() == IdlType.LongLong: 
    print_simple_type("int", name)
  elif  t.kind() == IdlType.ULong:
    print_simple_type("unsigned int", name)    
  elif t.kind() == IdlType.Double:
    print_simple_type("double", name)
  elif t.kind() == IdlType.Float:
    print_simple_type("float", name)
  elif t.kind() == IdlType.String:
    print_simple_type("string", name)
  elif t.kind() == IdlType.Octet or t.kind() == IdlType.Char: 
    print_simple_type("char", name)
  elif t.kind() == IdlType.Enum: 
    e = t.asEnumType()
    ?>
      std::cout << "<!name!> =";
      switch(<!name!>) {
<?
    for x in e.enumerators():?>
	case <!x!>_Corba: std::cout << "<!x!>"; break;
<?
    ?>
      }
      std::cout << std::endl;
<?
  elif t.kind() == IdlType.Sequence: 
    s = t.asSequenceType()
    seqType = MapTypeToCorbaCpp(s.seqType(), True)
    print_simple_type("int", name + ".length()")
    ?>
    // data
    for(int j=0; j < <!name!>.length(); ++j)
      Printer<<!seqType!>>::print(<!name!>[j]);
<?
?>

<?
for t in comp.typesVect():
  if not t.identifier() or t.identifier() == IDSType.identifier():
    continue
  typeName = t.identifier() + "_Corba"
  ?>
void Printer<<!typeName!>>::print (const <!typeName!>& v)
{
<?
  flatList = flat_struct(t, "v", ".")
  for x in flatList:
    print_type(x[0], x[1])
  ?>
}
