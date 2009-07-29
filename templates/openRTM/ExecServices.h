
#ifndef <!upperCompName!>_EXECSERVICES_H
#define <!upperCompName!>_EXECSERVICES_H

#include <list>
#include <boost/shared_ptr.hpp>
#include "<!comp.name()!>Struct.h"

class <!capCompName!>ControlData;

<?
for s in comp.servicesMap():
  service = s.data()
  if service.type == Service.Control:
    continue

  statusStr = ""
  first = True
  for c in service.codels():
    if first:
      first = False
    else:
      statusStr += ", "
    statusStr += upper(service.name) + "_" + upper(c.key())
      
    inputStr = ", int id"
    for i in service.inputs():
      inputStr += ", "
      t = input_type(i)
      inputStr += MapTypeToCpp(t) + " " + i.identifier

    if service.output.identifier:
      t = input_type(service.output)
      outputType = MapTypeToCpp(t)
  ?>
class <!service.name!>Service {
  public:
//     enum Status { <!statusStr!>}
    typedef boost::shared_ptr<<!service.name!>Service> Ptr;
    typedef std::list<Ptr> List;

    <!service.name!>Service(<!capCompName!>ControlData *data <!inputStr!>);
    ~<!service.name!>Service();

    int id() const { return m_id; }
    bool step();
    void abort();

<?
  for c in service.codels():
    if c.key() == "control":
      continue
    print "    int " + c.key() + "();" 
  ?>

  private:
    bool m_aborted;
    int m_id;
    int m_status;
<?
  for i in service.inputs():
    t = input_type(i)
    print "    " + MapTypeToCpp(t) + " in_" + i.identifier + ";"    
  if service.output.identifier:
    print "    " + outputType + " out_" + service.output.identifier + ";"
  ?>
    <!capCompName!>ControlData *m_data;
}; 

<?
?>

#endif
