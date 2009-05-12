
#include "<!comp.name()!>ControlImpl.h"

// forward declaration of user codels
<?
for s in servicesMap:
  service = s.data()
  if service.type == ServiceType.Control and service.hasCodel("control"):
    print "int " + real_codel_signature(service.codel("control"), service) + ";"
?>

<!capCompName!>ControlImpl::<!capCompName!>ControlImpl(<!capCompName!>ControlData *data)
: m_data(0)
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
  int res = <!codel_call(service.codel("control"), service)!>;
  if(res == ERROR)
      return;
<?
    for s in service.inputs():
	print "  m_" + s + " = in_" + s + ";" 
    if len (service.output) > 0:
	print "  return m_" + service.output + ";"
    ?>
}

<?
prefix = capCompName + "ControlImpl"
for s in comp.servicesMap():
  service = s.data()
  if service.type != ServiceType.Control:
    if len(service.output) > 0:
      returnStr = "  return m_data->m_" + service.output + ";"
    else:
      returnStr = "  return;"
    ?>
int <!service_cpp_args(service, prefix)!>
{
  static m_serviceCount = 0;
<?
    if service.hasCodel("control"):
	?>
  int res = <!real_codel_call(service.codel("control"), "m_data->", service)!>;
  if(res == ERROR)
      <!returnStr!>
<?
    ?>
  // create the activity
  <!service.name!>Service *s = new <!service.name!>Service(m_data, m_serviceCount++, dist);
  m_data->m_<!service.name!>Services.push_back(s);
  return m_serviceCount;
}
<?
?>


