<?
currentTask = comp.task(currentTaskName)
?>

#ifndef <!upperCompName!>_<!upper(currentTaskName)!>_IMPL_H
#define <!upperCompName!>_<!upper(currentTaskName)!>_IMPL_H

#include "I<!comp.name()!><!currentTaskName!>Skel.h"

/*
 * Example class implementing IDL interface MyService
 */
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

   // I<!capCompName!><!currentTaskName!> interface
<?
for s in comp.servicesMap():
  service = s.data()
  if service.type != ServiceType.Control and service.taskName == currentTaskName:
    print "   " + service_cpp_signature(service) + ";"
?>
};

#endif // <!upperCompName!>_<!upper(currentTaskName)!>_IMPL_H

