# -*- coding: utf-8 -*-
from string import upper

IDSType = comp.IDSType
servicesMap = comp.servicesMap()
capCompName = comp.name().capitalize()
upperCompName = upper(comp.name())

def service_idl_signature(service):
    # find the service output type
    if len(service.output) > 0:
	outputType = comp.typeFromIdsName(service.output)
    else:
	outputType = BaseType.voidType

    # then create the args list
    args = ""
    for i in service.inputs():
	t = comp.typeFromIdsName(i)
	args += "in " + MapTypeToIdl(t) + " " + i + ", "
    return MapTypeToIdl(outputType) + " " + service.name + "(" + args[:-2] + ");"

def service_cpp_signature(service):
    # find the service output type
    if len(service.output) > 0:
	outputType = comp.typeFromIdsName(service.output)
    else:
	outputType = BaseType.voidType

    # then create the args list
    args = ""
    for i in service.inputs():
	t = comp.typeFromIdsName(i)
	args += MapTypeToCpp(t) + " in_" + i + ", "
    return MapTypeToCpp(outputType) + " " + service.name + "(" + args[:-2] + ")"

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
	proto += " in_" + s + ", ";
    if len(service.output) > 0:
	proto += " out_" + service.output + ", "; 

  for type in codel.inTypes:
    proto += "& in_" + type + ", ";
  for type in codel.outTypes:
    proto += "& out_" + type + ", ";
  for port in codel.outPorts:
    proto += "outport_" + port + ", "; 
  for port in codel.inPorts:
    proto += "inport_" + port + ", "; 
  proto = codel.name + "(" + proto[:-2] + ")"
  return proto

def service_call(service):
    # create the args list
    args = ""
    for i in service.inputs():
	args += "in_" + i + ", "
    return service.name + "(" + args[:-2] + ")"

