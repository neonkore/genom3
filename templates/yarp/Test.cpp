<?
def parseInput(type, name):
  if type.kind() == IdlKind.Enum:?>
      int tmp;
      cin >> tmp;
      <!name!> = (<!MapTypeToC(type, True)!>) tmp;
<?
  else: ?>
      cin >> <!name!>;
<?
?>
#include <iostream>
#include <yarp/os/all.h>
#include "<!comp.name()!>Struct.hpp"

using namespace std;
using namespace yarp::os;


<?
for t in tasksMap:
  ?>
class <!task.name!>ReplyReader : public PortReader
{
    virtual bool read(ConnectionReader& connection) 
    {
	Bottle b;
	b.read(connection);

	string req = ReplyReader::readRequestName(&b);
<?
  for s in servicesMap:
    service = s.data()
    if service.type == ServiceType.Control or service.taskName != task.name:
      continue
    serviceInfo = services_info_dict[service.name]
    if not serviceInfo.outputFlag:
      continue
  ?>
	if(req == "<!service.name!>") {
	    ReplyAnswer<<!serviceInfo.outputTypeCpp!>> answer;
	    cout << "Final reply: " << answer << endl;
	    return true;
	}
<?
  ?>
	cout << "Unknown reply Received" << endl;
	return true;
    }
};
<?
?>

class <!comp.name()!>Test {

public:

<?
for t in tasksMap:
  task = t.data()
  ?>
  yarp::os::BufferedPort<yarp::os::Bottle> <!task.name!>_req_port;
  yarp::os::BufferedPort<yarp::os::Bottle> <!task.name!>_reply_port;
  <!task.name!>ReplyReader <!service.name!>Reader;
<?
?>
  yarp::os::BufferedPort<yarp::os::Bottle> Control_req_port;
//   yarp::os::BufferedPort<yarp::os::Bottle> Control_reply_port;

<!comp.name()!>Test()
{
    Control_req_port.open("/<!comp.name()!>/Test/Services/Control");
    Network::connect("/<!comp.name()!>/Test/Services/Control", "/<!comp.name()!>/Services/Control");
//     Control_req_port.open("/<!comp.name()!>/Test/Control_reply");
//     Network::connect("/<!comp.name()!>/", "/<!comp.name()!>/Control");
<?
for t in tasksMap:
  task = t.data()
  ?>
    <!task.name!>_req_port.open("/<!comp.name()!>/Test/Services/<!task.name!>");
    Network::connect("/<!comp.name()!>/Test/Services/<!task.name!>", "/<!comp.name()!>/Services/<!task.name!>");
    <!task.name!>_reply_port.open("/<!comp.name()!>/Test/Services/Replies/<!task.name!>");
    Network::connect("/<!comp.name()!>/Services/Replies/<!task.name!>", "/<!comp.name()!>/Test/Services/Replies/<!task.name!>");
    <!task.name!>_reply_port.setReader(<!task.name!>Reader);
<?
?>
}

void printUsage()
{
  // print usage
  cout << "Available actions:" << endl;
<?
idx = 0
for s in servicesMap:
  service = s.data()
  idx += 1
  print "  cout << \"  (" + str(idx) + ") " + service.name + "\" << endl;"
?>

}

void executeAction(int action)
{
  switch(action) {
<?
idx = 0
for s in servicesMap:
  service = s.data()
  serviceInfo = services_info_dict[service.name]
  idx += 1
  inputFlatList = inputList(service)
  serviceArgs = "0"
  if serviceInfo.inputFlag:
    serviceArgs = serviceInfo.inputName 
  ?>
    case <!idx!>: {
<?
  for i in service.inputs():
    print "      " + MapTypeToC(inputType(i), True) + " " + i.identifier + ";";
  if service.output.identifier and service.type == ServiceType.Control:
    print "      " + MapTypeToC(inputType(service.output), True) + " " + service.output.identifier + ";";

  for x in inputFlatList:
    t = MapTypeToC(x[0], True)
    ?>
      cout << "Enter <!t!> <!x[1]!>:  " << endl;
<?
    parseInput(x[0], x[1]);

  if service.type == ServiceType.Control:
    ?>
      RqstWriter<<!serviceInfo.requestType!>>::send(Control_req_port, "/<!comp.name()!>/Test", "<!service.name!>", <!serviceArgs!>);
      ReplyAnswer<<!serviceInfo.replyType!>> answer(Control_req_port.read());
      cout << "Final reply: " << answer;
<?
  else:?>
      RqstWriter<<!serviceInfo.requestType!>>::send(<!service.taskName!>_req_port, "/<!comp.name()!>/Test", "<!service.name!>", <!serviceArgs!>);
      ReplyAnswer<<!serviceInfo.replyType!>> answer(<!service.taskName!>_req_port.read());
      cout << "Intermediate reply: " << answer;

//       cout << "Wait final reply ?";
//       string res;
//       cin >> res;
//       if(res == "y") {
// 	  ReplyAnswer<<!serviceInfo.replyType!>> answer(<!service.taskName!>_reply_port.read());
// 	  cout << "Final reply: " << answer; 
//       }
<?
  ?>
      break;
    }
<?
?>      
  }
}

};

int main()
{
    yarp::os::Network yarp;

    Contact c = yarp.queryName("<!comp.name()!>");
    if(!c.isValid()) {
      cout << "Impossible to contact module." << endl;
      return 1;
    }

    <!comp.name()!>Test test;
    while(true) {
	int action;  
	std::cin >> action;
	test.executeAction(action);
    }
}



