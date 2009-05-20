
#ifndef <!upperCompName!>_TEST_H
#define <!upperCompName!>_TEST_H

#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>

// stub headers
#include "ControlTaskSkel.h"

using namespace RTC;

class <!capCompName!>Test  : public RTC::DataFlowComponentBase
{
 public:
  <!capCompName!>Test(RTC::Manager* manager);
  ~<!capCompName!>Test();

  // The initialize action (on CREATED->ALIVE transition)
//   virtual RTC::ReturnCode_t onInitialize();

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
  virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

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


 protected:
  // CORBA Port declaration
  RTC::CorbaPort m_servicePort;

  // Service declaration
  RTC::CorbaConsumer<I<!capCompName!>> m_service;

  // Services' output ports
<?
for name,typeName in output_ports_map.iteritems():
  ?>
  <!name!>OutStruct m_<!name!>_data;
  InPort<<!name!>OutStruct> m_<!name!>_inport;
<?
?>
  // DataInPort declaration
<?
for port in outports:
    typeName = MapTypeToCpp(port.idlType)
    ?>
  <!typeName!> m_<!port.name!>_data;
  InPort<<!typeName!>> m_<!port.name!>;
<?
?>
  // DataOutPort declaration
<?
for port in inports:
    typeName = MapTypeToCpp(port.idlType)
    ?>
//   <!typeName!> m_<!port.name!>_data;
//   OutPort<<!typeName!>> m_<!port.name!>;
<?
?>
};


extern "C"
{
  void <!capCompName!>TestInit(RTC::Manager* manager);
};

#endif // <!upperCompName!>_CONTROL_H

