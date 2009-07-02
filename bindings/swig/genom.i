%module genom

%{
 /* Includes the header in the wrapper code */
 #include "../../utils/idlvalues.h"
 #include "../../utils/idltype.h"
 #include "../../utils/ast.h"
 #include "../../utils/typevisitor.h"
 #include "../../bindings/swig/tclinterpreter.h"

using namespace G3nom;
using namespace Idl;
%}

namespace boost {
 template<class T> class shared_ptr
 {
 public:
 T * operator-> () const;
 };
}

%include "std_string.i"
%include "tcl/std_map.i"
%include "tcl/std_vector.i" 

%include "../../utils/idlvalues.h"
%include "../../utils/idltype.h"
%include "../../utils/ast.h"
%include "../../utils/typevisitor.h"

/* Shared ptrs */
%template(IdlTypePtr) boost::shared_ptr<G3nom::Idl::IdlType>;
%template(ServicePtr) boost::shared_ptr<G3nom::Service>;
%template(CodelPtr) boost::shared_ptr<G3nom::Codel>;
%template(PortPtr) boost::shared_ptr<G3nom::Port>;
%template(TaskPtr) boost::shared_ptr<G3nom::Task>;
/* %template(EventPtr) boost::shared_ptr<G3nom::Event>;  */

/* Maps */
%template(ServiceMap) std::map<std::string, G3nom::Service::Ptr >;
%template(PortMap) std::map<std::string, G3nom::Port::Ptr >;
%template(CodelMap) std::map<std::string, G3nom::Codel::Ptr >;
%template(TaskMap) std::map<std::string, G3nom::Task::Ptr >;
%template(IdlTypeMap) std::map<std::string, G3nom::Idl::IdlType::Ptr >;
%template(ConstValueMap) std::map<std::string, G3nom::Idl::ConstValue>;
/* %template(EventRevMap) std::map<G3nom::Event::Ptr, std::string>; */

/* Vectors */
%template(IntVect) std::vector<int>;
%template(StringVect) std::vector<std::string> ;
%template(IdlTypeVect) std::vector< IdlTypePtr >;
%template(ServiceInputVect) std::vector<G3nom::ServiceInput>;


%{
void tclWriteStdout(const char *text)
{
  TclInterpreter::getInstance()->writeStdout(text);
}

Component* getCurrentComponent()
{
  TclInterpreter *i = TclInterpreter::getInstance();
  return i->component();
}
%}

G3nom::Component* getCurrentComponent();
void tclWriteStdout(const char *text);

