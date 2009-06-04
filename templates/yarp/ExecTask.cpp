<?
currentTask = comp.task(currentTaskName)
?>

#include "<!comp.name()!><!currentTaskName!>.hpp"

#include "<!comp.name()!>Module.hpp"
#include "ExecServices.hpp"
#include "lib/RepliesWriter.hpp"

using namespace GenomYarp;
using namespace yarp::os;

// forward declaration of user codels
<?
for s in servicesMap:
  service = s.data()
  if service.type == ServiceType.Control or service.taskName != currentTaskName:
    continue
  if service.hasCodel("control"):
    print "int " + real_codel_signature(service.codel("control"), service) + ";"
?>

<!comp.name()!><!currentTaskName!>::<!comp.name()!><!currentTaskName!>(<!comp.name()!>ControlData *data)
<?
if currentTask.period:?>
: RateThread(<!currentTask.period!>), m_data(data)
<?
else:?>
: Thread(), m_data(data)
<?
?>
{
//     setOptions(<!currentTask.stackSize!>);
} 

<!comp.name()!><!currentTaskName!>::~<!comp.name()!><!currentTaskName!>()
{}

bool <!comp.name()!><!currentTaskName!>::threadInit()
{
    m_request_port.open("/<!comp.name()!>/Services/<!currentTaskName!>");
//     m_reply_port.open("/<!comp.name()!>/Services/Replies/<!currentTaskName!>");
    m_request_port.useCallback(*this);

<?
if currentTask.hasCodel("init"):
  ?>
  // call user init func
  int res = <!real_codel_call(currentTask.codel("init"))!>;
  return (res >= 0); // error
<?
?>
}

<?
if currentTask.hasCodel("end"):
  ?>
void <!comp.name()!><!currentTaskName!>::threadRelease()
{
  // call user init func
  int res = <!real_codel_call(currentTask.codel("end"))!>;
  return (res >= 0); // error
}
<?
?>

void <!comp.name()!><!currentTaskName!>::run()
{
<?
for s in comp.servicesMap():
  service = s.data()
  if service.type != ServiceType.Exec or service.taskName != currentTaskName:
    continue
  serviceInfo = services_info_dict[service.name]
  ?>
  for(<!service.name!>Service::List::iterator it = m_data-><!service.name!>Services.begin();
	it != m_data-><!service.name!>Services.end();) {
    if(!(*it)->step()) // delete the service
      it = m_data-><!service.name!>Services.erase(it);
    else
      ++it;
  }
<?
?>
}
    
void <!comp.name()!><!currentTaskName!>::onRead(yarp::os::Bottle& command)
{
    string client_name  = RqstReader::readClientName(command);
    int rqst_id = RqstReader::readRqstId(command);
    string request_name  = RqstReader::readRequestName(command);

    std::cout << "<!currentTaskName!>: Received request from " << client_name
	<< ", id=" << rqst_id << ", service=" << request_name << std::endl;

    if(m_reply_ports.find(client_name) == m_reply_ports.end()) {
	// unknwon client, create the reply port and connect it
	string port_name = "/<!comp.name()!>/Services/Replies/<!currentTaskName!>/" + client_name;
	string client_port_name = client_name + "/<!comp.name()!>/Services/Replies/<!currentTaskName!>";

	m_reply_ports[client_name] = new RequestPort();
	m_reply_ports[client_name]->open(port_name.c_str());
	if(!Network::connect(port_name.c_str(), client_port_name.c_str())) {
	    cout << "Impossible to connect to client " << client_name << " reply port. Aborting." << endl;
	    return;
	}
    }

<?
first = True
for s in servicesMap:
  service = s.data()
  if service.type == ServiceType.Control or service.taskName != currentTaskName:
    continue
  if first:
    first = False
    elseStr = ""
  else:
    elseStr = "else"
  ?>
    <!elseStr!> if(request_name == "<!service.name!>") {
	run<!service.name!>(client_name, rqst_id, command);
    }
<?
?>
    else {     // wrong request name
	string r = "No such service: "  + request_name;
	cout << r << endl;
	ReplyWriter<VoidIO>::send(*m_reply_ports[client_name], client_name, rqst_id, request_name, r, 0);
    }
}


<?
for s in servicesMap:
  service = s.data()
  if service.type == ServiceType.Control or service.taskName != currentTaskName:
    continue
  serviceInfo = services_info_dict[service.name]
  args = ""
  for i in service.inputs():
    args += ", in_" + i.identifier
  ?>
bool <!comp.name()!><!currentTaskName!>::run<!service.name!>(const std::string &clientName, int rqst_id, const Bottle &command)
{
<?
  if serviceInfo.inputFlag: ?>
  <!serviceInfo.inputTypeCpp!> in_<!serviceInfo.inputName!> = RqstReader::readRqstInput<<!serviceInfo.inputTypeCpp!>>(command);
<?
  if service.hasCodel("control"):
	codelLock(service.codel("control"), service)
	?>
  // call real control codel
  int res = <!real_codel_call(service.codel("control"), "m_data->",service)!>;
<?
	codelRelease(service.codel("control"), service)
	?>
  if(res < 0) { // error
    string r = "<!service.name!>: " + errorString(res);
    cout << r << endl;
    ReplyWriter<VoidIO>::send(*m_reply_ports[clientName], clientName, rqst_id, "<!service.name!>", r, 0);    
    return true;
  }
<?
  ?>

  // kill existing incompatible services
<?
  if service.incompatibleServices() and service.incompatibleServices()[0] != "none":
      if service.incompatibleServices()[0] == "all":
	print "  m_data->killAllServices();"
      else:
	for incomp in service.incompatibleServices():
	  print "  m_data->kill" + incomp + "Services();" 

  if len(service.inputs()) > 1:
    for i in service.inputs():
      if i.type == ServiceInputKind.IDSMember:
	print "  m_data->" + i.identifier + " = input." + i.identifier + ";" 
  elif serviceInfo.inputFlag:
    if service.inputs()[0].kind == ServiceInputKind.IDSMember:
      print "  m_data->" + serviceInfo.inputName + " = input;" 
  ?>

  // create the activity
  <!service.name!>Service::Ptr s = <!service.name!>Service::Ptr(new <!service.name!>Service(m_data, rqst_id, clientName, *m_reply_ports[clientName] <!args!>));
  m_data-><!service.name!>Services.push_back(s);

  // send first reply
//   ReplyWriter<VoidIO>::send(m_reply_port, clientName, rqst_id, "<!service.name!>", "OK", 0);    
  return true;
}
<?
?>

