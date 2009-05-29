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
?>

class <!comp.name()!><!currentTaskName!> : public yarp::os::<!taskBaseClass!>
{
    public:
      <!comp.name()!><!currentTaskName!>();
      ~<!comp.name()!><!currentTaskName!>();

    virtual void run();
<?
if currentTask.hasCodel("init"): ?>
    virtual bool threadInit();
<?
?>
    private:
      <!comp.name()!>ControlData *m_data;
};

#endif
 
