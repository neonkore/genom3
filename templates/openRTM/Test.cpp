<?
def parseInput(type, name):
  if type.kind() == IdlKind.Enum:?>
      int tmp;
      cin >> tmp;
      <!name!> = (<!MapTypeToCpp(type, True)!>) tmp;
<?
  else:
    print "cin >> " + name + ";"
?>
#include "<!comp.name()!>Test.h"

#include <iostream>

using namespace std;

// Module specification
static const char* <!comp.name()!>Test_spec[] =
  {
    "implementation_id", "<!capCompName!>Test",
    "type_name",         "<!capCompName!>Test",
    "description",       "Test program for <!capCompName!> component",
    "version",           "<!comp.version!>",
    "vendor",            "AIST",
    "category",          "Generic",
    "activity_type",     "SPORADIC",
    "kind",              "<!comp.name()!>",
    "max_instance",      "10",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    ""
  };

<?
portInit = ""
for name,type in output_ports_map.iteritems():
  portInit += "    m_" + name + "_inport(\"" + name + "_inport\", m_" + name + "_data),\n" 
# for port in inports:
#   portInit += "    " + port.name + "(\"" + port.name + "\", m_" + port.name + "_data),\n" 
for port in outports:
  portInit += "    m_" + port.name + "(\"" + port.name + "\", m_" + port.name + "_data),\n" 
?>
<!capCompName!>Test::<!capCompName!>Test(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_servicePort("<!capCompName!>Test"),
<!portInit[:-2]!>
{
  // Set InPort buffers
<?
for port in inports:
    typeName = MapTypeToCpp(port.idlType)
    ?>
//   registerOutPort("<!port.name!>", m_data.<!port.name!>);
<?
?>
  // Set OutPort buffers
<?
for port in outports:
    typeName = MapTypeToCpp(port.idlType)
    ?>
  registerInPort("<!port.name!>", m_<!port.name!>);
<?
for name,typeName in output_ports_map.iteritems():
  ?>
  registerInPort("<!name!>_inport", m_<!name!>_inport);
<?
?>
  // Set service provider to Ports
  m_servicePort.registerConsumer("<!capCompName!>Service", "I<!capCompName!>", m_service);

  // Set CORBA Service Ports
  registerPort(m_servicePort);
}

<!capCompName!>Test::~<!capCompName!>Test()
{
}

/*
RTC::ReturnCode_t <!capCompName!>Control::onFinalize()
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!>Control::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!>Control::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!>Control::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!comp.name()!>Control::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

RTC::ReturnCode_t <!capCompName!>Test::onExecute(RTC::UniqueId ec_id)
{
  int action;  
  // print usage
  cout << "Available actions:" << endl;
<?
idx = 0
for s in servicesMap:
  service = s.data()
  idx += 1
  print "  cout << \"  (" + str(idx) + ") " + service.name + "\" << endl;"
?>

  std::cin >> action;
  switch(action) {
<?
idx = 0
for s in servicesMap:
  service = s.data()
  idx += 1
  inputFlatList = inputList(service)
  serviceArgs = ""
  for i in service.inputs():
    serviceArgs += i.identifier + ", "
  serviceArgs = serviceArgs[:-2]
  ?>
    case <!idx!>: {
<?
  for i in service.inputs():
    print "      " + MapTypeToCpp(inputType(i), True) + " " + i.identifier + ";";
  if len(service.output) > 0 and service.type == ServiceType.Control:
    print "      " + MapTypeToCpp(comp.typeFromIdsName(service.output), True, True) + " " + service.output + ";";

  for x in inputFlatList:
    t = MapTypeToCpp(x[0], True)
    ?>
      cout << "Enter <!t!> <!x[1]!>:  ";
<?
    parseInput(x[0], x[1]);

  if service.type == ServiceType.Control:
    if len(service.output) > 0:
      outputType = MapTypeToCpp(comp.typeFromIdsName(service.output), True)
      ?>
      <!service.output!> = m_service-><!service.name!>(<!serviceArgs!>);
      cout << endl << "Result: " << <!service.output!> << endl;
<?
    else:?>
      m_service-><!service.name!>(<!serviceArgs!>);
<?
  else:
    if len(service.output) > 0:?>
      cout << endl << "Started activity " << m_service-><!service.name!>(<!serviceArgs!>) << endl;
<?
    else:?>
      m_service-><!service.name!>(<!serviceArgs!>);
      cout << endl << "Started activity " << endl;
<?
  ?>
      break;
    }
<?
?>      
  }
}

/*
RTC::ReturnCode_t <!capCompName!>Control::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!>Control::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!>Control::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!comp.name()!>Control::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/
/*
RTC::ReturnCode_t <!capCompName!>Control::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


extern "C"
{
 
  void <!capCompName!>TestInit(RTC::Manager* manager)
  {
    RTC::Properties profile(<!comp.name()!>Test_spec);
    manager->registerFactory(profile,
                             RTC::Create<<!capCompName!>Test>,
                             RTC::Delete<<!capCompName!>Test>);
  }
  
};



