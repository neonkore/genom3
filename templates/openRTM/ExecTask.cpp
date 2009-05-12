<?
currentTask = comp.task(currentTaskName)
if currentTask.period == 0:
  activityType = "SPORADIC"
else:
  activityType = "PERIODIC"
?>
#include "<!comp.name()!><!currentTaskName!>.h"

// forward declaration of user codels
<?
if currentTask.hasCodel("init"):
    print "int " + real_codel_signature(currentTask.codel("init")) + ";"
?>

<!capCompName!><!currentTaskName!>::<!capCompName!><!currentTaskName!>(RTC::Manager* manager)
  : RTC::DataFlowComponentBase(manager),
    m_data(0)
{
}

<!capCompName!><!currentTaskName!>::~<!capCompName!><!currentTaskName!>()
{
}

void <!capCompName!><!currentTaskName!>::setData(<!capCompName!>ControlData *data)
{
  m_data = data;
  m_service.setData(data);
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

RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onExecute(RTC::UniqueId ec_id)
{
<?
for s in comp.servicesMap():
  service = s.data()
  if service.type != ServiceType.Exec or service.taskName != currentTaskName:
    continue
  ?>
  for(std::list<<!service.name!>Service*>::iterator it = m_service.m_<!service.name!>Services.begin();
	it != m_service.m_<!service.name!>Services.end(); ++it) {
    if(!it->step()) { // delete the service
<?
  if len(service.output) > 0:
    ?>
      // get the output result
      m_data->m_<!service.name!>_outport->write(make_pair(it->result(), m_id))
<?
  ?>
      delete *it;
      m_data->m_<!service.name!>Services.remove(*it);
    }
  }
<?
?>
  return RTC::RTC_OK;
}

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
