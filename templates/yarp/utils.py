# -*- coding: utf-8 -*-
from string import upper

IDSType = comp.IDSType
servicesMap = comp.servicesMap()
tasksMap = comp.tasksMap()
typesVect = comp.typesVect()
portsMap = comp.portsMap()
capCompName = comp.name().capitalize()
upperCompName = upper(comp.name())

typesVect = []
for t in comp.typesVect():
  typesVect.append(t)

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

outports = []
inports = []
for p in portsMap:
    port = p.data()
    if port.type == PortType.Outgoing:
	outports.append(port)
    else:
	inports.append(port)

def pointerTo(t):
  s = MapTypeToC(t,True)
  if t.kind() == IdlKind.String:
    return s
  else:
    return s+"*"

class ServiceInfo:
  def __init__(self, service):
    # inputs
    if not service.inputs():
      self.inputFlag = False
      self.signatureProto = ""
      self.userSignatureProto = ""
    else:
      self.inputFlag = True

      if len(service.inputs()) > 1: # need to create the type
	s = StructType()
	s.setIdentifier(service.name + "_input_struct")
	for i in service.inputs():
	  t = inputType(i)
	  s.addMember(t, i.identifier)
	self.inputName = service.name + "_input"
	self.inputType = NamedType(service.name + "_input_struct", s)
	# add a type and the corresponding element in the ids
	typesVect.append(s)
	IDSType.addMember(self.inputType, service.name + "_input")

      else:
	self.inputName = service.inputs()[0].identifier
	self.inputType = inputType(service.inputs()[0])
      self.inputTypeCpp = MapTypeToC(self.inputType)

    if not service.output.identifier:
      self.outputFlag = False
    else:
      self.outputFlag = True
      self.outputName = service.output.identifier
      self.outputType = inputType(service.output)
      self.outputTypeCpp = MapTypeToC(self.outputType)

# create serviceInfo objects
services_info_dict = dict()
for s in servicesMap:
    service = s.data()
    services_info_dict[service.name] = ServiceInfo(service)    

def real_codel_call(codel, data_prefix="", service=None):
  proto = ""
  if service is not None:
    for i in service.inputs():
	proto += " &in_" + i.identifier + ", ";
    if service.output.identifier:
	proto += " &out_" + service.output.identifier + ", "; 

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


def real_codel_signature(codel, service=None):
  proto = ""
  if service is not None:
    for s in service.inputs():
	idstype = inputType(s);
	proto += pointerTo(idstype) + " in_" + s.identifier + ", ";
    if service.output.identifier:
	idstype = inputType(service.output);
	proto += pointerTo(idstype) + " out_" + service.output.identifier + ", "; 

  for type in codel.inTypes:
    idstype = comp.typeFromIdsName(type);
    proto += pointerTo(idstype) + " in_" + type + ", ";
  for type in codel.outTypes:
    idstype = comp.typeFromIdsName(type);
    proto += pointerTo(idstype) + " out_" + type + ", ";
  for port in codel.outPorts:
    p = comp.port(port)
    if p is not None:
	proto += pointerTo(p.idlType) + " outport_" + port + ", "; 
    else:
	proto += port + ", "
  for port in codel.inPorts:
    p = comp.port(port)
    if p is not None:
	proto += pointerTo(p.idlType) + " inport_" + port + ", "; 
    else:
	proto += port + ", "
  proto = codel.name + "(" + proto[:-2] + ")"
  return proto


def startStateForService(service):
  if service.hasCodel("start"):
    return upper(service.name) + "_START"
  else:
    return upper(service.name) + "_MAIN"
