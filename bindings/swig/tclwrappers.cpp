#include "genom_wrap.cxx"

#define FOREACH_PTR_MAP(ptrBaseType, swigType) int foreach_##ptrBaseType##_map(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj* CONST objv[]) \
{\
  std::map< std::string,boost::shared_ptr<ptrBaseType> > *arg1 = 0 ;\
  void *argp1 ; \
  int res1; \
  ptrBaseType::Map::const_iterator it; \
   res1 = SWIG_ConvertPtr(objv[2], &argp1, SWIGTYPE_p_std__mapT_std__string_boost__shared_ptrT_G3nom__##swigType##_t_t,  0 ); \
  if (!SWIG_IsOK(res1)) { \
        SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "foreach_" #ptrBaseType "_map" "', argument " "1"" of type '" "std::map<std::string, boost::shared_ptr<" #ptrBaseType "> > const &""'"); \
  } \
  if (!argp1) {\
    SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "foreach_" #ptrBaseType "_map" "', argument " "1"" of type '" "std::map<std::string, boost::shared_ptr<" #ptrBaseType "> > const &""'"); \
  } \
  \
  arg1 = reinterpret_cast< std::map< std::string,boost::shared_ptr<ptrBaseType> > * >(argp1);\
  it = arg1->begin(); \
  for(; it != arg1->end(); ++it) {\
	Tcl_Obj *keyValue = SwigString_FromString(it->first);\
	Tcl_Obj *dataValue = SWIG_NewInstanceObj((new ptrBaseType::Ptr(static_cast< const ptrBaseType::Ptr& >(it->second))), SWIGTYPE_p_boost__shared_ptrT_G3nom__##swigType##_t, SWIG_POINTER_OWN | 0 );\
	Tcl_Obj *key, *data;\
	Tcl_ListObjIndex(interp, objv[1], 0, &key);\
	Tcl_ListObjIndex(interp, objv[1], 1, &data);\
	\
	Tcl_ObjSetVar2(interp, key, 0, keyValue, 0);\
	Tcl_ObjSetVar2(interp, data, 0, dataValue, 0);\
	Tcl_EvalObj(interp, objv[3]);\
  }\
  return TCL_OK;\
\
fail:\
	return TCL_ERROR;\
}\


int foreach_ConstValue_map(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj* CONST objv[]) 
{
  ConstValue::Map *arg1 = 0 ;
  void *argp1 ;
  int res1;
  ConstValue::Map::const_iterator it;

  res1 = SWIG_ConvertPtr(objv[2], &argp1, SWIGTYPE_p_std__mapT_std__string_G3nom__Idl__ConstValue_t,  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "foreach_ConstValue_map" "', argument " "1"" of type '" "ConstValue::Map const &""'"); 
  }
  if (!argp1) {
    SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "foreach_ConstValue_map" "', argument " "1"" of type '" "ConstValue::Map const &""'"); 
  }
  arg1 = reinterpret_cast< ConstValue::Map* >(argp1);
  
  it = arg1->begin();
  for(; it != arg1->end(); ++it) {
	Tcl_Obj *keyValue = SwigString_FromString(it->first);
	Tcl_Obj *dataValue = SWIG_NewInstanceObj( SWIG_as_voidptr(&it->second), SWIGTYPE_p_G3nom__Idl__ConstValue,0);
	Tcl_Obj *key, *data;
	Tcl_ListObjIndex(interp, objv[1], 0, &key);
	Tcl_ListObjIndex(interp, objv[1], 1, &data);

	Tcl_ObjSetVar2(interp, key, 0, keyValue, 0);
	Tcl_ObjSetVar2(interp, data, 0, dataValue, 0);
	Tcl_EvalObj(interp, objv[3]);
  }
  
  return TCL_OK;
  
fail:
	return TCL_ERROR;
}

int foreach_int_vect(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj* CONST objv[]) 
{
  std::vector<int> *arg1 = 0 ;
  void *argp1 ;
  int res1;
  std::vector<int>::const_iterator it;

  res1 = SWIG_ConvertPtr(objv[2], &argp1, SWIGTYPE_p_std__vectorT_int_t,  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "foreach_string_vect" "', argument " "1"" of type '" "std::vector<std::string> const &""'"); 
  }
  if (!argp1) {
    SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "foreach_string_vect" "', argument " "1"" of type '" "std::vector<std::string> const &""'"); 
  }
  arg1 = reinterpret_cast< std::vector<int>* >(argp1);
  
  it = arg1->begin();
  for(; it != arg1->end(); ++it) {
	Tcl_Obj *value = SWIG_From_int(*it);
	Tcl_ObjSetVar2(interp, objv[1], 0, value, 0);
	Tcl_EvalObj(interp, objv[3]);
  }
  
  return TCL_OK;
  
fail:
	return TCL_ERROR;
}

int foreach_string_vect(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj* CONST objv[]) 
{
  std::vector<std::string> *arg1 = 0 ;
  void *argp1 ;
  int res1;
  std::vector<std::string>::const_iterator it;

  res1 = SWIG_ConvertPtr(objv[2], &argp1, SWIGTYPE_p_std__vectorT_std__string_t,  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "foreach_string_vect" "', argument " "1"" of type '" "std::vector<std::string> const &""'"); 
  }
  if (!argp1) {
    SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "foreach_string_vect" "', argument " "1"" of type '" "std::vector<std::string> const &""'"); 
  }
  arg1 = reinterpret_cast< std::vector<std::string>* >(argp1);
  
  it = arg1->begin();
  for(; it != arg1->end(); ++it) {
	Tcl_Obj *value = SwigString_FromString(*it);
	Tcl_ObjSetVar2(interp, objv[1], 0, value, 0);
	Tcl_EvalObj(interp, objv[3]);
  }
  
  return TCL_OK;
  
fail:
	return TCL_ERROR;
}

int foreach_ServiceInput_vect(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj* CONST objv[]) 
{
  std::vector<ServiceInput> *arg1 = 0 ;
  void *argp1 ;
  int res1;
  std::vector<ServiceInput>::iterator it;

  res1 = SWIG_ConvertPtr(objv[2], &argp1, SWIGTYPE_p_std__vectorT_G3nom__ServiceInput_t,  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "foreach_ServiceInput_vect" "', argument " "1"" of type '" "std::vector<ServiceInput> const &""'"); 
  }
  if (!argp1) {
    SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "foreach_ServiceInput_vect" "', argument " "1"" of type '" "std::vector<ServiceInput> const &""'"); 
  }

  arg1 = reinterpret_cast<std::vector<ServiceInput>* >(argp1);
  
  for(it = arg1->begin(); it != arg1->end(); ++it) {
	G3nom::ServiceInput *result = &(*it);
	Tcl_Obj *value = SWIG_NewInstanceObj( SWIG_as_voidptr(result), SWIGTYPE_p_G3nom__ServiceInput,0);
	Tcl_ObjSetVar2(interp, objv[1], 0, value, 0);
	Tcl_EvalObj(interp, objv[3]);
  }
  
  return TCL_OK;
  
fail:
	return TCL_ERROR;
}

int foreach_IdlType_vect(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj* CONST objv[]) 
{
  std::vector<IdlType::Ptr> *arg1 = 0 ;
  void *argp1 ;
  int res1;
  std::vector<IdlType::Ptr>::iterator it;

  res1 = SWIG_ConvertPtr(objv[2], &argp1, SWIGTYPE_p_std__vectorT_boost__shared_ptrT_G3nom__Idl__IdlType_t_t,  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "foreach_IdlType_vect" "', argument " "1"" of type '" "std::vector<IdlType::Ptr> const &""'"); 
  }
  if (!argp1) {
    SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "foreach_IdlType_vect" "', argument " "1"" of type '" "std::vector<IdlType::Ptr> const &""'"); 
  }

  arg1 = reinterpret_cast<std::vector<IdlType::Ptr>* >(argp1);
  
  for(it = arg1->begin(); it != arg1->end(); ++it) {
	Tcl_Obj *value = SWIG_NewInstanceObj((new IdlType::Ptr(static_cast< const IdlType::Ptr& >(*it))), SWIGTYPE_p_boost__shared_ptrT_G3nom__Idl__IdlType_t, SWIG_POINTER_OWN | 0 );
	Tcl_ObjSetVar2(interp, objv[1], 0, value, 0);
	Tcl_EvalObj(interp, objv[3]);
  }
  
  return TCL_OK;
  
fail:
	return TCL_ERROR;
}

FOREACH_PTR_MAP(Service, Service)
FOREACH_PTR_MAP(Task, Task)
FOREACH_PTR_MAP(Codel, Codel)
FOREACH_PTR_MAP(Port, Port)
FOREACH_PTR_MAP(IdlType, Idl__IdlType)
