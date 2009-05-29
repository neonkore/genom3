<?
currentTask = comp.task(currentTaskName)
if not currentTask.period:
  taskBaseClass = "Thread"
else:
  taskBaseClass = "RateThread"
?>

#ifndef <!upperCompName!>_<!upper(currentTaskName)!>_HPP
#define <!upperCompName!>_<!upper(currentTaskName)!>_HPP

#include <yarp/os/all.h>

// forward declaration of user codels
<?
if currentTask.hasCodel("init"):
    print "int " + real_codel_signature(currentTask.codel("init")) + ";"
if currentTask.hasCodel("end"):
    print "int " + real_codel_signature(currentTask.codel("end")) + ";"
?>

class <!comp.name()!><!currentTaskName!> : public yarp::os::<!taskBaseClass!>, public yarp::os::PortReader
{
    public:
      <!comp.name()!><!currentTaskName!>();
      ~<!comp.name()!><!currentTaskName!>();

    virtual void run();

    virtual bool threadInit();
<?
if currentTask.hasCodel("end"): ?>
    virtual bool threadRelease();
<?
?>
    private:
      <!comp.name()!>ControlData *m_data;
      // Request port
      yarp::os::BufferedPort<yarp::os::Bottle> m_request_port;
      yarp::os::BufferedPort<yarp::os::Bottle> m_reply_port;
};

#endif
 
