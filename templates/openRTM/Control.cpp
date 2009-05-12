
#include "<!comp.name()!>Control.h"

// Module specification
static const char* <!comp.name()!>Control_spec[] =
  {
    "implementation_id", "<!comp.name()!>Control",
    "type_name",         "<!comp.name()!>Control",
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

<!capCompName!>Control::<!capCompName!>Control(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager)
    m_controlServicePort("<!capCompName!>Control"),
    m_service(&m_data)
{
  // Set InPort buffers
<?
for port in inports:
    typeName = MapTypeToCpp(port.idlType)
    ?>
  registerInPort("<!port.name!>", m_data.m_<!port.name!>);
<?
?>
  // Set OutPort buffers
<?
for port in outports:
    typeName = MapTypeToCpp(port.idlType)
    ?>
  registerOutPort("<!port.name!>", m_data.m_<!port.name!>);
<?
?>
  // Set service provider to Ports
  m_controlServicePort.registerProvider("controlService", "I<!capCompName!>Control", m_controlService);

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
  <!capCompName!><!task.name!> *m_<!capCompName!><!task.name!> = new <!capCompName!><!task.name!>(&m_data); 
<?
  if task.period > 0: ?>
  PeriodicExecutionContext *m_<!capCompName!><!task.name!>_exc = new PeriodicExecutionContext(m_<!capCompName!><!task.name!>, 1.0 / (<!task.period!> * 1000));
<? 
  else: ?>
  ExtTriggerExecutionContext *m_<!capCompName!><!task.name!>_exc = new ExtTriggerExecutionContext(m_<!capCompName!><!task.name!>);
<?
  ?>
  m_<!capCompName!><!task.name!>_exc->add(m_<!capCompName!><!task.name!>);
  m_<!capCompName!><!task.name!>_exc->activate_component(m_<!capCompName!><!task.name!>);
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



