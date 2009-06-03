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
<?
for s in servicesMap:
  service = s.data()
  if service.type != ServiceType.Control:
    continue
  if service.hasCodel("control"):
    print "int " + real_codel_signature(service.codel("control"), service) + ";"
?>

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
  <!service.name!>Services.clear();
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
} 

<!comp.name()!>Module::~<!comp.name()!>Module()
{}

bool <!comp.name()!>Module::open(yarp::os::Searchable& config)
{
    // initialize data
    m_data = new <!comp.name()!>ControlData();

    // open data ports
<?
for p in outports: ?>
    m_data-><!p.name!>_outport.open("/<!comp.name()!>/OutPorts/<!p.name!>");
<?
for p in inports: ?>
    m_data-><!p.name!>_inport.open("/<!comp.name()!>/InPorts/<!p.name!>");
<?
?>
    m_request_port.open("/<!comp.name()!>/Services/Control");
//     attach(m_request_port);
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
for p in outports: ?>
    m_data-><!port.name!>_outport.interrupt();
<?
for p in inports: ?>
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
    return true;
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

    std::cout << "Control: Received request from " << client_name << ", id=" << rqst_id << ", service=" << request_name << std::endl;
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
      cout << r << endl;
      ReplyWriter<VoidIO>::send(m_request_port, client_name, rqst_id, request_name, r, 0);
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
	?>
  // call real control codel
  int res = <!real_codel_call(service.codel("control"), "m_data->", service)!>;
  if(res < 0) { // error
    string r = "<!service.name!> : " + errorString(res);
    cout << r << endl;
    ReplyWriter<VoidIO>::send(m_request_port, clientName, rqst_id, "<!service.name!>", r, 0);    
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
      print "  m_data->" + serviceInfo.inputName + " = in_" + serviceInfo.inputName + ";" 
  ?>

<?
  if serviceInfo.outputFlag: ?>
  ReplyWriter<<!serviceInfo.outputTypeCpp!>>::send(m_request_port, clientName, rqst_id, "<!service.name!>", "OK", &m_data-><!serviceInfo.outputName!>);    
<?
  else: ?>
  ReplyWriter<VoidIO>::send(m_request_port, clientName, rqst_id, "<!service.name!>", "OK", 0);    
<?
  ?>
  return true;
}
<?
?>

