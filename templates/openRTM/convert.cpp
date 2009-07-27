#include "<!comp.name()!>Struct.hpp"

<?
def copy_type_from_corba(t, src, out, reverse):
  if t.kind() == IdlKind.Typedef or t.kind() == IdlKind.Named:
    copy_type_from_corba(t.unalias(), src, out, reverse)
  elif t.kind() == IdlKind.Enum:
    e = t.asEnumType()
    ?>
  switch(<!src!>) {
<?
    for x in e.enumerators(): 
      if reverse:?>
    case <!x!> : <!out!> = <!x!>_Corba; break;
<?
      else: ?>
    case <!x!>_Corba : <!out!> = <!x!>; break;
<?
    ?>
  }
<?
  elif t.kind() == IdlKind.Struct:
    s = t.asStructType()
    if reverse:
      convertFun = "convertFromCorbaReverse"
    else:
      convertFun = "convertFromCorba"
    for m in s.members(): 
      if m.data.kind() == IdlKind.Array: 
	a = m.data.asArrayType()
	?>
  for(int i=0; i < <!a.bounds()[0]!>; ++i)
<?
	if needs_conversion_fun(a.type()): ?>
    <!convertFun!>_<!a.type().identifier()!>(&<!src!>.<!m.key!>[i], &<!out!>.<!m.key!>[i]);
<?
	else: ?>
    <!out!>.<!m.key!>[i] = <!src!>.<!m.key!>[i];
<?
      elif m.data.kind() == IdlKind.String: 
	s = m.data.asStringType()
	if reverse: ?>
    <!out!>.<!m.key!> = CORBA::string_dup(<!src!>.<!m.key!>);
<?
	else:?>
    strncpy(<!out!>.<!m.key!>, <!src!>.<!m.key!>, <!s.bound()!>);
<?
      elif needs_conversion_fun(m.data): ?>
  <!convertFun!>_<!m.data.identifier()!>(&<!src!>.<!m.key!>, &<!out!>.<!m.key!>);
<?
      else: ?>
  <!out!>.<!m.key!> = <!src!>.<!m.key!>;
<?
  elif t.kind() == IdlKind.String:
    s = t.asStringType()
    if reverse: ?>
    <!out!> = CORBA::string_dup(<!src!>);
<?
    else: ?>
    strncpy(<!out!>, <!src!>, <!s.bound()!>);
<?
  else: ?>
  <!out!> = <!src!>;
<?

def convert_from_corba(t, reverse = False):
  cppType = MapTypeToCpp(t, True)
  corbaType = MapTypeToCorbaCpp(t, True)
  if reverse: ?>
void convertFromCorbaReverse_<!t.identifier()!>(const <!cppType!> *in, <!corbaType!> *out)
<?
  else: ?>
void convertFromCorba_<!t.identifier()!>(const <!corbaType!> *in, <!cppType!> *out)
<?
  ?>
{
<?
  copy_type_from_corba(t, "(*in)", "(*out)", reverse)
  ?>
}

<?
for t in comp.typesVect():
  if needs_conversion_fun(t) and t.identifier() != IDSType.identifier() and not is_dynamic(t):
    convert_from_corba(t)
    convert_from_corba(t, True)
?>
