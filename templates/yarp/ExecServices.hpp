
#ifndef <!upperCompName!>_EXECSERVICES_H
#define <!upperCompName!>_EXECSERVICES_H

#include <list>
#include <string>
#include <boost/shared_ptr.hpp>
#include <yarp/os/all.h>

#include "<!comp.name()!>Struct.hpp"
#include "lib/Events.hpp"

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
class <!service.name!>Service
<?
  if service.events():
    print ": public GenomYarp::EventReceiver"
  ?>
{
  public:
//     enum Status { <!statusStr!>}
    typedef boost::shared_ptr<<!service.name!>Service> Ptr;
    typedef std::list<Ptr> List;

    <!service.name!>Service(<!comp.name()!>ControlData *data,
	int id, std::string clientName, yarp::os::BufferedPort<yarp::os::Bottle> &replyPort <!inputStr!>);
    ~<!service.name!>Service();

    int id() const { return m_id; }
    std::string clientName() const { return m_clientName; }
<?
  if service.events(): ?>
    static int statusFromEventString(const std::string &ev);
<?
  ?>
    bool step();
    void abort();

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
    bool m_aborted;
    int m_id;
    std::list<int> m_status;
    std::string m_clientName;
<?
  for i in service.inputs():
    t = inputType(i)
    print "    " + MapTypeToCpp(t) + " in_" + i.identifier + ";"    
  if service.output.identifier:
    print "    " + outputType + " out_" + service.output.identifier + ";"
  ?>
    <!comp.name()!>ControlData *m_data;
    yarp::os::BufferedPort<yarp::os::Bottle> &m_replyPort;
}; 

<?
?>

#endif
