
#include "<!comp.name()!>Control.h"

<?
for t in comp.tasksMap():
  print "#include \"" + comp.name() + t.data().name + ".h\""
?>

// Module specification
static const char* <!comp.name()!>Control_spec[] =
  {
    "implementation_id", "<!capCompName!>Control",
    "type_name",         "<!capCompName!>Control",
    "description",       "comp.desccription()",
    "version",           "<!comp.version!>",
    "vendor",            "AIST",
    "category",          "Generic",
    "activity_type",     "SPORADIC",
    "kind",              "<!comp.name()!>",
    "max_instance",      "10",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    ""
  };

<?
portInit = ""
for port in inports:
  portInit += port.name + "(\"" + port.name + "\", " + port.name + "_data),\n" 
for port in outports:
  portInit += port.name + "(\"" + port.name + "\", " + port.name + "_data),\n" 
for name,type in output_ports_map.iteritems():
  portInit += name + "_outport(\"" + name + "_outport\", " + name + "_data),\n" 
?>
<!capCompName!>ControlData::<!capCompName!>ControlData()
: <!portInit[:-2]!>
{}

void <!capCompName!>ControlData::killAllServices()
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
void <!capCompName!>ControlData::kill<!service.name!>Services()
{
  <!service.name!>Services.clear();
}
<?
?>

<!capCompName!>Control::<!capCompName!>Control(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_controlServicePort("<!capCompName!>Control"),
    m_service(&m_data)
{
  // Set InPort buffers
<?
for port in inports:
    typeName = MapTypeToCpp(port.idlType)
    ?>
  registerInPort("<!port.name!>", m_data.<!port.name!>);
<?
?>
  // Set OutPort buffers
<?
for port in outports:
    typeName = MapTypeToCpp(port.idlType)
    ?>
  registerOutPort("<!port.name!>", m_data.<!port.name!>);
<?
for name,typeName in output_ports_map.iteritems():
  ?>
  registerOutPort("<!name!>_outport", m_data.<!name!>_outport);
<?
?>
  // Set service provider to Ports
  m_controlServicePort.registerProvider("<!capCompName!>Service", "I<!capCompName!>", m_service);

  // Set CORBA Service Ports
  registerPort(m_controlServicePort);
}

<!capCompName!>Control::~<!capCompName!>Control()
{
}

RTC::ReturnCode_t <!capCompName!>Control::onInitialize()
{
    // create tasks and connect them
<?
for t in comp.tasksMap():
  task = t.data()
  ?>  
  <!capCompName!><!task.name!> *m_<!capCompName!><!task.name!> = new <!capCompName!><!task.name!>(m_pManager); 
  m_<!capCompName!><!task.name!>->setData(&m_data);
  RTObject_var rtobj = m_<!capCompName!><!task.name!>->getObjRef();
<?
  if task.period > 0: ?>
  PeriodicExecutionContext *m_<!capCompName!><!task.name!>_exc = new PeriodicExecutionContext(::DataFlowComponent::_narrow(rtobj));
  m_<!capCompName!><!task.name!>_exc->set_rate(1000 / <!task.period!>);
<? 
  else: ?>
  ExtTriggerExecutionContext *m_<!capCompName!><!task.name!>_exc = new ExtTriggerExecutionContext(::DataFlowComponent::_narrow(rtobj));
<?
  ?>
  m_<!capCompName!><!task.name!>_exc->add(rtobj);
  m_<!capCompName!><!task.name!>_exc->activate_component(rtobj);
  m_<!capCompName!><!task.name!>_exc->start();

<?
if initServiceNb != -1:
  ?>
  // call user init function
  int res = <!codel_call(initService.codel("exec", initService))!>;
  if(res == ERROR)
      return RTC::RTC_ERROR;
<?
?>
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t <!capCompName!>Control::onFinalize()
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!>Control::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!>Control::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!>Control::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!comp.name()!>Control::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!>Control::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!>Control::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!>Control::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!>Control::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!comp.name()!>Control::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!>Control::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


extern "C"
{
 
  void <!capCompName!>ControlInit(RTC::Manager* manager)
  {
    RTC::Properties profile(<!comp.name()!>Control_spec);
    manager->registerFactory(profile,
                             RTC::Create<<!capCompName!>Control>,
                             RTC::Delete<<!capCompName!>Control>);
  }
  
};



