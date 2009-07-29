
#ifndef <!upperCompName!>_CONTROL_IMPL_H
#define <!upperCompName!>_CONTROL_IMPL_H

#include "<!comp.name()!>Struct.h"

class <!capCompName!>ControlData;
 
/*
 * Example class implementing IDL interface MyService
 */
class <!capCompName!>ControlImpl
 : public virtual POA_I<!capCompName!>,
   public virtual PortableServer::RefCountServantBase
{
 private:
   // Make sure all instances are built on the heap by making the
   // destructor non-public
   //virtual ~<!capCompName!>ControlImpl();

 public:
   // standard constructor
   <!capCompName!>ControlImpl(<!capCompName!>ControlData *data);
   virtual ~<!capCompName!>ControlImpl();

   // I<!capCompName!>Control interface
<?
for s in servicesMap:
  service = s.data()
  if service.type == Service.Control:
    print "   " + service_cpp_signature(service) + ";"

for t in comp.tasksMap():
    task = t.data()
    for s in comp.servicesMap():
      service = s.data()
      if service.type != Service.Control and service.taskName == task.name:
	  print "   " + service_cpp_signature(service) + ";"
?>
  private:
    <!capCompName!>ControlData *m_data;
};

#endif // <!upperCompName!>_CONTROL_IMPL_H

