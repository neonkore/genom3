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
    proto += addressOf(comp.typeFromIdsName(type), ids_prefix + type + ", ");
  for type in codel.outTypes:
    proto +=  addressOf(comp.typeFromIdsName(type), ids_prefix + type + ", ");
  for port in codel.outPorts:
    proto +=  ids_prefix + port + "_outport.data, "; 
  for port in codel.inPorts:
    proto += ids_prefix + port + "_inport.getDataPtr(), "; 
  proto = codel.name + "(" + proto[:-2] + ")"
  return proto

def startStateForService(service):
  if service.hasCodel("start"):
    return upper(service.name) + "_START"
  else:
    return upper(service.name) + "_MAIN"

def allocMemory(t, dest, scopedName):
    if t.kind() == IdlKind.Sequence:
      s = t.asSequenceType()
      seqType = MapTypeToC(s.seqType(), True)
      print dest + ".data = new " + seqType + "[" + lengthVar(scopedName) + "];"
      print dest + ".length = " + lengthVar(scopedName) + ";"
      print ""

      if isDynamic(s.seqType()):
	counter = counterName(dest)
	print "int " + counter + " = 0;"
	print "for(; " + counter + "<" + dest + ".length; ++" + counter + ") {"
	allocMemory(s.seqType(), dest + ".data[" + counter + "]", scopedName + ".data")
	print "}"
      else:
	print "memset(" + dest + " , 0, " + lengthVar(scopedName) + " * sizeof(" + seqType + "));"

    elif t.kind() == IdlKind.Struct:
      s = t.asStructType()
      for m in s.members():
	allocMemory(m.data(), dest + "." + m.key(), scopedName + "." + m.key())
    elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
      allocMemory(t.unalias(), dest, scopedName)

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

def callSizeCodel(port):
  sizeCodelArgs = ""
  for x in dynamicMembers(port.idlType, port.name + "_outport", True):
    print "int " + lengthVar(x[1]) + " = 0;"
    sizeCodelArgs += "&" + lengthVar(x[1]) + ", "

  for s in port.sizeCodel.inTypes:
    sizeCodelArgs += "&m_data->" + s + ", "
  for s in port.sizeCodel.outTypes:
    sizeCodelArgs += "&m_data->" + s + ", "
  for s in port.sizeCodel.inPorts:
    sizeCodelArgs += s + "_inport, "
  for s in port.sizeCodel.outPorts:
    sizeCodelArgs += s + "_outport, "

  print "int res = " + port.sizeCodel.name + "(" + sizeCodelArgs[:-2] + ");"

def codelLock(codel, service):
    for p in codel.outPorts:
      port = comp.port(p)
      if not isDynamic(port.idlType):
	continue

      print "if(!m_data->" + p + "_outport.isInitialized()) {"

      callSizeCodel(port)

      print "  if(res >= 0) {"
      print "    m_data->" + p + "_outport.initialize();"
      allocMemory(port.idlType, "(*m_data->" + p + "_outport.data)", p + "_outport")
      print "  }"
      print "}"
 
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


