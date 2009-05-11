
#include "<!comp.name()!>ControlImpl.h"

/*
 * Example implementational code for IDL interface MyService
 */
<!capCompName!>ControlImpl::<!capCompName!>ControlImpl()
{}


<!capCompName!>ControlImpl::~<!capCompName!>ControlImpl()
{}

<?
for s in servicesMap:
  service = s.data()
  if service.type == ServiceType.Control:
    ?>
<!service_cpp_signature(service)!>
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
?>

<?
for t in comp.tasksMap():
    task = t.data()
    print "//  I" + capCompName + task.name + " interface"
    for s in comp.servicesMap():
      service = s.data()
      if service.type != ServiceType.Control and service.taskName == task.name:
	  if len(service.output) > 0:
	    returnStr = "return "
	  else:
	    returnStr = ""
	  ?>
<!service_cpp_signature(service, capCompName + "ControlImpl")!>
{
    <!returnStr!>m_<!task.name!>ConsumerService-><!service_call(service)!>;
}

<?
?>
