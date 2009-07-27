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
	print real_codel_signature(c.data(), service) + ";"

for port in outports:
  if is_dynamic(port.idlType):
    print size_codel_signature(port) + ";"
?>
}

<?
for s in comp.servicesMap():
  service = s.data()
  if service.type == ServiceType.Control:
    continue

  inputStr = ", int id"
  for i in service.inputs():
    inputStr += ", "
    t = input_type(i)
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
  m_status = <!start_state_for_service(service)!>;
}

<!service.name!>Service::~<!service.name!>Service()
{
    <!service.name!>OutStruct s;
    s.id = m_id;

    // send the reply
    if(m_aborted) {
	genom_log("Service \"<!service.name!>\" with id:%d aborted", m_id);
	  s.res = SERVICE_ABORTED;
    } else {
	if(m_status < 0) { // error
	  genom_log("Service \"<!service.name!>\" with id:%d finished with error %d", m_id, s.res);
	  s.res = m_status;
	} else {
	  s.res = USER_OK;
	  genom_log("Service \"<!service.name!>\" with id:%d finished successfully", m_id);

<?
  if service.output.identifier:
    outputMember = "out_" + service.output.identifier
    copyTypeFromCorba(input_type(service.output), outputMember, "s.data", True)
  ?>
	}
    }
    m_data-><!service.name!>_outport.write(s);
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
    codel_lock(codel, service)
    ?>
  // call the user codel
  int res = <!real_codel_call(codel, "m_data->", service, False)!>;

  // update ports, release locks, etc
<?
    codel_release(codel, service);
    ?>
  return res;
}
<?
?> 
