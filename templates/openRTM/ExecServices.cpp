#include "ExecServices.h"

#include "userCodels.h"

// forward declaration of user codels
<?
for s in servicesMap:
  service = s.data()
  if service.type == ServiceType.Exec:
    for c in service.codels():
      if c.key() != "control":
	print "int " + real_codel_signature(c.data(), service) + ";"
?>

<?
for s in comp.servicesMap():
  service = s.data()
  if service.type == ServiceType.Control:
    continue

  inputStr = ""
  first = True
  for s in service.inputs():
    if first:
      first = False
    else:
      inputStr += ", "
    t = comp.typeFromIdsName(s)
    inputStr += MapTypeToCpp(t) + " " + s

  ?>
// <!service.name!>Service

<!service.name!>Service::<!service.name!>Service(<!capCompName!>ControlData *data, <!inputStr!>)
: m_data(data)
{
<?
  for s in service.inputs():
    print "  m_" + s + " = " + s + ";"
  ?>
  m_status = <!startStateForService(service)!>;
}

bool <!service.name!>Service::step()
{
  int res;
  switch(m_status) {
<?
  for c in service.codels():
    ?>
    case <!upper(service.name)!>_<!upper(c.key())!>:
      m_status = <!c.key()!>();
      return true;
<?
  ?>
    case GOTO_ETHER:
      return false;
  }
  return true;
}

<?
  for c in service.codels():
    if c.key() == "control":
      continue
    codel = c.data()
    ?>
int <!service.name!>Service::<!c.key()!>()
{
  // call the user codel
  int res = <!real_codel_call(codel, "m_data->", service)!>;

  // update ports
<?
    for p in codel.outPorts:
      print "  outport_" + p + "->write();"  
    ?>
  return res;
}
<?
?> 
