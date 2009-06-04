<?
def encodeSimpleType(type, name):
  ?>
    it = YarpCodec<<!type!>>::encode(b,<!name!>);
    if (it == -1) 
      return -1;
<?

def encodeType(t, name):
  if t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
    encodeType(t.unalias(), name)
  elif t.kind() == IdlKind.Short or t.kind() == IdlKind.Long or t.kind() == IdlKind.LongLong or t.kind() == IdlKind.ULong :
    encodeSimpleType("int", name)
  elif t.kind() == IdlKind.Double or t.kind() == IdlKind.Float:
    encodeSimpleType("double", name)
  elif t.kind() == IdlKind.String:
    encodeSimpleType("string", name)
  elif t.kind() == IdlKind.Octet or t.kind() == IdlKind.Char: 
    encodeSimpleType("char", name)
  elif t.kind() == IdlKind.Enum: ?>
  {
    int tmp = (int) <!name!>;
    it = YarpCodec<int>::encode(b,tmp);
    if (it == -1) 
      return -1;
  }
<?
  elif t.kind() == IdlKind.Sequence: 
    s = t.asSequenceType()
    seqType = MapTypeToC(s.seqType(), True)
    encodeSimpleType("int", name + ".length") 
    ?>
    // data
    for(int j=0; j < <!name!>.length; ++j)
      it = YarpCodec<<!seqType!>>::encode(b, <!name!>.data[j]);
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
  if t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
    decodeType(t.unalias(), name)
  elif t.kind() == IdlKind.Short or t.kind() == IdlKind.Long or t.kind() == IdlKind.LongLong or t.kind() == IdlKind.ULong : 
    decodeSimpleType("int", name)
  elif t.kind() == IdlKind.Double or t.kind() == IdlKind.Float:
    decodeSimpleType("double", name)
  elif t.kind() == IdlKind.String: 
    decodeSimpleType("string", name)
  elif t.kind() == IdlKind.Octet or t.kind() == IdlKind.Char:
    decodeSimpleType("char", name)
  elif t.kind() == IdlKind.Enum: 
    e = t.asEnumType()
    ?>
  {
    int tmp;
    try{
      it = YarpCodec<int>::decode(b,tmp,it);
      switch(tmp) {
<?
    for x in e.enumerators():?>
	case <!x!>: <!name!> = <!x!>; break;
<?
    ?>
	default:
	  throw YarpCodecException();
      }
    }
    catch(YarpCodecException& e){
      e.add("<!name!>","<!type!>");
      throw(e);
    }
    if (it == -1) 
      return -1;
  }
<?
  elif t.kind() == IdlKind.Sequence: 
    s = t.asSequenceType()
    seqType = MapTypeToC(s.seqType())
    decodeSimpleType("int", name + ".length")
    ?>
    // data
    if(<!name!>.data)
      delete[] <!name!>.data;
    <!name!>.data = new <!seqType!>[v.length];
    for(int j=0; j < <!name!>.length; ++j)
      it = YarpCodec<<!seqType!>>::decode(b, <!name!>.data[j], it);
<?

def printSimpleType(type, name):
  ?>
      std::cout << "<!name!> ";
      YarpCodec<<!type!>>::print(<!name!>);
      std::cout << std::endl; 
<?

def printType(t, name):
  if t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
    printType(t.unalias(), name)
  elif t.kind() == IdlKind.Short or t.kind() == IdlKind.Long or t.kind() == IdlKind.LongLong or t.kind() == IdlKind.ULong : 
    printSimpleType("int", name)
  elif t.kind() == IdlKind.Double or t.kind() == IdlKind.Float:
    printSimpleType("double", name)
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
    seqType = MapTypeToC(s.seqType(), True)
    printSimpleType("int", name + ".length")
    ?>
    // data
    for(int j=0; j < <!name!>.length; ++j)
      YarpCodec<<!seqType!>>::print(<!name!>.data[j]);
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
