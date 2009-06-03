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
#include "lib/RepliesReader.hpp"
#include "lib/RqstWriter.hpp"
#include "lib/DataServer.hpp"

using namespace std;
using namespace yarp::os;
using namespace GenomYarp;

<?
for t in tasksMap:
  task = t.data()
  ?>
class <!task.name!>ReplyReader : public TypedReaderCallback<Bottle>
{
    virtual void onRead(Bottle& b)
    {
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
	    ReplyAnswer<<!serviceInfo.outputTypeCpp!>> answer(&b);
	    cout << "Received reply: " << answer << endl;
	    return;
	}
<?
  ?>
	ReplyAnswer<VoidIO> answer(&b);
	cout << "Received reply: " << answer << endl;
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
  <!task.name!>ReplyReader <!task.name!>Reader;
<?
?>
  yarp::os::BufferedPort<yarp::os::Bottle> Control_req_port;
  yarp::os::BufferedPort<yarp::os::Bottle> Control_reply_port;
<?
for port in outports:
  typeName = MapTypeToC(port.idlType)
  ?>
  DataServer<<!typeName!>> <!port.name!>_inport;
<?
?>

<!comp.name()!>Test()
{
    Control_req_port.open("/<!comp.name()!>/Test/Services/Control");
    Network::connect("/<!comp.name()!>/Test/Services/Control", "/<!comp.name()!>/Services/Control");
    Control_reply_port.open("/<!comp.name()!>/Test/Services/Replies/Control");
    Network::connect("/<!comp.name()!>/Services/Replies/Control", "/<!comp.name()!>/Test/Services/Replies/Control");
<?
for t in tasksMap:
  task = t.data()
  ?>
    <!task.name!>_req_port.open("/<!comp.name()!>/Test/Services/<!task.name!>");
    Network::connect("/<!comp.name()!>/Test/Services/<!task.name!>", "/<!comp.name()!>/Services/<!task.name!>");
    <!task.name!>_reply_port.open("/<!comp.name()!>/Test/Services/Replies/<!task.name!>");
    Network::connect("/<!comp.name()!>/Services/Replies/<!task.name!>", "/<!comp.name()!>/Test/Services/Replies/<!task.name!>");
    <!task.name!>_reply_port.useCallback(<!task.name!>Reader);
<?
for port in outports: ?>
    <!port.name!>_inport.open("/<!comp.name()!>/Test/InPorts/<!port.name!>");
    Network::connect("/<!comp.name()!>/OutPorts/<!port.name!>", "/<!comp.name()!>/Test/InPorts/<!port.name!>");
<?
?>
}

void printUsage()
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
?>

}

<?
for s in servicesMap:
  service = s.data()
  serviceInfo = services_info_dict[service.name]
  idx += 1
  inputFlatList = inputList(service)
  serviceArgs = "input"
  if serviceInfo.inputFlag:
    serviceArgs = serviceInfo.inputName 
  ?>
void run<!service.name!>()
{
  static int rqst_id = 0;
<?
  for i in service.inputs():
    print "      " + MapTypeToC(inputType(i), True) + " " + i.identifier + ";";
  if service.output.identifier and service.type == ServiceType.Control:
    print "      " + MapTypeToC(inputType(service.output), True) + " " + service.output.identifier + ";";
  if not serviceInfo.inputFlag: ?>
      VoidIO input; // fake input object for the rqst writer
<?
  for x in inputFlatList:
    t = MapTypeToC(x[0], True)
    ?>
      cout << "Enter <!t!> <!x[1]!>:  " << endl;
<?
    parseInput(x[0], x[1]);

  if service.type == ServiceType.Control:
    ?>
      RqstWriter<<!serviceInfo.requestType!>>::send(Control_req_port, "/<!comp.name()!>/Test", rqst_id++, "<!service.name!>", <!serviceArgs!>);
      ReplyAnswer<<!serviceInfo.replyType!>> answer(Control_reply_port.read());
      cout << "Final reply: " << answer;
<?
  else:?>
      RqstWriter<<!serviceInfo.requestType!>>::send(<!service.taskName!>_req_port, "/<!comp.name()!>/Test", rqst_id++, "<!service.name!>", <!serviceArgs!>);
//       ReplyAnswer<<!serviceInfo.replyType!>> answer(<!service.taskName!>_reply_port.read());
//       cout << "Intermediate reply: " << answer;

//       cout << "Wait final reply ?";
//       string res;
//       cin >> res;
//       if(res == "y") {
// 	  ReplyAnswer<<!serviceInfo.replyType!>> answer(<!service.taskName!>_reply_port.read());
// 	  cout << "Final reply: " << answer; 
//       }
<?
  ?>
}

<?
for port in outports:
  typeName = MapTypeToC(port.idlType)
  ?>
void read<!port.name!>()
{
  <!typeName!> res;
  <!port.name!>_inport.getLatestData(res);
  cout << endl << "<!port.name!> :" << endl;
  YarpCodec<<!typeName!>>::print(res);
  cout << endl;
}
<?
?>

void executeAction(int action)
{
  switch(action) {
    case 0:
	exit(0);

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
  }
}

};

int main()
{
    yarp::os::Network yarp;

//     Contact c = yarp.queryName("<!comp.name()!>");
//     if(!c.isValid()) {
//       cout << "Impossible to contact module." << endl;
//       return 1;
//     }

    <!comp.name()!>Test test;
    while(true) {
	cout << endl;
	test.printUsage();
	int action;  
	std::cin >> action;
	test.executeAction(action);
    }
}



