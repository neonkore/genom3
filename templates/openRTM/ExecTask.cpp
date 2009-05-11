<?
currentTask = comp.task(currentTaskName)
if currentTask.period == 0:
  activityType = "SPORADIC"
else:
  activityType = "PERIODIC"
?>
#include "<!comp.name()!><!currentTaskName!>.h"

// Module specification
static const char* <!comp.name()!><!currentTaskName!>_spec[] =
  {
    "implementation_id", "<!comp.name()!><!currentTaskName!>",
    "type_name",         "<!comp.name()!><!currentTaskName!>",
    "description",       "currentTask.description()",
    "version",           "<!comp.version!>",
    "vendor",            "AIST",
    "category",          "Generic",
    "activity_type",     "<!activityType!>",
    "kind",              "<!comp.name()!>",
    "max_instance",      "10",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    ""
  };

<!capCompName!><!currentTaskName!>::<!capCompName!><!currentTaskName!>(RTC::Manager* manager)
  : RTC::DataFlowComponentBase(manager)
    m_servicePort("<!capCompName!><!currentTaskName!>")
{
  // Set service provider to Ports
  m_servicePort.registerProvider("<!currentTaskName!>Service", "I<!capCompName!><!currentTaskName!> ", m_service);

  // Set CORBA Service Ports
  registerPort(m_servicePort);
}

<!capCompName!><!currentTaskName!>::~<!capCompName!><!currentTaskName!>()
{
}

<?
if currentTask.hasCodel("init"):
  ?>
RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onInitialize()
{
  int res = <!codel_call(currentTask.codel("init"))!>;
  if(res == ERROR)
      return RTC::RTC_ERROR;
  return RTC::RTC_OK;
}<?
?>

/*
RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onFinalize()
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


extern "C"
{
 
  void <!capCompName!><!currentTaskName!>Init(RTC::Manager* manager)
  {
    RTC::Properties profile(<!comp.name()!><!currentTaskName!>_spec);
    manager->registerFactory(profile,
                             RTC::Create<<!capCompName!><!currentTaskName!>>,
                             RTC::Delete<<!capCompName!><!currentTaskName!>>);
  }
  
};



