<?
currentTask = comp.task(currentTaskName)
?>
#include "<!comp.name()!><!currentTaskName!>Impl.h"

// forward declaration of user codels
<?
for s in servicesMap:
  service = s.data()
  if service.type == ServiceType.Exec and service.hasCodel("control"):
    print "int " + real_codel_signature(service.codel("control"), service) + ";"
?>

<!capCompName!><!currentTaskName!>Impl::<!capCompName!><!currentTaskName!>Impl()
: m_data(0)
{}


<!capCompName!><!currentTaskName!>Impl::~<!capCompName!><!currentTaskName!>Impl()
{}

void <!capCompName!><!currentTaskName!>Impl::setData(<!capCompName!>ControlData *data)
{
  m_data = data;
}

//  I<!capCompName!><!currentTaskName!> interface
<?
prefix = capCompName + currentTaskName + "Impl"
for s in comp.servicesMap():
  service = s.data()
  if service.type != ServiceType.Control and service.taskName == task.name:
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
  m_<!service.name!>Services.push_back(s);
  return m_serviceCount;
}
<?
?>
