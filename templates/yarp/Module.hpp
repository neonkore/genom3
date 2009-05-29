#ifndef <!upperCompName!>_MODULE_HPP
#define <!upperCompName!>_MODULE_HPP

#include <yarp/os/all.h>

#include "lib/OutPort.hpp"
#include "lib/DataServer.hpp"

// definition of the ids
struct <!comp.name()!>ControlData {
  <!comp.name()!>ControlData() {}

  void lockForRead();
  void lockForWrite();

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
  // Data ports
<?
for port in inports:
    typeName = MapTypeToCpp(port.idlType)
    ?>
  DataServer<<!typeName!>> <!port.name!>_inport;
<?
for port in outports:
    typeName = MapTypeToCpp(port.idlType)
    ?>
  OutPort<<!typeName!>> <!port.name!>_outport;
<?
?>
};

class <!comp.name()!>Module : public yarp::os::Module
{
    public:
      <!comp.name()!>Module();
      ~<!comp.name()!>Module();

    bool open(Searchable& config);
    bool interruptModule();

    bool updateModule();

    private:
      <!comp.name()!>ControlData *m_data;
      // Request port
      yarp::os::BufferedPort<yarp::os::Bottle> m_request_port;
      yarp::os::BufferedPort<yarp::os::Bottle> m_reply_port;

<?
for t in tasksMap: 
  task = t.data()
  ?>
      <!comp.name()!><!task.name!> * m_<!task.name!>Task;
<?
?>
};

#endif
 
