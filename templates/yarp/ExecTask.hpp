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

class <!comp.name()!>ControlData;

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
      <!comp.name()!><!currentTaskName!>(<!comp.name()!>ControlData *data);
      ~<!comp.name()!><!currentTaskName!>();

    virtual void run();

    virtual bool threadInit();
<?
if currentTask.hasCodel("end"): ?>
    virtual bool threadRelease();
<?
?>
    virtual bool read(yarp::os::ConnectionReader &);

    protected:
<?
for s in servicesMap:
  service = s.data()
  if service.type == ServiceType.Control or service.taskName != currentTaskName:
    continue
  ?>
      bool run<!service.name!>(const std::string &clientName, int rqst_id,
	    const yarp::os::Bottle &command, yarp::os::Bottle &reply);
<?
?>

    private:
      <!comp.name()!>ControlData *m_data;
      // Request port
      yarp::os::BufferedPort<yarp::os::Bottle> m_request_port;
      yarp::os::BufferedPort<yarp::os::Bottle> m_reply_port;
};

#endif
 
