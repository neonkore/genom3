
#include "<!comp.name()!>ControlImpl.h"

#include "<!comp.name()!>Control.h"
#include "userCodels.h"
#include "ExecServices.h"


void throwError(int e)
{
  switch(e) {
<?
errorSet = createErrorList()
for e in errorSet:
    print "    case ERROR_" + e + ": throw " + e + "();"
?>
    case USER_OK:  return;
    default:
	return;
  }
}
  

// forward declaration of user codels
extern "C" {
<?
for s in servicesMap:
  service = s.data()
  if service.hasCodel("control"):
    print "int " + real_codel_signature(service.codel("control"), service) + ";"
?>
}

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
	copyCodelArgs(service.codel("control"), service)
	codelLock(service.codel("control"), service)
	?>
  int res = <!real_codel_call(service.codel("control"), "m_data->", service)!>;
<?
	codelRelease(service.codel("control"), service)
	copyCodelArgsReverse(service.codel("control"), service)
	?>
  if(res < 0) //error
      throwError(res);

  // kill existing incompatible services
<?
    if service.incompatibleServices() and service.incompatibleServices()[0] != "none":
      if service.incompatibleServices()[0] == "all":
	print "  m_data->killAllServices();"
      else:
	for incomp in service.incompatibleServices():
	  print "  m_data->kill" + incomp + "Services();" 

    if service.inputs():
      if service.inputs()[0].kind == ServiceInputKind.IDSMember: 
	?>
  m_data->idsMutex.acquire_write();
<?
	copyTypeFromCorba(inputType(service.inputs()[0]), "in_" + service.inputs()[0].identifier, "m_data->" + service.inputs()[0].identifier, False)
	?> 
  m_data->idsMutex.release();
<?
    if service.output.identifier:
      outputType = inputType(service.output)
      output = MapTypeToCorbaCpp(outputType, True)
    
      if needsConversion(outputType):
	if isCorbaDynamic(outputType):
	  outputName = "(*out)"
	  print "  " + output + " *out = new " + output + ";"
	else:
	  outputName = "out"
	  print "  " + output + " out;"
	copyTypeFromCorba(outputType, "m_data->" + service.output.identifier, outputName, True)
	?>
  return out;
<?
      else: ?>
  return m_data-><!service.output.identifier!>;
<?
    ?>
}

<?
prefix = capCompName + "ControlImpl"
for s in comp.servicesMap():
  service = s.data()
  if service.type != ServiceType.Control:
    args = ", m_serviceCount++ "
    for i in service.inputs():
      if needsConversion(inputType(i)):
	args += ", " + i.identifier
      else:
	args += ", in_" + i.identifier
    ?>
<!service_cpp_signature(service, prefix)!>
{
  static long m_serviceCount = 0;
<?
    copyCodelArgs(service.codel("control"), service)
    if service.hasCodel("control"):
	codelLock(service.codel("control"), service)
	?>
  int res = <!real_codel_call(service.codel("control"), "m_data->", service)!>;
<?
	codelRelease(service.codel("control"), service)
	copyCodelArgsReverse(service.codel("control"), service)
	?>
  if(res < 0) // error
      throwError(res);
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

    if service.inputs():
      if service.inputs()[0].kind == ServiceInputKind.IDSMember: 
	?>
  m_data->idsMutex.acquire_write();
<?
	copyTypeFromCorba(inputType(service.inputs()[0]), "in_" + service.inputs()[0].identifier, "m_data->" + service.inputs()[0].identifier, False)
	?> 
  m_data->idsMutex.release();
<?
    ?>

  // create the activity
  <!service.name!>Service::Ptr s = <!service.name!>Service::Ptr(new <!service.name!>Service(m_data <!args!>));
  m_data-><!service.name!>Services.push_back(s);
  return m_serviceCount;
}
<?
?>


