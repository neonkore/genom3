#include "ExecServices.h"

#include "userCodels.h"
#include "<!comp.name()!>Control.h"

// forward declaration of user codels
extern "C" {
<?
for s in servicesMap:
  service = s.data()
  if service.type != ServiceType.Control:
    for c in service.codels():
      if c.key() != "control":
	print "int " + real_codel_signature(c.data(), service) + ";"

for port in outports:
  if isDynamic(port.idlType):
    print sizeCodelSignature(port) + ";"
?>
}

<?
for s in comp.servicesMap():
  service = s.data()
  if service.type == ServiceType.Control:
    continue

  if service.output.identifier:
    inputStr = ", int id"
  else:
    inputStr = ""
  for i in service.inputs():
    inputStr += ", "
    t = inputType(i)
    inputStr += MapTypeToCpp(t) + " " + i.identifier

  ?>
// <!service.name!>Service

<!service.name!>Service::<!service.name!>Service(<!capCompName!>ControlData *data <!inputStr!>)
:  m_aborted(false), m_data(data)
{
<?
  for i in service.inputs():
    print "  in_" + i.identifier + " = " + i.identifier + ";"
  ?>
  m_status = <!startStateForService(service)!>;
}

<!service.name!>Service::~<!service.name!>Service()
{
    // send the reply
    if(m_aborted) {
	genom_log("Service \"<!service.name!>\" with id:%d aborted", m_id);
    } else {
	genom_log("Service \"<!service.name!>\" with id:%d finished", m_id);
<?
  if service.output.identifier: 
    outputMember = "out_" + service.output.identifier
    ?>
      // get the output result
      <!service.name!>OutStruct s;
      s.id = m_id;
<?
    copyTypeFromCorba(inputType(service.output), outputMember, "s.data", True)
    ?>
      m_data-><!service.name!>_outport.write(s);
<?
  ?>
  }
}

void <!service.name!>Service::abort()
{
    m_aborted = true;
}

bool <!service.name!>Service::step()
{
  if(m_aborted)
    return false;

  int res;
  switch(m_status) {
<?
  for c in service.codels():
    if c.key() == "control":
      continue
    ?>
    case <!upper(service.name)!>_<!upper(c.key())!>:
      m_status = <!c.key()!>();
      if(m_status < 0) // error
	return false;
      return true;
<?
  ?>
    case <!upper(service.name)!>_ETHER:
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
<?
    codelLock(codel, service)
    ?>
  // call the user codel
  int res = <!real_codel_call(codel, "m_data->", service, False)!>;

  // update ports, release locks, etc
<?
    codelRelease(codel, service);
    ?>
  return res;
}
<?
?> 
