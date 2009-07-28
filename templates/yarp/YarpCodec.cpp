<?
def encode_simple_type(type, name):
  ?>
    it = YarpCodec<<!type!>>::encode(b,<!name!>);
    if (it == -1) 
      return -1;
<?

def encode_type(t, name):
  if t.kind() == IdlType.Named or t.kind() == IdlType.Typedef:
    encode_type(t.unalias(), name)
  elif t.kind() == IdlType.Short or t.kind() == IdlType.Long or t.kind() == IdlType.LongLong:
    encode_simple_type("int", name)
  elif  t.kind() == IdlType.ULong:
    encode_simple_type("unsigned int", name)    
  elif t.kind() == IdlType.Double:
    encode_simple_type("double", name)
  elif t.kind() == IdlType.Float:
    encode_simple_type("float", name)
  elif t.kind() == IdlType.String:
    encode_simple_type("string", name)
  elif t.kind() == IdlType.Octet or t.kind() == IdlType.Char: 
    encode_simple_type("char", name)
  elif t.kind() == IdlType.Enum: ?>
  {
    int tmp = (int) <!name!>;
    it = YarpCodec<int>::encode(b,tmp);
    if (it == -1) 
      return -1;
  }
<?
  elif t.kind() == IdlType.Sequence: 
    s = t.asSequenceType()
    seqType = MapTypeToCpp(s.seqType(), True)
    encode_simple_type("int", name + ".length") 
    if s.seqType().kind() == IdlType.Char: ?>
    b->add(new yarp::os::Value((void*) <!name!>.data, <!name!>.length));
    it++;
<?
    else: ?>
    // data
    for(int j=0; j < <!name!>.length; ++j)
      it = YarpCodec<<!seqType!>>::encode(b, <!name!>.data[j]);
<?

def decode_simple_type(type, name):
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

def decode_type(t, name):
  if t.kind() == IdlType.Named or t.kind() == IdlType.Typedef:
    decode_type(t.unalias(), name)
  elif t.kind() == IdlType.Short or t.kind() == IdlType.Long or t.kind() == IdlType.LongLong: 
    decode_simple_type("int", name)
  elif  t.kind() == IdlType.ULong:
    decode_simple_type("unsigned int", name)    
  elif t.kind() == IdlType.Double:
    decode_simple_type("double", name)
  elif t.kind() == IdlType.Float:
    decode_simple_type("float", name)
  elif t.kind() == IdlType.String: 
    decode_simple_type("string", name)
  elif t.kind() == IdlType.Octet or t.kind() == IdlType.Char:
    decode_simple_type("char", name)
  elif t.kind() == IdlType.Enum: 
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
  elif t.kind() == IdlType.Sequence: 
    s = t.asSequenceType()
    seqType = MapTypeToCpp(s.seqType())
    decode_simple_type("int", name + ".length")
    if s.seqType().kind() == IdlType.Char: ?>
    <!name!>.data = new char[<!name!>.length];
    memcpy(<!name!>.data, b->get(it).asBlob(), <!name!>.length);
    it++;
<?
    else: ?>
    <!name!>.data = new <!seqType!>[<!name!>.length];
    for(int j=0; j < <!name!>.length; ++j)
      it = YarpCodec<<!seqType!>>::decode(b, <!name!>.data[j], it);
<?

def print_simple_type(type, name):
  ?>
      std::cout << "<!name!> ";
      YarpCodec<<!type!>>::print(<!name!>);
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
	case <!x!>: std::cout << "<!x!>"; break;
<?
    ?>
      }
      std::cout << std::endl;
<?
  elif t.kind() == IdlType.Sequence: 
    s = t.asSequenceType()
    seqType = MapTypeToCpp(s.seqType(), True)
    print_simple_type("int", name + ".length")
    ?>
    // data
    for(int j=0; j < <!name!>.length; ++j)
      YarpCodec<<!seqType!>>::print(<!name!>.data[j]);
<?

def free_type(t, name):
  s = t.asSequenceType()
  if t.kind() == IdlType.Sequence:
    seqType = MapTypeToCpp(s.seqType())
    ?>
      delete[] &<!name!>;
      for(int j=0; j < <!name!>.length; ++j)
	  YarpCodec<<!seqType!>>::freeAllocatedMemory(&<!name!>.data[j]);
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
  flatList = flat_struct(t, "v", ".")
  for x in flatList:
    encode_type(x[0], x[1])
  ?>
    return it;
}

int YarpCodec<<!typeName!>>::decode(const yarp::os::Bottle *b, <!typeName!>& v, int it)
{
<?
  flatList = flat_struct(t, "v", ".")
  for x in flatList:
    decode_type(x[0], x[1])
  ?>
    return it;
}

int YarpCodec<<!typeName!>>::print (const <!typeName!>& v)
{
<?
  flatList = flat_struct(t, "v", ".")
  for x in flatList:
    print_type(x[0], x[1])
  ?>
      return 1;
}

void YarpCodec<<!typeName!>>::freeAllocatedMemory(<!typeName!> *v) 
{
<?
  flatList = flat_struct(t, "(*v)", ".")
  for x in flatList:
    free_type(x[0], x[1])
  ?>
}
<?
?>
