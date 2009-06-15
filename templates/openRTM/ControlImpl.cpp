
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
	codelLock(service.codel("control"), service)
	?>
  int res = <!real_codel_call(service.codel("control"), "", service)!>;
<?
	codelRelease(service.codel("control"), service)
	?>
  if(res < 0) //error
      return;

  // kill existing incompatible services
<?
    if service.incompatibleServices() and service.incompatibleServices()[0] != "none":
      if service.incompatibleServices()[0] == "all":
	print "  m_data->killAllServices();"
      else:
	for incomp in service.incompatibleServices():
	  print "  m_data->kill" + incomp + "Services();" 

    if service.inputs():
      if service.inputs()[0].kind == ServiceInputKind.IDSMember: ?>
  m_data->idsMutex.acquire_write();
  m_data-><!service.inputs()[0].identifier!> = in_<!service.inputs()[0].identifier !>; 
  m_data->idsMutex.release();
<?
    if service.output.identifier:?>
  return m_data-><!service.output.identifier!>;
<?
    ?>
}

<?
prefix = capCompName + "ControlImpl"
for s in comp.servicesMap():
  service = s.data()
  if service.type != ServiceType.Control:
    if service.output.identifier:
      returnStr = "  return m_data->" + service.output.identifier + ";"
    else:
      returnStr = "  return;"

    if service.output.identifier:
      args = ", m_serviceCount++ "
    else:
      args = ""
    for i in service.inputs():
      args += ", in_" + i.identifier
    ?>
<!service_cpp_signature(service, prefix)!>
{
<?
    if service.output.identifier: ?>
  static long m_serviceCount = 0;
<?
    if service.hasCodel("control"):
	codelLock(service.codel("control"), service)
	?>
  int res = <!real_codel_call(service.codel("control"), "m_data->", service)!>;
<?
	codelRelease(service.codel("control"), service)
	?>
  if(res < 0) // error
      <!returnStr!>
<?
    ?>
  // kill existing incompatible services
<?
    if service.incompatibleServices() and service.incompatibleServices()[0] != "none":
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
    if service.output.identifier: ?>
  return m_serviceCount;<?
    ?>
}
<?
?>


