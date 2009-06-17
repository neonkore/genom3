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
    res.extend(flatStruct(inputType(i), i.identifier, '.'))
  return res

def service_idl_signature(service):
    # find the service output type
    if service.output.identifier:
	if service.type != ServiceType.Control:
	  outputType = BaseType.longType
	else:
	  outputType = inputType(service.output)
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
	args += MapTypeToCorbaCpp(t) + " in_" + i.identifier + ", "
    if className != "":
	return className + "::" + service.name + "(" + args[:-2] + ")"
    else:
	return service.name + "(" + args[:-2] + ")"

def service_cpp_signature(service, className=""):
    # find the service output type
    if service.output.identifier:
	if service.type != ServiceType.Control:
	  outputType = BaseType.longType
	else:
	  outputType = inputType(service.output)
    else:
	outputType = BaseType.voidType
    return MapTypeToCorbaCpp(outputType, True) + " " + service_cpp_args(service, className)

def pointerTo(t):
  s = MapTypeToC(t,True)
  if t.kind() == IdlKind.String:
    return s
  else:
    return s+"*"

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
	if isDynamic(p.idlType):
	  proto += MapTypeToCpp(p.idlType) + "Cpp* " + port + "_outport, " 
	else:
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
  if service is not None:
    for s in service.inputs():
	proto += " in_" + s.identifier + ", ";
    if service.output.identifier:
	proto += " out_" + service.output.identifier + ", "; 

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
    p = comp.port(port)
    if not p is None and isDynamic(p.idlType):
      proto += "&" + port + "_outport, "
    else:
      proto += "&" + data_prefix + port + "_data, " 
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
    if service.type != ServiceType.Exec or not service.output.identifier:
      continue
    typeName = MapTypeToIdl(inputType(service.output))
    m[service.name] = typeName
  return m

output_ports_map = outputPortsMap()

def codelNeedsLock(codel, service=None):
  if codel.outTypes:
    return 2
  elif not service is None and service.output.identifier and service.output.kind == ServiceInputKind.IDSMember:
    return 2
  elif codel.inTypes:
    return 1
  else:
    if not service is None:
      for i in service.inputs():
	if i.kind == ServiceInputKind.IDSMember:
	  return 1
    return 0

def isDynamic(t):
  if t.kind() == IdlKind.Sequence:
    return True
  elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
    return isDynamic(t.unalias())
  elif t.kind() == IdlKind.Struct:
    s = t.asStructType()
    for m in s.members():
      if isDynamic(m.data()):
	return True
    return False
  else:
    return False

def dynamicMembers(t, name):
  if t.kind() == IdlKind.Sequence:
    return [(t, name)]
  elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
    return dynamicMembers(t.unalias(), name)
  elif t.kind() == IdlKind.Struct:
    s = t.asStructType()
    l = []
    for m in s.members():
      l.extend(dynamicMembers(m.data(), name + "." + m.key()))
    return l
  else:
    return []

def copyType(t, dest, src, useIsEmptyVar = True):
    if t.kind() == IdlKind.Sequence:
      s = t.asSequenceType()
      if(useIsEmptyVar):
	print "if(" + dest.replace(".", "_") + "_is_empty) {"
	print dest + ".length = 0;"
	print dest + ".data = 0;" 
	print "} else {"
      if isDynamic(s.seqType()):
	print dest + ".data = new " + MapTypeToCpp(s.seqType(), True) + "Cpp[" + src + ".length()];"

	counter = dest.replace(".", "_") + "_counter"
	print "for(int " + counter + " =0; " + counter + "<" + src + ".length(); ++" + counter + ") {"
	copyType(s.seqType(), dest + ".data[" + counter + "]", src + "[" + counter + "]", False)
	print "}"
      else:
	print dest + ".length = " + src + ".length();"
	print dest + ".data = (" + MapTypeToCpp(s.seqType())  + "*) " + src + ".get_buffer();"
      if(useIsEmptyVar):
	print "}"

    elif t.kind() == IdlKind.Array:
      s = t.asArrayType()
      counter = dest.replace(".", "_") + "_counter"
      print "for(int " + counter + "=0; " + counter + "<" + src + ".length(); ++" + counter + ") {"
      copyType(s.type(), dest + ".data[" + counter + "]", src + "[" + counter + "]")
      print "}"
    elif t.kind() == IdlKind.Struct:
      s = t.asStructType()
      for m in s.members():
	copyType(m.data(), dest + "." + m.key(), src + "." + m.key(), useIsEmptyVar)
    elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
      copyType(t.unalias(), dest, src, useIsEmptyVar)
    else:
      print dest + " = " + src + ";"

def copyTypeReverse(t, dest, src, useIsEmptyVar = True, parentIsEmpty = False):
    if t.kind() == IdlKind.Sequence:
      s = t.asSequenceType()
      if useIsEmptyVar:
	print "if(!" + src.replace(".", "_") + "_is_empty) {"
      if not parentIsEmpty or useIsEmptyVar:
	if isDynamic(s.seqType()):
	  counter = src.replace(".", "_") + "_counter"
	  print "for(int " + counter + "=0; " + counter + "<" + src + ".length(); ++" + counter  + ") {"
	  copyTypeReverse(s.seqType(), dest + "[" + counter + "]", src + ".data[" + counter + "]", False)
	  print "}"
	  print "delete[] " + src + ".data;"
      if useIsEmptyVar:
	print "} else if(" + src+ ".data) {"
      if parentIsEmpty or useIsEmptyVar:
	if isDynamic(s.seqType()):
	  print dest + ".replace(" + src + ".length(), " + src + ".length(), new " + MapTypeToCorbaCpp(s.seqType(), True) + "[" + src + ".length()]);" 

	  counter = src.replace(".", "_") + "_counter"
	  print "for(int " + counter + "=0; " + counter + "<" + src + ".length(); ++" + counter  + ") {"
	  copyTypeReverse(s.seqType(), dest + "[" + counter + "]", src + ".data[" + counter + "]", False, True)
	  print "}"
	else:
	  print "  " + dest  + ".replace(" + src + ".length, " + src+ ".length, (" + MapTypeToCorbaCpp(s.seqType())  + "*) "+ src + ".data);"
      if useIsEmptyVar:
	print "}"

    elif t.kind() == IdlKind.Array:
      s = t.asArrayType()
      counter = src.replace(".", "_") + "_counter"
      print "for(int " + counter + "=0; " + counter + "<" + src + ".length(); ++" + counter + ") {"
      copyTypeReverse(s.type(), dest + "[" + counter + "]", src + ".data[" + counter + "]", useIsEmptyVar, parentIsEmpty)
      print "}"
    elif t.kind() == IdlKind.Struct:
      s = t.asStructType()
      for m in s.members():
	copyTypeReverse(m.data(), dest + "." + m.key(), src + "." + m.key(), useIsEmptyVar, parentIsEmpty)
    elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
      copyTypeReverse(t.unalias(), dest, src, useIsEmptyVar, parentIsEmpty)
    else:
      print dest + " = " + src + ";"

def codelLock(codel, service=None):
    #for p in codel.inPorts:
      #print "  m_data->" + p + "_inport.wait();"
    for p in codel.outPorts:
      port = comp.port(p)
      if not isDynamic(port.idlType):
	continue
      seqs = dynamicMembers(port.idlType, p)
      newType = MapTypeToCpp(port.idlType, True) + "Cpp"
      print "  " + newType + " " + p + "_outport;"

      for x in seqs:  
	print "  bool " + x[1].replace(".", "_") + "_outport_is_empty = m_data->" + x[1] + ".length() == 0;"

      copyType(port.idlType, p + "_outport", "m_data->" + p + "_data")

    res = codelNeedsLock(codel, service)
    if res == 2:
      print "  m_data->idsMutex.acquire_write();"
    elif res == 1:
      print "  m_data->idsMutex.acquire_read();"

def codelRelease(codel, service=None):
    for p in codel.outPorts:
      port = comp.port(p)
      if not isDynamic(port.idlType):
	continue
      copyTypeReverse(port.idlType,  "m_data->" + p + "_data", p + "_outport")

    #for p in codel.inPorts:
      #print "  m_data->" + p + "_inport.post();"
    res = codelNeedsLock(codel, service)
    if res:
      print "  m_data->idsMutex.release();"


