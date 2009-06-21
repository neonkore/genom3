#include "ExecServices.hpp"

#include "userCodels.h"
#include "<!comp.name()!>Module.hpp"
#include "lib/RepliesWriter.hpp"

using namespace GenomYarp;

std::string errorString(int err);

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

  serviceInfo = services_info_dict[service.name]

  inputStr = ""
  for i in service.inputs():
    inputStr += ", "
    t = inputType(i)
    inputStr += MapTypeToCpp(t) + " " + i.identifier

  ?>
// <!service.name!>Service

<!service.name!>Service::<!service.name!>Service(<!comp.name()!>ControlData *data, 
    int id, std::string clientName, yarp::os::BufferedPort<yarp::os::Bottle> &replyPort <!inputStr!>)
: m_aborted(false), m_data(data), m_id(id), m_clientName(clientName), m_replyPort(replyPort)
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
	genom_log("Service \"<!service.name!>\" from '%s' with id:%d aborted", m_clientName.c_str(), m_id);
	ReplyWriter<VoidIO>::send(m_replyPort, 
	    m_clientName, m_id, "<!service.name!>", "Aborted", 0);
    } else {
<?
  if serviceInfo.outputFlag: ?>
	genom_log("Service \"<!service.name!>\" from '%s' with id:%d finished", m_clientName.c_str(), m_id);
	ReplyWriter<<!serviceInfo.outputTypeCpp!>>::send(m_replyPort, 
	    m_clientName, m_id, "<!service.name!>", "OK", &out_<!service.output.identifier!>);    
<?
  else:?>
	genom_log("Service \"<!service.name!>\" from '%s' with id:%d finished", m_clientName.c_str(), m_id);
	ReplyWriter<VoidIO>::send(m_replyPort, 
	    m_clientName, m_id, "<!service.name!>", "OK", 0);
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

  switch(m_status) {
<?
  for c in service.codels():
    if c.key() == "control":
      continue
    ?>
    case <!upper(service.name)!>_<!upper(c.key())!>:
      m_status = <!c.key()!>();
      if(m_status < 0) { // error
	string r = "<!service.name!> : " + errorString(m_status);
	cout << r << endl;
	ReplyWriter<VoidIO>::send(m_replyPort, m_clientName, m_id, "<!service.name!>", r, 0);    
	return true;
      }
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
  int res = <!real_codel_call(codel, "m_data->", service, True)!>;

  // update ports, release locks, etc
<?
    codelRelease(codel, service);
    ?>
  return res;
}
<?
?> 
