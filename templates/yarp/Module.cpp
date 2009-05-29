#include "module.hpp"

// forward declaration of user codels
<?
for s in servicesMap:
  service = s.data()
  if service.type != ServiceType.Control:
    continue
  if service.hasCodel("control"):
    print "int " + real_codel_signature(service.codel("control"), service) + ";"
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

bool <!comp.name()!>Module::open(Searchable& config)
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
    attach(m_request_port);

    // create exec task and start them
<?
for t in tasksMap:
  task = t.data()
  if task.period:
    periodStr = ", " + str(task.period)
  else:
    periodStr = ""
  ?>
    m_<!task.name!>Task = new <!comp.name()!><!task.name!>(m_data<!periodStr!>);
    m_<!task.name!>Task->start();
<?
?>
    return true;
}

bool <!comp.name()!>Module::interruptModule()
{
<?
for p in portsMap: ?>
    <!p.key()!>.interrupt();
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
    string client_name  = RqstReader::readClientName(command);
    int rqst_id = RqstReader::readRqstId(command);
    string request_name  = RqstReader::readClientName(command);

<?
for s in servicesMap:
  service = s.data()
  if service.type != ServiceType.Control:
    continue
  ?>
    if(request_name == "<!service.name!>") 
	return run<!service.name!>(client_name, rqst_id, command, reply);
<?
?>
    // wrong request name
    string r = "No such service: "  + request_name;
    cout << r << endl;
    ReplyWriter<VoidIO>::write(reply, client_name, rqst_id, request_name, r, 0);
    return true;
}

<?
for s in servicesMap:
  service = s.data()
  if service.type != ServiceType.Control:
    continue
  serviceInfo = services_info_dict[service.name]
  ?>
bool <!comp.name()!>Module::run<!service.name!>(const std::string &clientName, int rqst_id, const Bottle &command, Bottle &reply)
{
<?
  if serviceInfo.inputFlag: ?>
  <!serviceInfo.inputTypeCpp!> input = RqstReader::readRqstInput<<!serviceInfo.inputTypeCpp!>>(command);
<?
  if service.hasCodel("control"):
	?>
  // call real control codel
  int res = <!real_codel_call(service.codel("control"), service)!>;
  if(res < 0) { // error
    string r = request_name + ": " + errorString(res);
    cout << r << endl;
    ReplyWriter<VoidIO>::write(reply, clientName, rqst_id, "<!service.name!>", r, 0);    
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

<?
  if serviceInfo.outputFlag: ?>
  ReplyWriter<<!serviceInfo.outputTypeCpp!>>::write(reply, clientName, rqst_id, "<!service.name!>", "OK", m_data-><!serviceInfo.outputName!>);    
<?
  else: ?>
  ReplyWriter<VoidIO>::write(reply, clientName, rqst_id, "<!service.name!>", "OK", 0);    
<?
  ?>
  return true;
}
<?
?>

