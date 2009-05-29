
#ifndef <!upperCompName!>_EXECSERVICES_H
#define <!upperCompName!>_EXECSERVICES_H

#include <list>
#include <boost/shared_ptr.hpp>

class <!comp.name()!>ControlData;

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
      
    inputStr = "";
    for i in service.inputs():
      inputStr += ", "
      t = inputType(i)
      inputStr += MapTypeToCpp(t) + " " + i.identifier

    if service.output.identifier:
      t = inputType(service.output)
      outputType = MapTypeToCpp(t)
  ?>
class <!service.name!>Service {
  public:
//     enum Status { <!statusStr!>}
    typedef boost::shared_ptr<<!service.name!>Service> Ptr;
    typedef std::list<Ptr> List;

    <!service.name!>Service(<!capCompName!>ControlData *data,
	int id, std::string clientName <!inputStr!>);

    int id() const { return m_id; }
    std::string clientName() const { return m_clientName; }

    bool step();
<?
  if service.output.identifier:?>
    <!outputType!> result() { return out_<!service.output.identifier!>; }
<?
  for c in service.codels():
    if c.key() == "control":
      continue
    print "    int " + c.key() + "();" 
  ?>

  private:
    int m_id;
    int m_status;
    std::string m_clientName;
<?
  for i in service.inputs():
    t = inputType(i)
    print "    " + MapTypeToCpp(t) + " in_" + i.identifier + ";"    
  if service.output.identifier:
    print "    " + outputType + " out_" + service.output.identifier + ";"
  ?>
    <!capCompName!>ControlData *m_data;
}; 

<?
?>

#endif
