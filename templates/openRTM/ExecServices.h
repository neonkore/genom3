
#ifndef <!upperCompName!>_EXECSERVICES_H
#define <!upperCompName!>_EXECSERVICES_H

#include "ControlTaskSkel.h"

class <!capCompName!>ControlData;

<?
for s in comp.servicesMap():
  service = s.data()
  if service.type == ServiceType.Control:
    continue

  statusStr = ""
  first = True
  for c in service.codels():
    if first:
      first = False
    else:
      statusStr += ", "
    statusStr += upper(service.name) + "_" + upper(c.key())
      
    if len(service.output) > 0:
      inputStr = ", CORBA::Long id"
    else:
      inputStr = ""
    for s in service.inputs():
      inputStr += ", "
      t = comp.typeFromIdsName(s)
      inputStr += MapTypeToCpp(t) + " " + s

    if len(service.output) > 0:
      t = comp.typeFromIdsName(service.output)
      outputType = MapTypeToCpp(t)
  ?>
class <!service.name!>Service {
  public:
//     enum Status { <!statusStr!>}

    <!service.name!>Service(<!capCompName!>ControlData *data <!inputStr!>);

    int id() const { return m_id; }
    bool step();
<?
  if len(service.output) > 0:?>
    <!outputType!> result() { return out_<!service.output!>; }
<?
  for c in service.codels():
    if c.key() == "control":
      continue
    print "    int " + c.key() + "();" 
  ?>

  private:
    int m_id;
    int m_status;
<?
  for s in service.inputs():
    t = comp.typeFromIdsName(s)
    print "    " + MapTypeToCpp(t) + " in_" + s + ";"    
  if len(service.output) > 0:
    print "    " + outputType + " out_" + service.output + ";"
  ?>
    <!capCompName!>ControlData *m_data;
}; 

<?
?>

#endif
