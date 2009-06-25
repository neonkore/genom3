#include "<!comp.name()!>Printer.h"

<?
def simpleTypePrintFunction(type):
  ?>
void Printer<<!type!>>::print(const <!type!>& v)
{
    std::cout << v;
}

<?
simpleTypePrintFunction("int")
simpleTypePrintFunction("unsigned int")
simpleTypePrintFunction("char")
simpleTypePrintFunction("float")
simpleTypePrintFunction("double")
simpleTypePrintFunction("std::string")
?>

<?
def printSimpleType(type, name):
  ?>
      std::cout << "<!name!> ";
      Printer<<!type!>>::print(<!name!>);
      std::cout << std::endl; 
<?

def printType(t, name):
  if t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
    printType(t.unalias(), name)
  elif t.kind() == IdlKind.Short or t.kind() == IdlKind.Long or t.kind() == IdlKind.LongLong: 
    printSimpleType("int", name)
  elif  t.kind() == IdlKind.ULong:
    printSimpleType("unsigned int", name)    
  elif t.kind() == IdlKind.Double:
    printSimpleType("double", name)
  elif t.kind() == IdlKind.Float:
    printSimpleType("float", name)
  elif t.kind() == IdlKind.String:
    printSimpleType("string", name)
  elif t.kind() == IdlKind.Octet or t.kind() == IdlKind.Char: 
    printSimpleType("char", name)
  elif t.kind() == IdlKind.Enum: 
    e = t.asEnumType()
    ?>
      std::cout << "<!name!> =";
      switch(<!name!>) {
<?
    for x in e.enumerators():?>
	case <!x!>: std::cout << "<!x!>"; break;
<?
    ?>
      }
      std::cout << std::endl;
<?
  elif t.kind() == IdlKind.Sequence: 
    s = t.asSequenceType()
    seqType = MapTypeToCpp(s.seqType(), True)
    printSimpleType("int", name + ".length")
    ?>
    // data
    for(int j=0; j < <!name!>.length; ++j)
      YarpCodec<<!seqType!>>::print(<!name!>.data[j]);
<?
?>

<?
for t in comp.typesVect():
  if not t.identifier():
    continue
  typeName = t.identifier()
  ?>
int Printer<<!typeName!>>::print (const <!typeName!>& v)
{
<?
  flatList = flatStruct(t, "v", ".")
  for x in flatList:
    printType(x[0], x[1])
  ?>
}
