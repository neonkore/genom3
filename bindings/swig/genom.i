%module genom

%{
 /* Includes the header in the wrapper code */
 #include "../../utils/ast.h"
 #include "../../utils/idltype.h"
%}

namespace boost {
 template<class T> class shared_ptr
 {
 public:
 T * operator-> () const;
 };
}

/* %include "boost_shared_ptr.i"  */
%include "std_string.i"
%include "tcl/std_map.i"
%include "tcl/std_vector.i" 

%include "../../utils/ast.h"
%include "../../utils/idltype.h"

/* Maps */
%template(ServiceMap) std::map<std::string, G3nom::Service>;

%template(IdlTypePtr) boost::shared_ptr<G3nom::Idl::IldType>;
%template(ServicePtr) boost::shared_ptr<Service>;
%template(PortPtr) boost::shared_ptr<Port>;