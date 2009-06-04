<?
currentTask = comp.task(currentTaskName)
if not currentTask.period:
  taskBaseClass = "Thread"
else:
  taskBaseClass = "RateThread"
?>

#ifndef <!upperCompName!>_<!upper(currentTaskName)!>_HPP
#define <!upperCompName!>_<!upper(currentTaskName)!>_HPP

#include <string>
#include <yarp/os/all.h>

#include "lib/Struct.hpp"

class <!comp.name()!>ControlData;

// forward declaration of user codels
<?
if currentTask.hasCodel("init"):
    print "int " + real_codel_signature(currentTask.codel("init")) + ";"
if currentTask.hasCodel("end"):
    print "int " + real_codel_signature(currentTask.codel("end")) + ";"
?>

class <!comp.name()!><!currentTaskName!> : public yarp::os::<!taskBaseClass!>, public yarp::os::TypedReaderCallback<yarp::os::Bottle>
{
    public:
      <!comp.name()!><!currentTaskName!>(<!comp.name()!>ControlData *data);
      ~<!comp.name()!><!currentTaskName!>();

    virtual void run();

    virtual bool threadInit();
<?
if currentTask.hasCodel("end"): ?>
    virtual void threadRelease();
<?
?>
    virtual void onRead(yarp::os::Bottle& b);

    protected:
<?
for s in servicesMap:
  service = s.data()
  if service.type == ServiceType.Control or service.taskName != currentTaskName:
    continue
  ?>
      bool run<!service.name!>(const std::string &clientName, int rqst_id,
	    const yarp::os::Bottle &command);
<?
?>

    private:
      <!comp.name()!>ControlData *m_data;
      // Request port
      GenomYarp::RequestPort m_request_port;
      GenomYarp::RequestPortMap m_reply_ports;
};

#endif
 
