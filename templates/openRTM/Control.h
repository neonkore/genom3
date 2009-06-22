
#ifndef <!upperCompName!>_CONTROL_H
#define <!upperCompName!>_CONTROL_H

#include <rtm/idl/BasicDataTypeSkel.h>
#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>

// Service implementation headers
#include "<!comp.name()!>ControlImpl.h"
// stub headers
#include "ControlTaskSkel.h"

#include "ExecServices.h"
#include "logger.h"
#include "<!comp.name()!>Struct.hpp"

using namespace RTC;

// definition of th ids
struct <!capCompName!>ControlData {
  <!capCompName!>ControlData();

  void killAllServices();
<?
for s in servicesMap:
  if s.data().type == ServiceType.Exec:
    print "  void kill" + s.data().name + "Services();"
?>
  
  // members of the ids
<?
t = comp.IDSType.unalias()
if t.kind() == IdlKind.Struct:
  s = t.asStructType()
  for m in s.members():
     print "  " + MapTypeToCpp(m.data()) + " " + m.key() + ";"
?>
  // Locks
  ACE_RW_Mutex idsMutex;

  // DataInPort declaration
<?
for port in inports:
    typeName = MapTypeToCorbaCpp(port.idlType, True, True)
    ?>
  <!typeName!> <!port.name!>_data;
  InPort<<!typeName!>> <!port.name!>;
<?
?>

  // DataOutPort declaration
<?
for port in outports:
    typeName = MapTypeToCorbaCpp(port.idlType)
    ?>
  <!typeName!> <!port.name!>_data;
  OutPort<<!typeName!>> <!port.name!>;
<?
?>
  // Services' output ports
<?
for name,typeName in output_ports_map.iteritems():
  ?>
  <!name!>OutStruct <!name!>_data;
  OutPort<<!name!>OutStruct> <!name!>_outport;
<?
?>
  // Activities list
<?
for s in comp.servicesMap():
  service = s.data()
  if service.type != ServiceType.Control:
    ?>
   <!service.name!>Service::List <!service.name!>Services;
<?
?>
};

class <!capCompName!>Control  : public RTC::DataFlowComponentBase
{
 public:
  <!capCompName!>Control(RTC::Manager* manager);
  ~<!capCompName!>Control();

  <!capCompName!>ControlData* data() { return &m_data; }

  // The initialize action (on CREATED->ALIVE transition)
  virtual RTC::ReturnCode_t onInitialize();

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


 protected:
  // CORBA Port declaration
  RTC::CorbaPort m_controlServicePort;

  // Service declaration
  <!capCompName!>ControlImpl m_service;

 private:
    <!capCompName!>ControlData m_data;
};


extern "C"
{
  void <!capCompName!>ControlInit(RTC::Manager* manager);
};

#endif // <!upperCompName!>_CONTROL_H

