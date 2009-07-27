<?
currentTask = comp.task(currentTaskName)
if currentTask.period == 0:
  activityType = "SPORADIC"
else:
  activityType = "PERIODIC"
?>
#include "<!comp.name()!><!currentTaskName!>.h"

#include "<!comp.name()!>Control.h"
#include "ExecServices.h"

// forward declaration of user codels
extern "C" {
<?
if currentTask.hasCodel("init"):
    print real_codel_signature(currentTask.codel("init")) + ";"
if currentTask.hasCodel("end"):
    print real_codel_signature(currentTask.codel("end")) + ";"
?>
}

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
}

<?
if currentTask.hasCodel("init"):
  ?>
RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onInitialize()
{
<?
  codel_lock(currentTask.codel("init"));
  ?>
  int res = <!real_codel_call(currentTask.codel("init"), "m_data->")!>;
  // update ports, release locks, etc
<?
  codel_release(currentTask.codel("init"));
  ?>
  if(res < 0) // error
      return RTC::RTC_ERROR;
  return RTC::RTC_OK;
}<?
?>

RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onExecute(RTC::UniqueId ec_id)
{
<?
for s in comp.servicesMap():
  service = s.data()
  if service.type == ServiceType.Control or service.taskName != currentTaskName:
    continue
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
  return RTC::RTC_OK;
}

<?
if currentTask.hasCodel("end"):
  ?>
RTC::ReturnCode_t <!capCompName!><!currentTaskName!>::onFinalize()
{
<?
  codel_lock(currentTask.codel("end"));
  ?>
  int res = <!real_codel_call(currentTask.codel("end"), "m_data->")!>;
  // update ports, release locks, etc
<?
  codel_release(currentTask.codel("end"));
  ?>
  if(res < 0) // error
      return RTC::RTC_ERROR;
  return RTC::RTC_OK;
}<?
?>

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
