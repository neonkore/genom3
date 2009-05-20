
#include "<!comp.name()!>ControlImpl.h"

#include "<!comp.name()!>Control.h"
#include "userCodels.h"
#include "ExecServices.h"

// forward declaration of user codels
<?
for s in servicesMap:
  service = s.data()
  if service.hasCodel("control"):
    print "int " + real_codel_signature(service.codel("control"), service) + ";"
?>

<!capCompName!>ControlImpl::<!capCompName!>ControlImpl(<!capCompName!>ControlData *data)
: m_data(data)
{}


<!capCompName!>ControlImpl::~<!capCompName!>ControlImpl()
{}

<?
for s in servicesMap:
  service = s.data()
  if service.type == ServiceType.Control:
    ?>
<!service_cpp_signature(service, capCompName + "ControlImpl")!>
{
<?
    if service.hasCodel("control"):
	?>
  int res = <!real_codel_call(service.codel("control"), "", service)!>;
  if(res < 0) //error
      return;

  // kill existing incompatible services
<?
    if len(service.incompatibleServices()) > 0 and service.incompatibleServices()[0] != "none":
      if service.incompatibleServices()[0] == "all":
	print "  m_data->killAllServices();"
      else:
	for incomp in service.incompatibleServices():
	  print "  m_data->kill" + incomp + "Services();" 

    for i in service.inputs():
	print "  m_data->" + i.identifier + " = in_" + i.identifier + ";" 
    if len (service.output) > 0:
	print "  return m_data->" + service.output + ";"
    ?>
}

<?
prefix = capCompName + "ControlImpl"
for s in comp.servicesMap():
  service = s.data()
  if service.type != ServiceType.Control:
    if len(service.output) > 0:
      returnStr = "  return m_data->" + service.output + ";"
    else:
      returnStr = "  return;"

    if len(service.output) > 0:
      args = ", m_serviceCount++ "
    else:
      args = ""
    for i in service.inputs():
      args += ", in_" + i.identifier
    ?>
<!service_cpp_signature(service, prefix)!>
{
<?
    if len(service.output) > 0: ?>
  static long m_serviceCount = 0;
<?
    if service.hasCodel("control"):
	?>
  int res = <!real_codel_call(service.codel("control"), "m_data->", service)!>;
  if(res < 0) // error
      <!returnStr!>
<?
    ?>
  // kill existing incompatible services
<?
    if len(service.incompatibleServices()) > 0 and service.incompatibleServices()[0] != "none":
      if service.incompatibleServices()[0] == "all":
	print "  m_data->killAllServices();"
      else:
	for incomp in service.incompatibleServices():
	  print "  m_data->kill" + incomp + "Services();" 
    ?>

  // create the activity
  <!service.name!>Service::Ptr s = <!service.name!>Service::Ptr(new <!service.name!>Service(m_data <!args!>));
  m_data-><!service.name!>Services.push_back(s);
<?
    if len(service.output) > 0: ?>
  return m_serviceCount;<?
    ?>
}
<?
?>


