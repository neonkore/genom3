#include "ExecServices.hpp"

#include "userCodels.h"
#include "<!comp.name()!>Module.hpp"

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
  for i in service.inputs():
    inputStr += ", "
    t = inputType(i)
    inputStr += MapTypeToC(t) + " " + i.identifier

  ?>
// <!service.name!>Service

<!service.name!>Service::<!service.name!>Service(<!comp.name()!>ControlData *data, 
    int id, std::string clientName <!inputStr!>)
: m_data(data), m_id(id), m_clientName(clientName)
{
<?
  for i in service.inputs():
    print "  in_" + i.identifier + " = " + i.identifier + ";"
  ?>
  m_status = <!startStateForService(service)!>;
}

bool <!service.name!>Service::step()
{
  int res;
  switch(m_status) {
<?
  for c in service.codels():
    if c.key() == "control":
      continue
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
      print "  m_data->" + p + "_outport.exportData();"  
    ?>
  return res;
}
<?
?> 
