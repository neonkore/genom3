<?
currentTask = comp.task(currentTaskName)
?>

#include "<!comp.name()!><!currentTaskName!>Impl.h"

<!capCompName!><!currentTaskName!>Impl::<!capCompName!><!currentTaskName!>Impl
{}


<!capCompName!><!currentTaskName!>Impl::~<!capCompName!><!currentTaskName!>Impl()
{}

//  I<!capCompName!><!currentTaskName!> interface
<?
prefix = capCompName + currentTaskName + "Impl"
for s in comp.servicesMap():
  service = s.data()
  if service.type != ServiceType.Control and service.taskName == task.name:
    if len(service.output) > 0:
      returnStr = "  return m_" + service.output + ";"
    else:
      returnStr = "  return;"
    ?>
<!service_cpp_signature(service, prefix)!>
{
<?
    if service.hasCodel("control"):
	?>
  int res = <!codel_call(service.codel("control"), service)!>;
  if(res == ERROR)
      <!returnStr!>
<?
    for s in service.inputs():
	print "  m_" + s + " = in_" + s + ";" 
    if len (service.output) > 0:
	print returnStr
    ?>
}
<?
?>
