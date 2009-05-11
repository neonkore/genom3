<?
currentTask = comp.task(currentTaskName)
?>

#ifndef <!upperCompName!>_<!upper(currentTaskName)!>_IMPL_H
#define <!upperCompName!>_<!upper(currentTaskName)!>_IMPL_H

#include <list>

#include "I<!comp.name()!><!currentTaskName!>Skel.h"

class <!capCompName!>ControlData;

<?
for s in comp.servicesMap():
  service = s.data()
  if service.type != ServiceType.Control and service.taskName == currentTaskName:
    print "class " + service.name + "Service;"
?>

class <!capCompName!><!currentTaskName!>Impl
 : public virtual POA_I<!capCompName!><!currentTaskName!>,
   public virtual PortableServer::RefCountServantBase
{
  private:
   // Make sure all instances are built on the heap by making the
   // destructor non-public
   //virtual ~<!capCompName!><!currentTaskName!>Impl();

  public:
    // standard constructor
    <!capCompName!><!currentTaskName!>Impl();
    virtual ~<!capCompName!><!currentTaskName!>Impl();

    void setData(<!capCompName!>ControlData *data);

   // I<!capCompName!><!currentTaskName!> interface
<?
for s in comp.servicesMap():
  service = s.data()
  if service.type != ServiceType.Control and service.taskName == currentTaskName:
    print "   " + service_cpp_signature(service) + ";"
?>

   // Activities list
<?
for s in comp.servicesMap():
  service = s.data()
  if service.type != ServiceType.Control and service.taskName == currentTaskName:
    ?>
   std::list<<!service.name!>Service*> m_<!service.name!>Services;
<?
?>
  private:
    <!capCompName!>ControlData *m_data;
};

#endif // <!upperCompName!>_<!upper(currentTaskName)!>_IMPL_H

