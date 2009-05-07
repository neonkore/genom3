
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
<?#  m_MyServicePort("MyService")
for t in comp.tasksMap():
    task = t.data()
    print "    m_" + task.name + "ConsumerServicePort(\"" + capCompName + task.name + "Consumer\")," 
    print "    m_" + task.name + "ProviderServicePort(\"" + capCompName + task.name + "\")," 
?>    m_controlServicePort("<!capCompName!>Control")
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
<?
for port in inports:
    typeName = MapTypeToCpp(port.idlType)
    ?>
  registerInPort("<!port.name!>", m_<!port.name!>);
<?
?>
  // Set OutPort buffers
<?
for port in outports:
    typeName = MapTypeToCpp(port.idlType)
    ?>
  registerOutPort("<!port.name!>", m_<!port.name!>);
<?
?>
  // Set service provider to Ports
  m_controlServicePort.registerProvider("controlService", "I<!capCompName!>Control", m_controlService);
<?
for t in comp.tasksMap():
    task = t.data()
    print "  m_" + task.name + "ProviderServicePort.registerProvider(\"" + task.name + "ProviderService\","
    print "    \"I" + capCompName + task.name + "\", m_" + task.name + "ProviderService);"
?>
  // Set service consumers to Ports
<?
for t in comp.tasksMap():
    task = t.data()
    print "  m_" + task.name + "ConsumerServicePort.registerConsumer(\"" + task.name + "ConsumerService\","
    print "    \"I" + capCompName + task.name + "\", m_" + task.name + "ConsumerService);"
?>

  // Set CORBA Service Ports
  registerPort(m_controlServicePort);<?
for t in comp.tasksMap():
    task = t.data()
    ?>
  registerPort(m_<!task.name!>ConsumerServicePort);
  registerPort(m_<!task.name!>ProviderServicePort);<?
?>
}

<!comp.name()!>::~<!comp.name()!>()
{
}


RTC::ReturnCode_t <!comp.name()!>::onInitialize()
{
  // <rtc-template block="bind_config">
  // Bind variables and configuration variable

  // </rtc-template>
  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t <!comp.name()!>::onFinalize()
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!comp.name()!>::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!comp.name()!>::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!comp.name()!>::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!comp.name()!>::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!comp.name()!>::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!comp.name()!>::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!comp.name()!>::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!comp.name()!>::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!comp.name()!>::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!comp.name()!>::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


extern "C"
{
 
  void <!comp.name()!>Init(RTC::Manager* manager)
  {
    RTC::Properties profile(myserviceconsumer_spec);
    manager->registerFactory(profile,
                             RTC::Create<<!comp.name()!>>,
                             RTC::Delete<<!comp.name()!>>);
  }
  
};



