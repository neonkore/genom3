#include "<!comp.name()!>Module.hpp"

#include "lib/RepliesWriter.hpp"
#include "userCodels.h"

using namespace GenomYarp;

<?
for t in tasksMap:
  task = t.data()
  print "#include \"" + comp.name() + task.name + ".hpp\""
?> 

using namespace yarp::os;

// forward declaration of user codels
extern "C" {
<?
for s in servicesMap:
  service = s.data()
  if service.type != ServiceType.Control:
    continue
  if service.hasCodel("control"):
    print "int " + real_codel_signature(service.codel("control"), service) + ";"
?>
}

// helper function for codel calls
std::string errorString(int err)
{
  switch(err) {
<?
for e in errorList: ?>
    case ERROR_<!e!>: return "<!e!>";
<?
?>
    default: return "";
  }
  return "";
}

// <!comp.name!>ControlData methods
void <!comp.name()!>ControlData::killAllServices()
{
<?
for s in servicesMap:
  service = s.data()
  if service.type == ServiceType.Exec:
    print "  kill" + service.name + "Services();"
?>}

<?
for s in servicesMap:
  service = s.data()
  if service.type == ServiceType.Exec:
    ?>
void <!comp.name()!>ControlData::kill<!service.name!>Services()
{
  for(<!service.name!>Service::List::iterator it = <!service.name!>Services.begin();
	it != <!service.name!>Services.end(); ++it)
      (*it)->abort(); // will be deleted next step
}
<?
?>

<!comp.name()!>Module::<!comp.name()!>Module()
: m_data(0) <?
for t in tasksMap: ?>
, m_<!t.data().name!>Task(0)
<?
?>
{
    setName("<!comp.name()!>");
    log_to_file("./<!comp.name()!>.log");
} 

<!comp.name()!>Module::~<!comp.name()!>Module()
{
  end_logging();
}

bool <!comp.name()!>Module::open(yarp::os::Searchable& config)
{
    // initialize data
    m_data = new <!comp.name()!>ControlData();

    // open data ports
<?
for p in outports: ?>
    m_data-><!p.name!>_outport.open("/<!comp.name()!>/OutPorts/<!p.name!>");
    m_data-><!p.name!>_outport.setName("<!p.name!>");
<?
for p in inports: ?>
    m_data-><!p.name!>_inport.open("/<!comp.name()!>/InPorts/<!p.name!>");
    m_data-><!p.name!>_outport.setName("<!p.name!>");
<?
?>
    m_request_port.open("/<!comp.name()!>/Services/Control");
//     m_reply_port.open("/<!comp.name()!>/Services/Replies/Control");
    m_request_port.useCallback(*this);

    // create exec task and start them
<?
for t in tasksMap:
  task = t.data()
  ?>
    m_<!task.name!>Task = new <!comp.name()!><!task.name!>(m_data);
    m_<!task.name!>Task->start();
<?
?>
    return true;
}

bool <!comp.name()!>Module::interruptModule()
{
<?
for port in outports: ?>
    m_data-><!port.name!>_outport.interrupt();
<?
for port in inports: ?>
    m_data-><!port.name!>_inport.interrupt();
<?
for t in tasksMap: 
  task = t.data()
  ?>
    m_<!task.name!>Task->suspend();
<?
?>
    return true;
}

bool <!comp.name()!>Module::updateModule()
{
    return Module::updateModule();
}

bool <!comp.name()!>Module::respond(const Bottle &command, Bottle &reply)   
{
    Module::respond(command, reply);
}

void <!comp.name()!>Module::onRead(Bottle& command)
{
    string client_name  = RqstReader::readClientName(command);
    int rqst_id = RqstReader::readRqstId(command);
    string request_name  = RqstReader::readRequestName(command);

    genom_log("Control: Received request for service '%s' from '%s' with id:%d", request_name.c_str(), client_name.c_str(), rqst_id);

    if(m_reply_ports.find(client_name) == m_reply_ports.end()) {
	// unknwon client, create the reply port and connect it
	string port_name = "/<!comp.name()!>/Services/Replies/Control/" + client_name;
	string client_port_name = client_name + "/<!comp.name()!>/Services/Replies/Control";

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
  if service.type != ServiceType.Control:
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
    else {
      string r = "No such service: "  + request_name;
      genom_log(r.c_str());
      ReplyWriter<VoidIO>::send(*m_reply_ports[client_name], client_name, rqst_id, request_name, r, 0);
    }
}

<?
for s in servicesMap:
  service = s.data()
  if service.type != ServiceType.Control:
    continue
  serviceInfo = services_info_dict[service.name]
  ?>
bool <!comp.name()!>Module::run<!service.name!>(const std::string &clientName, int rqst_id, const Bottle &command)
{
<?
  if serviceInfo.inputFlag: ?>
  <!serviceInfo.inputTypeCpp!> in_<!serviceInfo.inputName!> = RqstReader::readRqstInput<<!serviceInfo.inputTypeCpp!>>(command);
<?
  if service.hasCodel("control"):
    codelLock(service.codel("control"), service)
    ?>
  // call real control codel
  int res = <!real_codel_call(service.codel("control"), "m_data->", service)!>;
<?
    codelRelease(service.codel("control"), service)
    ?>
  if(res < 0) { // error
    string r = "<!service.name!> : " + errorString(res);
    genom_log(r.c_str());
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
    ?>
  m_data->idsMutex.acquire_write();
<?
    for i in service.inputs():
      if i.type == ServiceInputKind.IDSMember: ?>
  m_data-><!i.identifier!> = input.<!i.identifier!>; 
<?
    ?>
  m_data->idsMutex.release();
<?
  elif serviceInfo.inputFlag:
    if service.inputs()[0].kind == ServiceInputKind.IDSMember: ?>
  m_data->idsMutex.acquire_write();
  m_data-><!serviceInfo.inputName!> = in_<!serviceInfo.inputName!>; 
  m_data->idsMutex.release();
<?
  ?>

<?
  if serviceInfo.outputFlag: ?>
  genom_log("Sending \"OK\" reply for \"<!service.name!>\" request from '%s' with id:%d", clientName.c_str(), rqst_id);
  ReplyWriter<<!serviceInfo.outputTypeCpp!>>::send(*m_reply_ports[clientName], clientName, rqst_id, "<!service.name!>", "OK", &m_data-><!serviceInfo.outputName!>);    
<?
  else: ?>
  genom_log("Sending \"OK\" reply for \"<!service.name!>\" request from '%s' with id:%d", clientName.c_str(), rqst_id);
  ReplyWriter<VoidIO>::send(*m_reply_ports[clientName], clientName, rqst_id, "<!service.name!>", "OK", 0);    
<?
  ?>
  return true;
}
<?
?>

