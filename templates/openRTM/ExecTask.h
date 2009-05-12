<?
currentTask = comp.task(currentTaskName)
?>
#ifndef <!upperCompName!>_<!upper(currentTaskName)!>_H
#define <!upperCompName!>_<!upper(currentTaskName)!>_H

#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>

using namespace RTC;

class <!capCompName!><!currentTaskName!> : public RTC::DataFlowComponentBase
{
 public:
  <!capCompName!><!currentTaskName!>(RTC::Manager* manager);
  ~<!capCompName!><!currentTaskName!>();

  void setData(<!capCompName!>ControlData *data);

<?
if currentTask.hasCodel("init"): ?>
  // The initialize action (on CREATED->ALIVE transition)
  virtual RTC::ReturnCode_t onInitialize();
<?
?>

  // The finalize action (on ALIVE->END transition)
  // virtual RTC::ReturnCode_t onFinalize();

  // The startup action when ExecutionContext startup
  // virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

  // The shutdown action when ExecutionContext stop
  // virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

  // The activated action (Active state entry action)
  // virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  // The deactivated action (Active state exit action)
  // virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  // The execution action that is invoked periodically
  // virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  // The aborting action when main logic error occurred.
  // virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

  // The error action in ERROR state
  // virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  // The reset action that is invoked resetting
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
  // The state update action that is invoked after onExecute() action
  // virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

  // The action that is invoked when execution context's rate is changed
  // virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);

 private:
  <!capCompName!>ControlData *m_data;
};


extern "C"
{
  void <!capCompName!><!currentTaskName!>Init(RTC::Manager* manager);
};

#endif // <!upperCompName!>_<!upper(currentTaskName)!>_H

