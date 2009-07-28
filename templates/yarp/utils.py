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

def input_list(service):
  serviceInfo = services_info_dict[service.name]
  res = []
  if len(service.inputs()) > 1:
    for i in service.inputs():
      res.extend(flat_struct(input_type(i), serviceInfo.inputName + "." + i.identifier, '.'))
    return res
  elif serviceInfo.inputFlag:
    return flat_struct(serviceInfo.inputType, serviceInfo.inputName, '.')
  return []

class ServiceInfo:
  """ This class is used to store all types of information about a service to
  avoid computing them again and again. Only the __init__ function is used to fill 
  the class."""
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
	  t = input_type(i)
	  s.addMember(t, i.identifier)
	self.inputName = service.name + "_input"
	self.inputType = NamedType(service.name + "_input_struct", s)
	# add a type and the corresponding element in the ids
	typesVect.append(s)
	IDSType.addMember(self.inputType, service.name + "_input")

      else:
	self.inputName = service.inputs()[0].identifier
	self.inputType = input_type(service.inputs()[0])
      self.inputTypeCpp = MapTypeToCpp(self.inputType)
      self.requestType = self.inputTypeCpp

    if not service.output.identifier:
      self.outputFlag = False
      self.replyType = "VoidIO"
    else:
      self.outputFlag = True
      self.outputName = service.output.identifier
      self.outputType = input_type(service.output)
      self.outputTypeCpp = MapTypeToCpp(self.outputType)
      self.replyType = self.outputTypeCpp

# create serviceInfo objects
services_info_dict = dict()
for s in servicesMap:
    service = s.data()
    services_info_dict[service.name] = ServiceInfo(service)    

def real_codel_call(codel, ids_prefix="", service=None, allArgs = False):
  """ Creates the code to call user codel. """
  proto = ""
  if service is not None:
    inputPrefix = ""
    if len(service.inputs()) > 1:
	serviceInfo = services_info_dict[service.name]
	inputPrefix = serviceInfo.inputName + "."
    for i in service.inputs():
	if inputPrefix and not allArgs:
	    proto += address_of(input_type(i), "in_" + inputPrefix + i.identifier) + ", "
	else:
	    proto += address_of(input_type(i), "in_" + i.identifier) + ", "
    if service.output.identifier:
	proto += "& out_" + service.output.identifier + ", "

  for type in codel.inTypes:
    proto += address_of(comp.typeFromIdsName(type), ids_prefix + type + ", ");
  for type in codel.outTypes:
    proto +=  address_of(comp.typeFromIdsName(type), ids_prefix + type + ", ");
  for port in codel.outPorts:
    proto +=  ids_prefix + port + "_outport.data, "; 
  for port in codel.inPorts:
    proto += ids_prefix + port + "_inport.getDataPtr(), "; 
  proto = codel.name + "(" + proto[:-2] + ")"
  return proto

def start_state_for_service(service):
  if service.hasCodel("start"):
    return upper(service.name) + "_START"
  else:
    return upper(service.name) + "_MAIN"

def allocate_memory(t, dest, scopedName):
    """ Allocate the memory as requested by the size codel."""
    if t.kind() == IdlType.Sequence:
      s = t.asSequenceType()
      seqType = MapTypeToC(s.seqType(), True)
      print dest + ".data = new " + seqType + "[" + length_var(scopedName) + "];"
      print dest + ".length = " + length_var(scopedName) + ";"
      print ""

      if is_dynamic(s.seqType()):
	counter = counter_name(dest)
	print "int " + counter + " = 0;"
	print "for(; " + counter + "<" + dest + ".length; ++" + counter + ") {"
	allocate_memory(s.seqType(), dest + ".data[" + counter + "]", scopedName + ".data")
	print "}"
      else:
	print "memset(&" + dest + " , 0, " + length_var(scopedName) + " * sizeof(" + seqType + "));"

    elif t.kind() == IdlType.Struct:
      s = t.asStructType()
      for m in s.members():
	allocate_memory(m.data, dest + "." + m.key, scopedName + "." + m.key)
    elif t.kind() == IdlType.Named or t.kind() == IdlType.Typedef:
      allocate_memory(t.unalias(), dest, scopedName)

def codel_needs_lock(codel, service):
  """ Returns whether it is necessary to lock the IDS when calling the codel. 0 means no locking,
  1 means lock for read and 2 read for write. """
  if codel.outTypes:
    return 2
  elif service.output.identifier and service.output.kind == ServiceInput.IDSMember:
    return 2
  elif codel.inTypes:
    return 1
  else:
    for i in service.inputs():
      if i.kind == ServiceInput.IDSMember:
	return 1
    return 0

def call_size_codel(port):
  """ String to call the size codel. """
  sizeCodelArgs = ""
  for x in dynamic_members(port.idlType, port.name + "_outport", True):
    print "size_t " + length_var(x[1]) + " = 0;"
    sizeCodelArgs += "&" + length_var(x[1]) + ", "

  for s in port.sizeCodel.inTypes:
    sizeCodelArgs += "&m_data->" + s + ", "
  for s in port.sizeCodel.outTypes:
    sizeCodelArgs += "&m_data->" + s + ", "
  for s in port.sizeCodel.inPorts:
    sizeCodelArgs += s + "_inport, "
  for s in port.sizeCodel.outPorts:
    sizeCodelArgs += s + "_outport, "

  print "int res = " + port.sizeCodel.name + "(" + sizeCodelArgs[:-2] + ");"

def codel_lock(codel, service):
    """ prepare to call user codel. Lock the ids, initialize ports, etc."""
    for p in codel.outPorts:
      port = comp.port(p)
      if not is_dynamic(port.idlType):
	continue

      print "if(!m_data->" + p + "_outport.isInitialized()) {"

      call_size_codel(port)

      print "  if(res >= 0) {"
      print "    m_data->" + p + "_outport.initialize();"
      allocate_memory(port.idlType, "(*m_data->" + p + "_outport.data)", p + "_outport")
      print "  }"
      print "}"
 
    for p in codel.inPorts:
      print "  m_data->" + p + "_inport.wait();"
    res = codel_needs_lock(codel, service)
    if res == 2:
      print "  m_data->idsMutex.acquire_write();"
    elif res == 1:
      print "  m_data->idsMutex.acquire_read();"

def codel_release(codel, service):
    """ Release locks, etc after codel has been called"""
    for p in codel.outPorts:
      print "  m_data->" + p + "_outport.exportData();"  
    for p in codel.inPorts:
      print "  m_data->" + p + "_inport.post();"
    res = codel_needs_lock(codel, service)
    if res:
      print "  m_data->idsMutex.release();"

def codel_to_event_name(name):
  #if name == "control":
    #return "onCalled"
  #elif name == "start":
    #return "onStart"
  #elif name == "end":
    #return "onEnd"
  #elif name == "inter":
    #return "onInter"
  return name

def full_port_name(p):
  """ Returns the name of the port in internal data structure."""
  port = comp.port(pev.portName())
  if port.type == Port.Incoming:
    return p + "_inport"
  else:
    return p + "_outport"

