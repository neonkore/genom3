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

def addressOf(t, s):
 if t.kind() == IdlKind.String:
   return s
 else:
    return "&" + s

def needsConversion(t):
  if t.isNative():
    return True
  if t.kind() == IdlKind.Long:
    return True
  elif t.kind() == IdlKind.Struct:
    return True
  elif t.kind() == IdlKind.Enum:
    return True
  elif t.kind() == IdlKind.Typedef or t.kind() == IdlKind.Named:
    return needsConversion(t.unalias())
  else:
    return False

def needsConversionFun(t):
  if t.kind() == IdlKind.Struct:
    return True
  elif t.kind() == IdlKind.Enum:
    return True
  elif t.kind() == IdlKind.Typedef or t.kind() == IdlKind.Named:
    return needsConversionFun(t.unalias())
  else: 
    return False

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

def cpp_arg(t,name):
  if t.identifier():
    cppType = MapTypeToCorbaCpp(t) + "_Corba"
  else:
    cppType = MapTypeToCorbaCpp(t)
  if t.kind() == IdlKind.Struct:  
    return "const " + cppType + " &" + name
  elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
    if t.unalias().kind() == IdlKind.Struct:
      return "const " + cppType + " &" + name
    else:
      return cppType + " " + name
  elif t.kind() == IdlKind.String:
    return "const " + cppType + " " + name
  else:
    return cppType + " " + name

def service_cpp_args(service, className=""):
    # create the args list
    args = ""
    for i in service.inputs():
	t = inputType(i)
	args += cpp_arg(t," in_" + i.identifier) + ", "
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

    output = MapTypeToCorbaCpp(outputType, True)
    if outputType.identifier():
      output += "_Corba"
    if isCorbaDynamic(outputType):
      output += "*"

    if className:
      return output + " " + className + "::" + service_cpp_args(service, className)
    else:
      return output + " " + service_cpp_args(service, className)

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
	if isDynamic(p.idlType):
	  proto += MapTypeToCpp(p.idlType) + "* " + port + "_outport, " 
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

def real_codel_call(codel, data_prefix="", service=None, useCopiedArgs = True):
  proto = ""
  if service is not None:
    for i in service.inputs():
      t = inputType(i)
      if needsConversion(t) and useCopiedArgs:
	proto += addressOf(inputType(i), i.identifier) + ", "
      else:
	proto += addressOf(inputType(i), " in_" + i.identifier) + ", "
    if service.output.identifier:
	proto += " &out_" + service.output.identifier + ", " 

  for type in codel.inTypes:
    t = comp.typeFromIdsName(type)
    proto += addressOf(t, data_prefix + type) + ", "
  for type in codel.outTypes:
    t = comp.typeFromIdsName(type)
    proto += addressOf(t, data_prefix + type) + ", "
  for port in codel.outPorts:
    p = comp.port(port)
    if isDynamic(p.idlType):
      proto += "&" + port + "_outport, "
    elif needsConversion(p.idlType):
      proto += "&" + port + ", "
    else:
      proto += "&" + data_prefix + port + "_data, " 
  for port in codel.inPorts:
    p = comp.port(port)
    if isDynamic(p.idlType):
      proto += "&" + port + "_inport, "
    elif needsConversion(p.idlType):
      proto += "&" + port + ", "
    else:
      proto += "&" + data_prefix + port + "_data, "  

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

def isCorbaDynamic(t):
  if t.kind() == IdlKind.String:
    return True
  elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
    return isCorbaDynamic(t.unalias())
  elif t.kind() == IdlKind.Struct:
    s = t.asStructType()
    for m in s.members():
      if isCorbaDynamic(m.data()):
	return True
    return False
  else:
    return False

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

def dynamicMembers(t, name, recursive = False):
  if t.kind() == IdlKind.Sequence:
    if recursive:
      s = t.asSequenceType()
      l = [(t, name)]
      l.extend(dynamicMembers(s.seqType(), name + ".data", True))
      return l
    else:
      return [(t, name)]
  elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
    return dynamicMembers(t.unalias(), name, recursive)
  elif t.kind() == IdlKind.Struct:
    s = t.asStructType()
    l = []
    for m in s.members():
      l.extend(dynamicMembers(m.data(), name + "." + m.key(), recursive))
    return l
  else:
    return []

def toIdentifier(n):
  res = n.replace(".", "_")
  res = res.replace("[", "")
  res = res.replace("]", "")
  res = res.replace("(*", "")
  res = res.replace(")", "")
  res = res.replace("m_data->", "")
  return res

def counterName(n):
  return toIdentifier(n) + "_counter"

def lengthVar(n):
  return toIdentifier(n) + "_length"

def copyType(t, dest, src):
    if t.kind() == IdlKind.Sequence:
      s = t.asSequenceType()
      if isDynamic(s.seqType()):
	print dest + ".data = new " + MapTypeToCpp(s.seqType(), True) + "[" + src + ".length()];"
	print dest + ".length = " + src + ".length();"

	counter = counterName(dest)
	print "for(int " + counter + " =0; " + counter + "<" + src + ".length(); ++" + counter + ") {"
	copyType(s.seqType(), dest + ".data[" + counter + "]", src + "[" + counter + "]")
	print "}"
      else:
	print dest + ".length = " + src + ".length();"
	print dest + ".data = (" + MapTypeToCpp(s.seqType())  + "*) " + src + ".get_buffer();"

    elif t.kind() == IdlKind.Array:
      s = t.asArrayType()
      counter = counterName(dest)
      print "for(int " + counter + "=0; " + counter + " < " + str(s.bounds()[0])  + "; ++" + counter + ") {"
      copyType(s.type(), dest + "[" + counter + "]", src + "[" + counter + "]")
      print "}"
    elif t.kind() == IdlKind.Struct:
      s = t.asStructType()
      for m in s.members():
	copyType(m.data(), dest + "." + m.key(), src + "." + m.key())
    elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
      copyType(t.unalias(), dest, src)
    else:
      copyTypeFromCorba(t, src, dest, False)
      #if needsConversionFun(t):
	#print "convertFromCorba_" + t.identifier() + "(&" + src + ", &" + dest + ");"
      #else:
	#print dest + " = " + src + ";"

def copyTypeReverse(t, dest, src, useIsEmptyVar = True, parentIsEmpty = False):
    if t.kind() == IdlKind.Sequence:
      s = t.asSequenceType()
      if not parentIsEmpty or useIsEmptyVar:
	if isDynamic(s.seqType()):
	  counter = counterName(src)
	  print "for(int " + counter + "=0; " + counter + " < " + src + ".length; ++" + counter  + ") {"
	  copyTypeReverse(s.seqType(), dest + "[" + counter + "]", src + ".data[" + counter + "]", False)
	  print "}"
	  print "delete[] " + src + ".data;"

    elif t.kind() == IdlKind.Array:
      s = t.asArrayType()
      counter = counterName(src)
      print "for(int " + counter + "=0; " + counter + "<" + str(s.bounds()[0]) + "; ++" + counter + ") {"
      copyTypeReverse(s.type(), dest + "[" + counter + "]", src + "[" + counter + "]", useIsEmptyVar, parentIsEmpty)
      print "}"
    elif t.kind() == IdlKind.Struct:
      s = t.asStructType()
      for m in s.members():
	copyTypeReverse(m.data(), dest + "." + m.key(), src + "." + m.key(), useIsEmptyVar, parentIsEmpty)
    elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
      copyTypeReverse(t.unalias(), dest, src, useIsEmptyVar, parentIsEmpty)
    else:
      copyTypeFromCorba(t, src, dest, True)
      #if needsConversionFun(t):
	#print "convertFromCorbaReverse_" + t.identifier() + "(&" + src + ", &" + dest + ");"
      #else:
	#print dest + " = " + src + ";"

def freeMemory(t, src):
    if t.kind() == IdlKind.Sequence:
      s = t.asSequenceType()
      if isDynamic(s.seqType()):
	counter = counterName(src)
	print "for(int " + counter + "=0; " + counter + " < " + src + ".length; ++" + counter  + ") {"
	freeMemory(s.seqType(), src + ".data[" + counter + "]")
	print "}"
	print "delete[] " + src + ".data;"

    elif t.kind() == IdlKind.Struct:
      s = t.asStructType()
      for m in s.members():
	freeMemory(m.data(), src + "." + m.key())
    elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
      freeMemory(t.unalias(), src)


def allocMemory(t, dest, scopedName):
    if t.kind() == IdlKind.Sequence:
      s = t.asSequenceType()
      seqType = MapTypeToCorbaCpp(s.seqType(), True)
      if s.seqType().identifier():
	seqType += "_Corba"

      print dest + ".replace(" + lengthVar(scopedName) + ", " + lengthVar(scopedName) + ", new " + seqType + "[" + lengthVar(scopedName) + "]);"
      print ""

      if isDynamic(s.seqType()):
	counter = counterName(dest)
	print "int " + counter + " = 0;"
	print "for(; " + counter + "<" + dest + ".length(); ++" + counter + ") {"
	allocMemory(s.seqType(), dest + "[" + counter + "]", scopedName + ".data")
	print "}"
      else:
	print "memset(" + dest + ".get_buffer() , 0, " + lengthVar(scopedName) + " * sizeof(" + seqType + "));"

    elif t.kind() == IdlKind.Struct:
      s = t.asStructType()
      for m in s.members():
	allocMemory(m.data(), dest + "." + m.key(), scopedName + "." + m.key())
    elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
      allocMemory(t.unalias(), dest, scopedName)


def sizeCodelSignature(port):
  sizeCodelArgs = ""
  for x in dynamicMembers(port.idlType, port.name + "_outport", True):
    sizeCodelArgs += "int *" + lengthVar(x[1]) + ", "

  for s in port.sizeCodel.inTypes:
    idstype = comp.typeFromIdsName(s);
    sizeCodelArgs += pointerTo(idstype) + " in_" + s + ", "
  for s in port.sizeCodel.outTypes:
    idstype = comp.typeFromIdsName(s);
    sizeCodelArgs += pointerTo(idstype) + " out_" + s + ", "
  for s in port.sizeCodel.inPorts:
    p = comp.port(s)
    sizeCodelArgs += pointerTo(port.idlType) + " " + s + "_inport, "
  for s in port.sizeCodel.outPorts:
    p = comp.port(s)
    sizeCodelArgs += pointerTo(port.idlType) + " " + s + "_outport, "

  return "int " + port.sizeCodel.name + "(" + sizeCodelArgs[:-2] + ")"

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

def copyCodelArgs(codel, service):
    if not service is None:
      for i in service.inputs():
	t = inputType(i)
	if needsConversion(t):
	  if needsConversionFun(t):
	    print MapTypeToCpp(t, True) + " " + i.identifier + ";"
	    print "convertFromCorba_" + t.identifier() + "(&in_" + i.identifier + ", &" + i.identifier + ");"
	  else:
	    print MapTypeToCpp(t, True) + " " + i.identifier + " = in_" + i.identifier + ";"

def codelLock(codel, service=None):
    #for p in codel.inPorts:
      #print "  m_data->" + p + "_inport.wait();"
    for p in codel.outPorts:
      port = comp.port(p)
      if not isDynamic(port.idlType):
	continue
      seqs = dynamicMembers(port.idlType, p + "_data.data")
      newType = MapTypeToCpp(port.idlType, True)
      print "  " + newType + " " + p + "_outport;"

      str = ""
      for x in seqs:  
	str += "m_data->" + x[1] + ".length() == 0 && "
      print "bool " +  p + "_outport_is_empty = " + str[:-3] + ";"

      print "if(" + p + "_outport_is_empty) {"

      callSizeCodel(port)

      print "  if(res >= 0) {"
      allocMemory(port.idlType, "m_data->" + p + "_data.data", p + "_outport")
      print "  }"
      print "}\n\n"

      copyType(port.idlType, p + "_outport", "m_data->" + p + "_data.data")


    for p in codel.inPorts:
      port = comp.port(p)
      if not isDynamic(port.idlType):
	continue
      newType = MapTypeToCpp(port.idlType, True)
      print "  " + newType + " " + p + "_inport;"
      copyType(port.idlType, p + "_inport", "m_data->" + p + "_data.data")


    res = codelNeedsLock(codel, service)
    if res == 2:
      print "  m_data->idsMutex.acquire_write();"
    elif res == 1:
      print "  m_data->idsMutex.acquire_read();"

    for p in codel.outPorts:
      port = comp.port(p)
      if isDynamic(port.idlType):
	continue
      
      if needsConversion(port.idlType):
	if needsConversionFun(port.idlType):
	  print MapTypeToCpp(port.idlType, True) + " " + port.name + ";"
	  print "convertFromCorba_" + port.idlType.identifier() + "(&m_data->" + port.name + "_data.data, &" + port.name + ");"
	else:
	  print MapTypeToCpp(port.idlType, True) + " " + port.name + " = m_data->" + port.name + "_data.data;"

    for p in codel.inPorts:
      port = comp.port(p)
      if isDynamic(port.idlType):
	continue
      
      if needsConversion(port.idlType):
	if needsConversionFun(port.idlType):
	  print MapTypeToCpp(port.idlType, True) + " " + port.name + ";"
	  print "convertFromCorba_" + port.idlType.identifier() + "(&m_data->" + port.name + "_data.data, &" + port.name + ");"
	else:
	  print MapTypeToCpp(port.idlType, True) + " " + port.name + " = m_data->" + port.name + "_data.data;"

def copyCodelArgsReverse(codel, service):
    if not service is None:
      for i in service.inputs():
	t = inputType(i)
	if needsConversion(t):
	  print "//in_" + i.identifier + " = " + i.identifier + ";"

def codelRelease(codel, service=None):
    # update dynamic ports
    for p in codel.outPorts:
      port = comp.port(p)
      if not isDynamic(port.idlType):
	if needsConversion(port.idlType):
	  if needsConversionFun(port.idlType):
	    print "convertFromCorbaReverse_" + port.idlType.identifier() + "(&" + port.name + ", &m_data->" + port.name + "_data.data);"
	  else:
	    print MapTypeToCpp(port.idlType, True) + " m_data->" + port.name + "_data = " + port.name + ";"
      else:
	copyTypeReverse(port.idlType,  "m_data->" + p + "_data.data", p + "_outport")
	freeMemory(port.idlType, p + "_outport")

    for p in codel.outPorts:
      print "m_data->" + port.name + ".write();"

    for p in codel.inPorts:
      port = comp.port(p)
      print "m_data->" + p + ".read();"
      if not isDynamic(port.idlType):
	if needsConversion(port.idlType):
	  if needsConversionFun(port.idlType):
	    print "convertFromCorbaReverse_" + port.idlType.identifier() + "(&" + port.name + ", &m_data->" + port.name + "_data.data);"
	  else:
	    print "m_data->" + port.name + "_data.data = " + port.name + ";"
      else:
	freeMemory(port.idlType, p + "_inport")

    #for p in codel.inPorts:
      #print "  m_data->" + p + "_inport.post();"
    res = codelNeedsLock(codel, service)
    if res:
      print "  m_data->idsMutex.release();"


