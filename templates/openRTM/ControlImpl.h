
#include "I<!comp.name()!>ControlSkel.h"


#ifndef <!upperCompName!>_CONTROL_IMPL_H
#define <!upperCompName!>_CONTROL_IMPL_H
 
/*
 * Example class implementing IDL interface MyService
 */
class <!capCompName!>ControlImpl
 : public virtual POA_I<!capCompName!>Control,
   public virtual PortableServer::RefCountServantBase
{
 private:
   // Make sure all instances are built on the heap by making the
   // destructor non-public
   //virtual ~<!capCompName!>ControlImpl();

 public:
   // standard constructor
   <!capCompName!>ControlImpl();
   virtual ~<!capCompName!>ControlImpl();

   // attributes and operations
<?
for s in servicesMap:
  service = s.data()
  if service.type == ServiceType.Control:
    print "  " + service_cpp_signature(service) + ";"
?>   
};

#endif // <!upperCompName!>_CONTROL_IMPL_H

