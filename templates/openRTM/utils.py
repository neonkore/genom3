# -*- coding: utf-8 -*-
from string import upper

IDSType = comp.IDSType
servicesMap = comp.servicesMap()
capCompName = comp.name().capitalize()
upperCompName = upper(comp.name())

def inputType(i):
  if i.kind == ServiceInputKind.IDSMember:
    return comp.typeFromIdsName(i.identifier)
  else:
    return i.type

# returns a flat list of the structure of a type
def flatStruct(t, name, separator = "_"):
    if t.kind() == IdlKind.Named:
	n = t.asNamedType()
	return flatStruct(n.type(), name, separator)
    elif t.kind() == IdlKind.Struct:
	s = t.asStructType()
	l = [] 
	for m in s.members():
	    l.extend(flatStruct(m.data(), name + separator + m.key(), separator))
	return l
    else:
	return [(t, name)]  

def inputList(service):
  res = []
  for i in service.inputs():
    res.extend(flatStruct(comp.typeFromIdsName(i), i, '.'))
  return res

def service_idl_signature(service):
    # find the service output type
    if len(service.output) > 0:
	outputType = BaseType.longType
    else:
	outputType = BaseType.voidType

    # then create the args list
    args = ""
    for i in service.inputs():
	t = inputType(i)
	args += "in " + MapTypeToIdl(t) + " " + i.identifier + ", "
    return MapTypeToIdl(outputType) + " " + service.name + "(" + args[:-2] + ");"

def service_cpp_args(service, className=""):
    # create the args list
    args = ""
    for i in service.inputs():
	t = inputType(i)
	args += MapTypeToCpp(t) + " in_" + i.identifier + ", "
    if className != "":
	return className + "::" + service.name + "(" + args[:-2] + ")"
    else:
	return service.name + "(" + args[:-2] + ")"

def service_cpp_signature(service, className=""):
    # find the service output type
    if len(service.output) > 0:
	if service.type != ServiceType.Control:
	  outputType = BaseType.longType
	else:
	  outputType = comp.typeFromIdsName(service.output)
    else:
	outputType = BaseType.voidType
    return MapTypeToCpp(outputType, True, True) + " " + service_cpp_args(service, className)

def pointerTo(t):
  s = MapTypeToC(t,True)
  if t.kind() == IdlKind.String:
    return s
  else:
    return s+"*"

def real_codel_signature(codel, service=None):
  proto = ""
  if service != None:
    for s in service.inputs():
	idstype = inputType(s);
	proto += pointerTo(idstype) + " in_" + s.identifier + ", ";
    if len(service.output) > 0:
	idstype = comp.typeFromIdsName(service.output);
	proto += pointerTo(idstype) + " out_" + service.output + ", "; 

  for type in codel.inTypes:
    idstype = comp.typeFromIdsName(type);
    proto += pointerTo(idstype) + " in_" + type + ", ";
  for type in codel.outTypes:
    idstype = comp.typeFromIdsName(type);
    proto += pointerTo(idstype) + " out_" + type + ", ";
  for port in codel.outPorts:
    p = comp.port(port)
    if p != None:
	proto += pointerTo(p.idlType) + " outport_" + port + ", "; 
    else:
	proto += port + ", "
  for port in codel.inPorts:
    p = comp.port(port)
    if p != None:
	proto += pointerTo(p.idlType) + " inport_" + port + ", "; 
    else:
	proto += port + ", "
  proto = codel.name + "(" + proto[:-2] + ")"
  return proto

# try to find an init service
def findInitService():
  i=-1
  for s in servicesMap:
    i += 1
    if s.data().type == ServiceType.Init:
      return s.data(), i
  return 0,-1

initService,initServiceNb = findInitService()

# create a list of outports and inports
outports = []
inports = []
for p in comp.portsMap():
    if p.data().type == PortType.Outgoing:
	outports.append(p.data())
    else:
	inports.append(p.data())

def codel_call(codel, service=None):
  proto = ""
  if service != None:
    for s in service.inputs():
	proto += " in_" + s.identifier + ", ";
    if len(service.output) > 0:
	proto += " out_" + service.output + ", "; 

  for type in codel.inTypes:
    proto += "& in_" + type + ", ";
  for type in codel.outTypes:
    proto += "& out_" + type + ", ";
  for port in codel.outPorts:
    proto += port + ", "; 
  for port in codel.inPorts:
    proto += port + ", "; 
  proto = codel.name + "(" + proto[:-2] + ")"
  return proto

def real_codel_call(codel, data_prefix="", service=None):
  proto = ""
  if service != None:
    for i in service.inputs():
	proto += " &in_" + i.identifier + ", ";
    if len(service.output) > 0:
	proto += " &out_" + service.output + ", "; 

  for type in codel.inTypes:
    proto += "& " + data_prefix + type + ", ";
  for type in codel.outTypes:
    proto += "& " + data_prefix + type + ", ";
  for port in codel.outPorts:
    proto += "&" + data_prefix + port + "_data, "; 
  for port in codel.inPorts:
    proto += "&" + data_prefix + port + ", "; 
  proto = codel.name + "(" + proto[:-2] + ")"
  return proto

def service_call(service):
    # create the args list
    args = ""
    for i in service.inputs():
	args += "in_" + i.identifier + ", "
    return service.name + "(" + args[:-2] + ")"

def serviceOutportsSet(service):
  portList = []
  for c in service.codels():
    for port in c.data().outPorts:
      portList.append(port)
  return set(portList)

def serviceInportsSet(service):
  portList = []
  for c in service.codels():
    for port in c.data().inPorts:
      portList.append(port)
  return set(portList)

def startStateForService(service):
  if service.hasCodel("start"):
    return upper(service.name) + "_START"
  else:
    return upper(service.name) + "_MAIN"

def outputPortsMap():
  m = {}
  for s in servicesMap:
    service = s.data()
    if service.type != ServiceType.Exec or len(service.output) == 0:
      continue
    typeName = MapTypeToIdl(comp.typeFromIdsName(service.output))
    m[service.name] = typeName
  return m

output_ports_map = outputPortsMap()

