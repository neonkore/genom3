<?
def encodeSimpleType(type, name):
  ?>
    it = YarpCodec<<!type!>>::encode(b,(<!name!>));
    if (it == -1) 
      return -1;
<?

def encodeType(t, name):
  if t.kind() == IdlKind.Short or t.kind() == IdlKind.Long or t.kind() == IdlKind.LongLong or t.kind() == IdlKind.ULong :
    encodeSimpleType("int", name)
  elif t.kind() == IdlKind.Double or t.kind() == IdlKind.Float:
    encodeSimpleType("double", name)
  elif t.kind() == IdlKind.String:
    encodeSimpleType("string", name)
  elif t.kind() == IdlKind.Octet or t.kind() == IdlKind.Char: ?>
    b->add(new yarp::os::Value((void *)(<!name!>), 1));
    ++it;
<?

def decodeSimpleType(type, name):
  ?>
    try{
      it = YarpCodec<<!type!>>::decode(b,<!name!>,it);
    }
    catch(YarpCodecException& e){
      e.add("<!name!>","<!type!>");
      throw(e);
    }
    if (it == -1) 
      return -1;
<?

def decodeType(t, name):
  if t.kind() == IdlKind.Short or t.kind() == IdlKind.Long or t.kind() == IdlKind.LongLong or t.kind() == IdlKind.ULong : 
    decodeSimpleType("int", name)
  elif t.kind() == IdlKind.Double or t.kind() == IdlKind.Float:
    decodeSimpleType("double", name)
  elif t.kind() == IdlKind.String: 
    decodeSimpleType("string", name)
  elif t.kind() == IdlKind.Octet or t.kind() == IdlKind.Char: ?>
    <!name!> = *(b->get(it).asBlob());
    it++;
<?

def printSimpleType(type, name):
  ?>
      std::cout << "<!name!> ";
      YarpCodec<<!type!>>::print(<!name!>);
      std::cout << std::endl; 
<?

def printType(t, name):
  if t.kind() == IdlKind.Short or t.kind() == IdlKind.Long or t.kind() == IdlKind.LongLong or t.kind() == IdlKind.ULong : 
    printSimpleType("int", name)
  elif t.kind() == IdlKind.Double or t.kind() == IdlKind.Float:
    printSimpleType("double", name)
  elif t.kind() == IdlKind.String:
    printSimpleType("string", name)
  elif t.kind() == IdlKind.Octet or t.kind() == IdlKind.Char: ?>
      std::cout << "<!name!>" << " Binary-Non-Printable" << std::endl;
<?
?>

#include "<!comp.name()!>YarpCodec.hpp"

using namespace GenomYarp;
using namespace std;

<?
for t in typesVect:
  typeName = t.identifier()
  ?>
int YarpCodec<<!typeName!>>::encode(yarp::os::Bottle *b,const <!typeName!>& v)
{
    int it = 0;
<?
  flatList = flatStruct(t, "v", ".")
  for x in flatList:
    encodeType(x[0], x[1])
  ?>
    return it;
}

int YarpCodec<<!typeName!>>::decode(const yarp::os::Bottle *b, <!typeName!>& v, int it)
{
<?
  flatList = flatStruct(t, "v", ".")
  for x in flatList:
    decodeType(x[0], x[1])
  ?>
    return it;
}

int YarpCodec<<!typeName!>>::print (const <!typeName!>& v)
{
<?
  flatList = flatStruct(t, "v", ".")
  for x in flatList:
    printType(x[0], x[1])
  ?>
      return 1;
}

<?
?>
