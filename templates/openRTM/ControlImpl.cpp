
#include "<!capCompName!>ControlImpl.h"

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
	print "return m_" + service.output + ";"
    ?>
}
<?
?>  
