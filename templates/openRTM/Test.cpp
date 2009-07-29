<?
def parse_input(type, name):
  if type.kind() == IdlType.Enum:?>
  {
      int tmp;
      cin >> tmp;
      <!name!> = (<!MapTypeToCorbaCpp(type, True)!>) tmp;
  }
<?
  elif type.kind() == IdlType.String: 
    s = type.asStringType()
    ?>
  {
      std::string tmp;
      cin >> tmp;
      <!name!> = CORBA::string_alloc(tmp.length());
      strncpy(<!name!>, tmp.c_str(), tmp.length() + 1);
  }
<?
  else:
    print "cin >> " + name + ";"
?>
#include "<!comp.name()!>Test.h"
#include "<!comp.name()!>Printer.h"

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
    ?>
//   registerOutPort("<!port.name!>", m_data.<!port.name!>);
<?
?>
  // Set OutPort buffers
<?
for port in outports:
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

void <!capCompName!>Test::printUsage()
{
  // print usage
  cout << "Available actions:" << endl;
  cout << "  (0) Exit" << endl;
  
<?
idx = 10
for s in servicesMap:
  service = s.data()
  idx += 1
  print "  cout << \"  (" + str(idx) + ") " + service.name + "\" << endl;"

print "  cout << \"---------------------------\" << endl;"
for port in outports:
  idx += 1
  print "  cout << \"  (" + str(idx) + ") Show " + port.name + "\" << endl;"
print "  cout << \"---------------------------\" << endl;"
for port in inports:
  idx += 1
  print "  cout << \"  (" + str(idx) + ") Update " + port.name + "\" << endl;"
?>

}

RTC::ReturnCode_t <!capCompName!>Test::onExecute(RTC::UniqueId ec_id)
{
  printUsage();

  int action;  
  std::cin >> action;
  switch(action) {
    case 0:
	exit();

<?
idx = 10
for s in servicesMap:
  service = s.data()
  idx += 1
  ?>
    case <!idx!>: {
      run<!service.name!>();
      break;
    }
<?
for port in outports:
  idx += 1
  ?>
    case <!idx!>: {
      read<!port.name!>();
      break;
    }
<?
?> 
  }

  return RTC::RTC_OK;
}

<?
idx = 0
for s in servicesMap:
  service = s.data()
  idx += 1
  inputFlatList = input_list(service)
  serviceArgs = ""
  for i in service.inputs():
    serviceArgs += i.identifier + ", "
  serviceArgs = serviceArgs[:-2]
  ?>
void <!capCompName!>Test::run<!service.name!>()
{
<?
  for i in service.inputs():
    input = MapTypeToCorbaCpp(input_type(i), True)
    print "      " + input + " " + i.identifier + ";";

  for i in service.inputs():
    flat = flat_struct(input_type(i), i.identifier, ".", i.defaultValue, i.doc)
    if i.defaultValue.isEmpty() or i.defaultValue.kind() == Literal.Struct:
      defValue = ""
    else :
      defValue = "(default : \" << " + i.defaultValue.toString() + " << \")"
    ?>
     cout << "Enter <!i.identifier!> (<!i.doc.doc!>) <!defValue!> :" << endl;
<?
    for x in flat:
      t = MapTypeToCpp(x[0], True)
      if len(flat) > 1: 
	if x[2] is None or x[2].isEmpty():
	  value = ""
	else:
	  value = "(default : \" << " + x[2].toString() + " << \")"

	if not x[3] is None:
	  docStr = ": " + x[3].doc
	else:
	  docStr = ""
	?>
      cout << "Enter <!t!> <!x[1]!> <!docStr!> <!value!>:  " << endl;
<?
    parse_input(x[0], x[1]);

  if service.type == Service.Control:
    if service.output.identifier:
      outputType = MapTypeToCorbaCpp(input_type(service.output), True)    
      if is_corba_dynamic(input_type(service.output)):
	outputName = "*out"
      else:
	outputName = "out"
      ?>
    try {
      <!outputType!> <!outputName!> = m_service-><!service.name!>(<!serviceArgs!>);
      cout << endl << "Result: ";
      Printer<<!outputType!>>::print(<!outputName!>);
      cout << endl;
    } catch (CORBA::UserException& ex) {
      cout << "Error when calling service \"<!service.name!>\": " << ex._name() << endl;
      return;
    }
<?
      if is_corba_dynamic(input_type(service.output)):?>
      delete out;
<?
    else:?>
    try {
      m_service-><!service.name!>(<!serviceArgs!>);
    } catch (CORBA::UserException& ex) {
      cout << "Error when calling service \"<!service.name!>\": " << ex._name() << endl;
      return;
    }
<?
  else:
    ?>
      try {
	cout << endl << "Started activity " << m_service-><!service.name!>(<!serviceArgs!>) << endl;
      } catch (CORBA::UserException& ex) {
	cout << "Error when calling service \"<!service.name!>\" control codel: " << ex._name() << endl;
	return;
      }

      while(!m_<!service.name!>_inport.isNew())
	coil::usleep(1000);

      m_<!service.name!>_inport.read();
      cout << "Received final answer : " << m_<!service.name!>_data.res << " from \"<!service.name!>\" with id: ";
      cout << m_<!service.name!>_data.id << endl;
<?
    if service.output.identifier:
      outputType = MapTypeToCorbaCpp(input_type(service.output), True)
      ?>
      cout << "Result is : ";
      Printer<<!outputType!>>::print(m_<!service.name!>_data.data);
      cout << endl;
<?
  ?>
}
<?
?>      


<?
for port in outports:
  typeName = MapTypeToCorbaCpp(port.idlType)
  ?>
void <!capCompName!>Test::read<!port.name!>()
{
  m_<!port.name!>.update();
  cout << endl << "<!port.name!> :" << endl;
  Printer<<!typeName!>>::print(m_<!port.name!>_data.data);
  cout << endl;
}
<?
?>

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



