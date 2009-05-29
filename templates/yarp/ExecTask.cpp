<?
currentTask = comp.task(currentTaskName)
?>

#include "<!comp.name()!><!currentTaskName!>.hpp"

#include "<!comp.name()!>Module.h"
#include "ExecServices.h"

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
    setOptions(<!currentTask.stackSize!>);
} 

bool <!comp.name()!><!currentTaskName!>::threadInit()
{
    m_request_port.open("/<!comp.name()!>/Services/<!currentTaskName!>");

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
bool <!comp.name()!><!currentTaskName!>::threadRelease()
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
    if(!(*it)->step()) { // delete the service
<?
  if serviceInfo.outputFlag: ?>
      // send the result
      <!serviceInfo.outputTypeCpp!> res = (*it)->result();
      ReplyWriter<<!serviceInfo.outputTypeCpp!>>::write(reply, 
	  (*it)->clientName(), (*it)->id(), "<!service.name!>", "OK", &res);    
<?
  else:?>
      // send the final reply
      ReplyWriter<VoidIO>::write(reply, 
	  (*it)->clientName(), (*it)->id(), "<!service.name!>", "OK", 0);
<?
  ?>
      it = m_data-><!service.name!>Services.erase(it);
    } else
      ++it;
  }
<?
?>
}

bool <!comp.name()!><!currentTaskName!>::respond(const Bottle &command, Bottle &reply)   
{
    string client_name  = RqstReader::readClientName(command);
    int rqst_id = RqstReader::readRqstId(command);
    string request_name  = RqstReader::readClientName(command);

<?
for s in servicesMap:
  service = s.data()
  if service.type == ServiceType.Control or service.taskName != currentTaskName:
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
  if service.type == ServiceType.Control or service.taskName != currentTaskName:
    continue
  serviceInfo = services_info_dict[service.name]
  args = ", rqst_id"
  for i in service.inputs():
    args += ", in_" + i.identifier
  ?>
bool <!comp.name()!>Module::run<!service.name!>(const std::string &clientName, int rqst_id, const Bottle &command, Bottle &reply)
{
<?
  if serviceInfo.inputFlag: ?>
  <!serviceInfo.inputTypeCpp!> in_<!serviceInfo.inputName!> = RqstReader::readRqstInput<<!serviceInfo.inputTypeCpp!>>(command);
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

  // create the activity
  <!service.name!>Service::Ptr s = <!service.name!>Service::Ptr(new <!service.name!>Service(m_data <!args!>));
  m_data-><!service.name!>Services.push_back(s);

  // send first reply
  ReplyWriter<VoidIO>::write(reply, clientName, rqst_id, "<!service.name!>", "OK", 0);    
  return true;
}
<?
?>

