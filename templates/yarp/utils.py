# -*- coding: utf-8 -*-
from string import upper

IDSType = comp.IDSType.unalias()
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

def addressOf(t, s):
 if t.kind() == IdlKind.String:
   return s
 else:
    return "&" + s

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
  serviceInfo = services_info_dict[service.name]
  res = []
  if len(service.inputs()) > 1:
    for i in service.inputs():
      res.extend(flatStruct(inputType(i), serviceInfo.inputName + "." + i.identifier, '.'))
    return res
  elif serviceInfo.inputFlag:
    return flatStruct(serviceInfo.inputType, serviceInfo.inputName, '.')
  return []

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
      self.requestType = "VoidIO"
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
      self.inputTypeCpp = MapTypeToCpp(self.inputType)
      self.requestType = self.inputTypeCpp

    if not service.output.identifier:
      self.outputFlag = False
      self.replyType = "VoidIO"
    else:
      self.outputFlag = True
      self.outputName = service.output.identifier
      self.outputType = inputType(service.output)
      self.outputTypeCpp = MapTypeToCpp(self.outputType)
      self.replyType = self.outputTypeCpp

# create serviceInfo objects
services_info_dict = dict()
for s in servicesMap:
    service = s.data()
    services_info_dict[service.name] = ServiceInfo(service)    

def real_codel_call(codel, ids_prefix="", service=None, allArgs = False):
  proto = ""
  if service is not None:
    inputPrefix = ""
    if len(service.inputs()) > 1:
	serviceInfo = services_info_dict[service.name]
	inputPrefix = serviceInfo.inputName + "."
    for i in service.inputs():
	if inputPrefix and not allArgs:
	    proto += addressOf(inputType(i), "in_" + inputPrefix + i.identifier) + ", "
	else:
	    proto += addressOf(inputType(i), "in_" + i.identifier) + ", "
    if service.output.identifier:
	proto += "& out_" + service.output.identifier + ", "

  for type in codel.inTypes:
    proto += "& " + ids_prefix + type + ", ";
  for type in codel.outTypes:
    proto += "& " + ids_prefix + type + ", ";
  for port in codel.outPorts:
    proto +=  ids_prefix + port + "_outport.data, "; 
  for port in codel.inPorts:
    proto += ids_prefix + port + "_inport.getDataPtr(), "; 
  proto = codel.name + "(" + proto[:-2] + ")"
  return proto


def real_codel_signature(codel, service=None):
  proto = ""
  if service is not None:
    for s in service.inputs():
	idstype = inputType(s);
	proto += "const " + pointerTo(idstype) + " in_" + s.identifier + ", ";
    if service.output.identifier:
	idstype = inputType(service.output);
	proto += pointerTo(idstype) + " out_" + service.output.identifier + ", "; 

  for type in codel.inTypes:
    idstype = comp.typeFromIdsName(type);
    proto += "const " + pointerTo(idstype) + " in_" + type + ", ";
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

# error related functions
def createErrorList():
  l = []
  for s in servicesMap:
    service = s.data()
    for e in service.errorMessages():
	l.append(e)
  for t in comp.tasksMap():
    for e in t.data().errorMessages():
	l.append(e)
  return set(l)
errorList = createErrorList();

def codelNeedsLock(codel, service):
  if codel.outTypes:
    return 2
  elif service.output.identifier and service.output.kind == ServiceInputKind.IDSMember:
    return 2
  elif codel.inTypes:
    return 1
  else:
    for i in service.inputs():
      if i.kind == ServiceInputKind.IDSMember:
	return 1
    return 0

def codelLock(codel, service):
    for p in codel.inPorts:
      print "  m_data->" + p + "_inport.wait();"
    res = codelNeedsLock(codel, service)
    if res == 2:
      print "  m_data->idsMutex.acquire_write();"
    elif res == 1:
      print "  m_data->idsMutex.acquire_read();"

def codelRelease(codel, service):
    for p in codel.outPorts:
      print "  m_data->" + p + "_outport.exportData();"  
    for p in codel.inPorts:
      print "  m_data->" + p + "_inport.post();"
    res = codelNeedsLock(codel, service)
    if res:
      print "  m_data->idsMutex.release();"

