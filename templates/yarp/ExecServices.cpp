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

  serviceInfo = services_info_dict[service.name]
  eventsList = comp.eventsForService(service.name)

  inputStr = ""
  for i in service.inputs():
    inputStr += ", "
    t = input_type(i)
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
  m_status.push_front(<!start_state_for_service(service)!>);

  // connect events if necessary
<?
  for e in service.events():
    ev = e.key()
    codel = e.data().replace(".", "_")
    if ev.kind() == EventKind.PortEv:
      pev = ev.asPortEvent()
      port = comp.port(pev.portName())

      if port.type == PortType.Outgoing:
	portName = port.name + "_outport"
      else:
	portName = port.name + "_inport"
      print "  m_data->" + portName + ".registerReceiver(\"" + pev.kindAsString() + "\", this);"

    elif ev.kind() == EventKind.NamedEv:
      print "  m_data->events_inport.registerReceiver(\"" + ev.identifier() + "\", this);"

  if eventsList:?>
  m_eventsSender.setName("<!service.name!>");
<?
  for ev in eventsList: 
    evName = codel_to_event_name(ev)
    ?>
  m_eventsSender.registerReceiver("<!evName!>", &m_data->events_outport);
<?
  ?>
}

<!service.name!>Service::~<!service.name!>Service()
{
  // disconnect events if necessary
<?
  for e in service.events():
    ev = e.key()
    codel = e.data().replace(".", "_")
    if ev.kind() == EventKind.PortEv:
      pev = ev.asPortEvent()
      port = comp.port(pev.portName())

      if port.type == PortType.Outgoing:
	portName = port.name + "_outport"
      else:
	portName = port.name + "_inport"
      print "  m_data->" + portName + ".unregisterReceiver(\"" + pev.kindAsString() + "\", this);"
    elif ev.kind() == EventKind.NamedEv:
      print "  m_data->events_inport.unregisterReceiver(\"" + ev.identifier() + "\", this);"
  ?>

    // send the reply
    if(m_aborted) {
	genom_log("Service \"<!service.name!>\" from '%s' with id:%d aborted", m_clientName.c_str(), m_id);
	ReplyWriter<VoidIO>::send(m_replyPort, 
	    m_clientName, m_id, "<!service.name!>", "Aborted", 0);
    } else {
<?
  if serviceInfo.outputFlag: ?>
    if(m_status.empty()) {
      genom_log("Service \"<!service.name!>\" from '%s' with id:%d finished successfully", m_clientName.c_str(), m_id);
      ReplyWriter<<!serviceInfo.outputTypeCpp!>>::send(m_replyPort, 
	    m_clientName, m_id, "<!service.name!>", "OK", &out_<!service.output.identifier!>);    
    } else {
      std::string r = errorString(m_status.front());      
      genom_log("Service \"<!service.name!>\" from '%s' with id:%d finished with errror: %s", m_clientName.c_str(), m_id, r.c_str());
      ReplyWriter<VoidIO>::send(m_replyPort, m_clientName, m_id, "<!service.name!>", "<!service.name!> Error: " + r, 0);    
    }

<?
  else:?>
    if(m_status.empty()) {
	genom_log("Service \"<!service.name!>\" from '%s' with id:%d finished", m_clientName.c_str(), m_id);
	ReplyWriter<VoidIO>::send(m_replyPort, 
	    m_clientName, m_id, "<!service.name!>", "OK", 0);
    } else {
      std::string r = errorString(m_status.front());      
      genom_log("Service \"<!service.name!>\" from '%s' with id:%d finished with errror: %s", m_clientName.c_str(), m_id, r.c_str());
      ReplyWriter<VoidIO>::send(m_replyPort, m_clientName, m_id, "<!service.name!>", "<!service.name!> Error: " + r, 0);    
    }
<?
  ?>
  }
}

void <!service.name!>Service::abort()
{
  m_status.clear();
<?
  if service.events(): 
    ?>
  m_status.push_front(statusFromEventString("inter"));
<?
  ?>
  m_aborted = true;
}

<?
  if service.events(): 
    ?>
int <!service.name!>Service::statusFromEventString(const std::string &ev)
{
<?
    for e in service.events():
      ev = e.key()
      codel = e.data().replace(".", "_")
      if codel == "control":
	continue
      ?>
  if(ev == "<!ev.identifier()!>") 
    return <!upper(service.name)!>_<!upper(codel)!>;
<?
    ?>

  genom_log("Received unknown event: %s", ev.c_str());
  return <!upper(service.name)!>_ETHER;
}

<?
  ?>

bool <!service.name!>Service::step()
{
<?
  if service.events(): 
    ?>
  while(!isEmpty()) {
    std::string ev = takeEvent();
    if(ev == "inter") { // module is interrupted, abort all (even sleep)
      m_status.clear();
      m_status.push_front(statusFromEventString(ev));
      break;
    }
    m_status.push_front(statusFromEventString(ev));
  }
<?
  else:?>
  if(m_aborted)
    return false;
<?
  ?>

  std::list<int> res_list;
  while(!m_status.empty()) {
    int res = <!upper(service.name)!>_ETHER;
    switch(m_status.front()) {
<?
  for c in service.codels():
    if c.key() == "control":
      continue
    ?>
      case <!upper(service.name)!>_<!upper(c.key())!>:
        res = <!c.key()!>();
        break;
<?
  ?>
      case <!upper(service.name)!>_ETHER:
        break;

      case <!upper(service.name)!>_SLEEP:
        res = <!upper(service.name)!>_SLEEP;
        break;

      default:
        std::cout << "Error unknown status value : " << m_status.front() << std::endl;
        break;
    }

    if(res < 0) { // error
// 	m_status.clear();
      m_status.push_front(res);
      return false;
    }

    // remove the executed step
    m_status.pop_front();
    // and add the new one
    if(res != <!upper(service.name)!>_ETHER)
      res_list.push_back(res);

  }

  m_status = res_list;
  // make sure there is no duplicate in the list
//   m_status.unique(); 
  if(m_status.empty())
    return false; // service is done

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
  int res = <!real_codel_call(codel, "m_data->", service, True)!>;

<?
    if c.key() in eventsList: 
      evName = codel_to_event_name(c.key())
      ?>
  // raise event
  m_eventsSender.sendEvent("<!evName!>");
<?
    ?>
  // update ports, release locks, etc
<?
    codel_release(codel, service);
    ?>
  return res;
}
<?
?> 
