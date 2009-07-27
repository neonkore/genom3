# -*- coding: utf-8 -*-
from string import upper

# copy some elements to not modify the ast
#servicesMap = comp.servicesMap()
servicesDict = dict()
for s in comp.servicesMap():
  servicesDict[s.key()] = s.data()

# copy the type vector to append elements
typesVect = []
for t in comp.typesVect():
  typesVect.append(t)

# name of the special ids member for connect requests
connectIDSMember = "_connect_str"

# we have to intercept this call because the connect service use 
# a field that is not really in the ids
def typeFromIdsName(name):
  if name == connectIDSMember:
    return StringType(256)
  else:
    return comp.typeFromIdsName(name)

def inputType(i):
  if i.kind == ServiceInputKind.IDSMember:
    return typeFromIdsName(i.identifier)
  else:
    return i.type

def idsNameForType(t):
  """ Returns the ids name for values with this type."""
  return "_" + MapTypeToC(t, True).replace(' ', '_')

ids_members = []
def idsMemberForInput(i, service):
  """ Find the correct IDS member to use to store the input i
  belonging to this service."""
  if i.kind == ServiceInputKind.IDSMember:
    return i.identifier
  else:
    # control service can share their input
    # (when they just have one input)
    if service.type == ServiceType.Control:
      name = idsNameForType(i.type)
      if name not in ids_members: # create a new ids member for this type
	IDSType.addMember(i.type, name)
	ids_members.append(name)
      return name
    else:
      name = service.name + "_" + i.identifier
      IDSType.addMember(i.type, name)
      return name

def typeProtoPrefix(t):
    """ Returns the prefix corresponding to this type to use for print, scan, etc. functions."""
    prefix = ""
    if t.kind() == IdlKind.Named:
	n = t.asNamedType()
	return typeProtoPrefix(n.type())
    if t.kind() == IdlKind.Struct:
	prefix = "struct_"
    elif t.kind() == IdlKind.Enum:
	prefix = "enum_"
    elif t.kind() == IdlKind.Typedef:
	return ""
    elif t.kind() == IdlKind.Char or t.kind() == IdlKind.Octet or t.kind() == IdlKind.Boolean:
        prefix = "char"
    elif t.kind() == IdlKind.Short or t.kind() == IdlKind.WChar or t.kind() == IdlKind.Long or t.kind() == IdlKind.LongLong:
        prefix = "int"
    elif t.kind() == IdlKind.UShort or t.kind() == IdlKind.ULong or t.kind() == IdlKind.ULongLong:
        prefix = "int"
    elif t.kind() == IdlKind.Float or t.kind() == IdlKind.Double:
        prefix = "double"
    elif t.kind() == IdlKind.String or t.kind() == IdlKind.WString:
	prefix = "string"
    return prefix + t.identifier()

def dynamicPortType(port):
   if port.idlType.kind == IdlKind.Sequence:
     return port.idlType.asSequenceType().seqType()
   else:
     t = port.idlType.unalias()
     if t is not None and t.kind() == IdlKind.Sequence:
       return t.asSequenceType().seqType()
     else:
	return BaseType.charType

# copy ids type
IDSType = StructType()
IDSType.setIdentifier(comp.IDSType.identifier())
s = comp.IDSType.unalias().asStructType()
for m in s.members():
    IDSType.addMember(m.data, m.key)

# add a member in the ids for connect services 
if inports:
  IDSType.addMember(StringType(256), connectIDSMember)

# create connect services for each inport
for port in inports:
  name = "connect" + port.name
  s = Service(name)
  s.type = ServiceType.Control

  i = ServiceInput()
  i.identifier = connectIDSMember
  i.kind = ServiceInputKind.IDSMember
  s.addInput(i)
  c = Codel(name + "Exec")
  s.addCodel("control", c)
  servicesDict[name] = s

# create ids member for dynamic posters
for port in outports:
  if isDynamic(port.idlType):
    IDSType.addMember(port.idlType, port.name + "_outport")
for port in inports:
  if isDynamic(port.idlType):
    IDSType.addMember(port.idlType, port.name + "_inport")

class ServiceInfo:
  """ This class is used to store all types of information about a service to
  avoid computing them again and again. Only the __init__ function is used to fill 
  the class."""
  def __init__(self, service):
    # inputs
    if not service.inputs():
      self.inputFlag = False
      self.inputSize = "0"
      self.inputNamePtr = "NULL"
      self.inputRefPtr = "NULL"
      self.inputFlatList = []
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
	self.inputName = idsMemberForInput(service.inputs()[0], service)
	self.inputType = inputType(service.inputs()[0])

      self.inputTypePtr = pointerTo(self.inputType)
      self.inputTypeProto = typeProtoPrefix(self.inputType)

      self.inputSize = "sizeof(" + MapTypeToC(self.inputType, True) + ")"
      if self.inputFlag and self.inputType.kind() == IdlKind.String:
	self.inputSize = str(self.inputType.asStringType().bound())
	if self.inputSize == "0":
	  self.inputSize = "0"

      self.inputNamePtr = addressOf(self.inputType, self.inputName)
      self.inputRefPtr = "&((*" + comp.name() + "DataStrId)." + self.inputName + ")" 
 
      if self.inputType.kind() == IdlKind.Struct or self.inputType.kind() == IdlKind.Typedef \
      or self.inputType.kind() == IdlKind.Array or self.inputType.kind() == IdlKind.Named:
	  self.inputNewline = "1"
      else:
	  self.inputNewline = "0"

      self.inputFlatList = flatStruct(self.inputType, self.inputName, ".") 
      if self.inputType.kind() == IdlKind.String:
	  st = self.inputType.asStringType()
	  self.inputVarDecl = "char " + self.inputName + "[" + str(st.bound()) + "]"
      else:
	  self.inputVarDecl = MapTypeToC(self.inputType,True) + " " + self.inputName

      self.signatureProto = ""
      self.userSignatureProto = ""
      for i in service.inputs():
	  idstype = inputType(i);
	  self.userSignatureProto += pointerTo(idstype) + " in_" + i.identifier + ", "
      if self.inputFlag:
	  self.signatureProto += pointerTo(self.inputType) + " in_" + self.inputName + ", "
      if service.output.identifier:
	  idstype = inputType(service.output);
	  self.signatureProto += pointerTo(idstype) + " out_" + service.output.identifier + ", "  
	  self.userSignatureProto += pointerTo(idstype) + " out_" + service.output.identifier + ", "  

    # outputs
    if not service.output.identifier:
      self.outputFlag = False
      self.outputSize = "0"
      self.outputNamePtr = "NULL"
      self.outputRefPtr = "NULL"
      self.outputFlatList = []
    else:
      self.outputFlag = True
      self.outputName = idsMemberForInput(service.output, service)
      self.outputType = inputType(service.output)
      self.outputTypeC = MapTypeToC(self.outputType,True)
      self.outputTypeProto = typeProtoPrefix(self.outputType)
      self.outputTypePtr = pointerTo(self.outputType)

      self.outputSize = "sizeof(" + MapTypeToC(self.outputType, True) + ")"

      if(self.outputType.kind() == IdlKind.String):
	  self.outputNamePtr = self.outputName
      else:
	  self.outputNamePtr = "&" + self.outputName
      self.outputRefPtr = "&((*" + comp.name() + "DataStrId)." + self.outputName + ")" 

      if self.outputType.kind() == IdlKind.Struct or self.outputType.kind() == IdlKind.Typedef \
      or self.outputType.kind() == IdlKind.Array or self.outputType.kind() == IdlKind.Named:
	self.outputNewline = "1"
      else:
	self.outputNewline = "0"

      self.outputFlatList = flatStruct(self.outputType, self.outputName, ".")
      if self.outputType.kind() == IdlKind.String:
	  st = self.outputType.asStringType()
	  self.outputVarDecl = "char " + self.outputName + "[" + str(st.bound()) + "]"
      else:
	  self.outputVarDecl = MapTypeToC(self.outputType,True) + " " + self.outputName

    # other attributes
    self.controlFuncFlag = service.hasCodel("control")
    self.controlFuncParams = ""
    if self.controlFuncFlag:
      for type in service.codel("control").inTypes:
	self.controlFuncParams += ", & SDI_F->" + type;
      for type in service.codel("control").outTypes:
	self.controlFuncParams += ", & SDI_F->" + type;

# create serviceInfo objects
services_info_dict = dict()
for name, service in servicesDict.iteritems():
    services_info_dict[name] = ServiceInfo(service)    

def convertFun(t):
    """ returns the function to use to convert from a const char* to type t."""
    if t.kind() == IdlKind.Char or t.kind() == IdlKind.Octet or t.kind() == IdlKind.Boolean:
        return ""
    elif t.kind() == IdlKind.Short or t.kind() == IdlKind.WChar or t.kind() == IdlKind.Long or t.kind() == IdlKind.LongLong:
        return "atoi"
    elif t.kind() == IdlKind.UShort or t.kind() == IdlKind.ULong or t.kind() == IdlKind.ULongLong:
        return "atoi"
    elif t.kind() == IdlKind.Float or t.kind() == IdlKind.Double:
        return "atof"
    return ""

def formatStringForType(t):
   """ Returns the printf format string corresponding to the type t."""
   if t.kind() == IdlKind.Char or t.kind() == IdlKind.Octet or t.kind() == IdlKind.Boolean:
       return "%c";
   elif t.kind() == IdlKind.Short or t.kind() == IdlKind.WChar or t.kind() == IdlKind.Long or t.kind() == IdlKind.LongLong:
       return "%d"
   elif t.kind() == IdlKind.UShort or t.kind() == IdlKind.ULong or t.kind() == IdlKind.ULongLong:
       return "%u" 
   elif t.kind() == IdlKind.Float or t.kind() == IdlKind.Double:
       return "%f"
   elif t.kind() == IdlKind.String or t.kind() == IdlKind.WString:
       return "%s"
   else:
       return ""

def sizeOfType(t):
    """ Returns a string to compute the size of type t."""
    if t.kind() == IdlKind.String:
	s = t.asStringType()
	return str(s.bound())
    else:
	return "sizeof(" + MapTypeToC(t,True) + ")"

def sizeOfIdsMember(name):
    """ Returns a string to compute the size of the IDS member name."""
    type = typeFromIdsName(name)
    if type is None:
	return "0"
    return sizeOfType(type)

# error related functions
def encodeError(i):
    return comp.uniqueId << 16 | 0x8000 | 100 << 8 | i

def isPeriodic():
    for t in comp.tasksMap():
	if t.data().period > 0:
	    return True
    return False

def serviceDescString(s):
    if s.type == ServiceType.Exec:
	# if reentrant return "(nE)"
	return "(E)"
    elif s.type == ServiceType.Init:
	return "(I)"
    return ""

def findServiceWithSameOutput(service, inputName):
    """ Find other services with an output corresponding to the service's input """
    l = []
    for name, ss in servicesDict.iteritems():
	if name == service.name:
	    break; 
	if ss.output == inputName:
	    l.append(ss)
    return l

def codel_signature(codel, service=None):
  """ Creates the signature (withot return value) of the internal function corresponding to a codel. """
  proto = codel.name + "_codel(";
  if service is not None:
    serviceInfo = services_info_dict[service.name]
    proto += serviceInfo.signatureProto

  for type in codel.inTypes:
    idstype = typeFromIdsName(type);
    proto += pointerTo(idstype) + " in_" + type + ", ";
  for type in codel.outTypes:
    idstype = typeFromIdsName(type);
    proto += pointerTo(idstype) + " out_" + type + ", ";
  proto +=  "int *report)"
  return proto

def codelSignatureFull(codel, service):
    """ Creates the full signature of the internal function corresponding to a codel. """
    if service.type != ServiceType.Exec or codel.key() == "control":
	return "STATUS " + codel_signature(codel.data(), service)
    else:
	return "ACTIVITY_EVENT " + codel_signature(codel.data(), service)

def real_codel_call(codel, service=None):
  """ Creates the string to call the user codel. """
  proto = ""
  if service is not None:
    inputPrefix = ""
    if len(service.inputs()) > 1:
	serviceInfo = services_info_dict[service.name]
	inputPrefix = serviceInfo.inputName + "->"
    for i in service.inputs():
	if inputPrefix:
	    proto += addressOf(inputType(i), "in_" + inputPrefix + i.identifier) + ", "
	else:
	    proto += "in_" + idsMemberForInput(i, service) + ", "
    if service.output.identifier:
	proto += " out_" + service.output.identifier + ", "

  for type in codel.inTypes:
    proto += "in_" + type + ", ";
  for type in codel.outTypes:
    proto += "out_" + type + ", ";
  for p in codel.outPorts:
    port = comp.port(p)
    if isDynamic(port.idlType):
      proto += "&SDI_F->" + p + "_outport, "
    else:
      proto +=  p + "_outport, "
  for p in codel.inPorts:
    port = comp.port(p)
    if isDynamic(port.idlType):
      proto += "&SDI_F->" + p + "_inport, "
    else:
      proto +=  p + "_inport, "
  proto = codel.name + "(" + proto[:-2] + ")"
  return proto

def nbExecService():
    """ Computes the number of exec services in the component """
    count = 0
    for name, service in servicesDict.iteritems():
	if service.type != ServiceType.Control:
	    count += 1
    return count

def maxServiceNameLength():
    maxLen = 0
    for name, service in servicesDict.iteritems():
	maxLen = max(maxLen, len(name))
    return maxLen

def typeSize(t):
    """ Returns the size of the type (or rather an estimation of it).
    This is used to compute the max resquest arg and output size (in posterLib.h)."""
    if t.kind() == IdlKind.Named:
	return typeSize(t.asNamedType().type())
    if t.kind() == IdlKind.Struct:
	s = t.asStructType()
	res = 0
	for m in s.members():
	    res += typeSize(m.data)
	return res
    elif t.kind() == IdlKind.Typedef:
	return typeSize(t.asTypedefType().type())
    elif t.kind() == IdlKind.Array:
	a = t.asArrayType()
	res = 1
	for x in a.bounds():
	  res *= x
	return res * typeSize(a.type())
    elif t.kind() == IdlKind.Sequence:
	s = t.asSequenceType()
	return s.bound() * typeSize(s.seqType())
    elif t.kind() == IdlKind.Char or t.kind() == IdlKind.Octet or t.kind() == IdlKind.Boolean:
        return 4
    elif t.kind() == IdlKind.Short or t.kind() == IdlKind.WChar or t.kind() == IdlKind.Long or t.kind() == IdlKind.LongLong or t.kind() == IdlKind.Enum:
	return 8
    elif t.kind() == IdlKind.UShort or t.kind() == IdlKind.ULong or t.kind() == IdlKind.ULongLong:
        return 8
    elif t.kind() == IdlKind.Float or t.kind() == IdlKind.Double:
        return 16
    elif t.kind() == IdlKind.String or t.kind() == IdlKind.WString:
	s = t.asStringType()
	if s is None:
	  return 1024
	else:
	  return s.bound()
    return 0

def maxArgsSize():
    """ compute the max request size """
    res = 8
    for name, service in servicesDict.iteritems():
      serviceInfo = services_info_dict[name]
      if serviceInfo.inputFlag:
	res = max(res, typeSize(serviceInfo.inputType))
    return res

def maxOutputSize():
    """ compute the max result size """
    res = 8
    for name, service in servicesDict.iteritems():
      serviceInfo = services_info_dict[name]
      if serviceInfo.outputFlag:
	res = max(res, typeSize(serviceInfo.outputType))
    return res

def computeTotalSize(t, name, addStructSize = True):
  """ Prints the string used to compute the total size of the static elements of a type. This is used
  when allocating sequences."""
  if t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
    return computeTotalSize(t.unalias(), name, addStructSize)
  elif t.kind() == IdlKind.Sequence:
    s = t.asSequenceType()
    res = computeTotalSize(s.seqType(), name + ".data")

    if addStructSize:
      structSize = "2*sizeof(int) + sizeof(" + MapTypeToC(s.seqType(), True) + "*) + "
    else:
      structSize = ""

    if res:
      return structSize + lengthVar(name) + " * ( " + res + ")" 
    else:
      return structSize + lengthVar(name) + "* (sizeof(" + MapTypeToC(s.seqType(), True) + "))" 

  elif t.kind() == IdlKind.Struct:
    s = t.asStructType()
    if addStructSize:
      str = "sizeof(" + MapTypeToC(t, True) + ")"
    else:
      str = "0"
    for m in s.members():
      res = computeTotalSize(m.data, name + "." + m.key, False)
      if res:
	str += " + " + res
    return str
  else :
    return ""

def copyType(t, dest, src, allocateMemory=True):
    """ Updates the IDS copy of a sequence type from the value stored in shared memory.
    If allocateMemory is True (for InPorts), memory will be allocated to store sequences."""
    if t.kind() == IdlKind.Sequence:
      s = t.asSequenceType()
      seqType = MapTypeToC(s.seqType(), True)
      if isDynamic(s.seqType()):
	if allocateMemory:
	  print dest + ".data = malloc(" + src + ".length * sizeof(" + seqType + "));"
	  print dest + ".length = " + src + ".length;"

	print seqType + "* " + toIdentifier(src) + "_name = (" + seqType + "*) (start + currentOffset);"
	print "currentOffset += " + src + ".length * sizeof(" + seqType + ");"

	counter = counterName(dest)
	print "int " + counter + " = 0;"
	print "for(; " + counter + "<" + src + ".length; ++" + counter + ") {"
	copyType(s.seqType(), dest + ".data[" + counter + "]", toIdentifier(src) + "_name[" + counter + "]", allocateMemory)
	print "}"

      else:
	if allocateMemory:
	  print dest + ".data = (" + seqType + "*) (start + currentOffset);"
	  print dest + ".length = " + src + ".length;"
	print "currentOffset += " + src + ".length * sizeof(" + seqType + ");"

    elif t.kind() == IdlKind.Struct:
      s = t.asStructType()
      for m in s.members():
	copyType(m.data, dest + "." + m.key, src + "." + m.key, allocateMemory)
    elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
      copyType(t.unalias(), dest, src, allocateMemory)
    elif t.kind() == IdlKind.Array:
      s = t.asArrayType()
      counter = counterName(dest)
      print "int " + counter + " = 0;"
      print "for(; " + counter + "<" + str(s.bounds()[0]) + "; ++" + counter + ") {"
      copyType(s.type(), dest + "[" + counter + "]", src + "[" + counter + "]", allocateMemory)
      print "}"
    elif t.kind() == IdlKind.String:
      s = t.asStringType()
      print "strncpy(" + dest + ", " + src + ", " + str(s.bound()) + ");"
    else:
      print dest + " = " + src + ";"

def copyTypeReverse(t, dest, src):
    """ Updates the shared memory contents after a codel call. """
    if t.kind() == IdlKind.Sequence:
      s = t.asSequenceType()
      if isDynamic(s.seqType()):
	counter = counterName(dest)
	print "int " + counter + " = 0;"
	print "for(; " + counter + "<" + src + ".length; ++" + counter + ") {"
	copyTypeReverse(s.seqType(), dest + ".data[" + counter + "]", src + ".data[" + counter + "]")
	print "}"

    elif t.kind() == IdlKind.Array:
      s = t.asArrayType()
      counter = counterName(dest)
      print "int " + counter + " = 0;"
      print "for(; " + counter + "<"  + str(s.bounds()[0]) + "; ++" + counter + ") {"
      copyTypeReverse(s.type(), dest + "[" + counter + "]", src + "[" + counter + "]")
      print "}"
    elif t.kind() == IdlKind.String:
      s = t.asStringType()
      print "strncpy(" + dest + ", " + src + ", " + str(s.bound()) + ");"
    elif t.kind() == IdlKind.Struct:
      s = t.asStructType()
      for m in s.members():
	copyTypeReverse(m.data, dest + "." + m.key, src + "." + m.key)
    elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
      copyTypeReverse(t.unalias(), dest, src)
    else:
      print dest + " = " + src + ";"

def allocateMemory(t, dest, idsDest, scopedName):
    """ Allocate the memory requested by the user after the size codel has been called."""
    if t.kind() == IdlKind.Sequence:
      s = t.asSequenceType()
      seqType = MapTypeToC(s.seqType(), True)
      print dest + ".data = (" + seqType + "*) (start + currentOffset);"
      print dest + ".length = " + lengthVar(scopedName) + ";"
      print "currentOffset += " + lengthVar(scopedName) + " * sizeof(" + seqType + ");"
      print ""

      if isDynamic(s.seqType()):
	print idsDest + ".data = malloc(" + lengthVar(scopedName) + " * sizeof(" + seqType + "));"
      else:
	print idsDest + ".data = " + dest + ".data;"
	print "memset(" + idsDest + ".data , 0, " + lengthVar(scopedName) + " * sizeof(" + seqType + "));"

      print idsDest + ".length = " + lengthVar(scopedName) + ";"
      print ""

      if isDynamic(s.seqType()):
	counter = counterName(dest)
	print "int " + counter + " = 0;"
	print "for(; " + counter + "<" + dest + ".length; ++" + counter + ") {"
	allocateMemory(s.seqType(), dest + ".data[" + counter + "]", idsDest + ".data[" + counter + "]", scopedName + ".data")
	print "}"

    elif t.kind() == IdlKind.Struct:
      s = t.asStructType()
      for m in s.members():
	allocateMemory(m.data, dest + "." + m.key, idsDest + "." + m.key, scopedName + "." + m.key)
    elif t.kind() == IdlKind.Named or t.kind() == IdlKind.Typedef:
      allocateMemory(t.unalias(), dest, idsDest, scopedName)

def codelLock(codel, service = None):
  """ Prepare the execution of a user codel. Finds posters pointers, initialize dynamic ports if 
  necessary, etc."""
  for port in codel.outPorts:
    p = comp.port(port)
    posterId = upper(comp.name()) + "_" + upper(port) + "_POSTER_ID"
    posterAddr = port + "_outport"
    posterType = MapTypeToC(p.idlType, True);

    if not isDynamic(p.idlType):
      print "/* find a pointer to <!port!> poster*/"
      print posterType + "* " + posterAddr + " = posterAddr(" + posterId + ");"
      print "if ("+posterAddr+" == NULL) {"
      print "  *report = errnoGet();"
      print "  return ETHER;"
      print "}"
      continue

    #seqs = dynamicMembers(p.idlType, posterAddr)
    #for x in seqs:  
      #print "  char " + isEmptyVar(x[1]) + " = SDI_F->" + x[1] + ".length == 0;"

    print "if(" + posterId + " != NULL) {"
    print "posterTake(" + upper(comp.name()) + "_" + upper(port) + "_POSTER_ID, POSTER_WRITE);"
    print "} else {"
    sizeCodelArgs = ""
    for x in dynamicMembers(p.idlType, posterAddr, True):
      print "int " + lengthVar(x[1]) + " = 0;"
      sizeCodelArgs += "&" + lengthVar(x[1]) + ", "

    for s in p.sizeCodel.inTypes:
      sizeCodelArgs += "&SDI_F->" + s + ", "
    for s in p.sizeCodel.outTypes:
      sizeCodelArgs += "&SDI_F->" + s + ", "
    for s in p.sizeCodel.inPorts:
      sizeCodelArgs += s + "_inport, "
    for s in p.sizeCodel.outPorts:
      sizeCodelArgs += s + "_outport, "

    print "int res = " + p.sizeCodel.name + "(" + sizeCodelArgs[:-2] + ");"
    print "if(res >= 0) {"

    # compute the total size of the poster
    print "int totalSize = " + computeTotalSize(p.idlType, "(*" + posterAddr + ")") + ";"

    # allocate the space in shared memory
    print "if(posterCreate(" + upper(comp.name()) + "_" + upper(p.name) + "_POSTER_NAME, totalSize, &("+posterId+")) != OK) {"
    print "  *report = S_" + comp.name() + "_stdGenoM_CONTROL_CODEL_ERROR;"
    print "  return ETHER;"
    print "}"

    # copy the data
    print "posterTake(" + posterId + ", POSTER_WRITE);"
    print posterType + " *" + posterAddr + " = posterAddr(" + posterId + ");"
    print "int currentOffset = sizeof(" + posterType + ");"
    print "char *start = (char*) " + posterAddr + ";"

    # allocate the memory
    allocateMemory(p.idlType, "(*" + posterAddr + ")", "SDI_F->" + p.name + "_outport", posterAddr)
 
    print "}"
    print "}"
  
  for port in codel.inPorts:
    p = comp.port(port)
    posterId = upper(comp.name()) + "_" + upper(port) + "_POSTER_ID"
    posterAddr = port + "_inport"
    posterType = MapTypeToC(p.idlType, True);

    print "/* find a pointer to <!port!> poster*/"
    print posterType + " *" + posterAddr + " = posterAddr(" + posterId + ");"
    print "if ("+posterAddr+" == NULL) {"
    print "  *report = errnoGet();"
    print "  return ETHER;"
    print "}"

    if not isDynamic(p.idlType):
      continue

    print "posterTake(" + posterId + ", POSTER_READ);"

    print "int currentOffset = sizeof(" + posterType + ");"
    print "char *start = (char*) " + posterAddr + ";"

    copyType(p.idlType,  "SDI_F->" + posterAddr,  "(*" + posterAddr + ")", False)

def codelRelease(codel, service=None):
  """  Cleanup after a codel has been called. Updates posters in shared memory if necessary. """
  for port in codel.outPorts:
    p = comp.port(port)
    posterId = upper(comp.name()) + "_" + upper(port) + "_POSTER_ID"

    if not isDynamicPort(p): 
      print "posterGive(" + posterId + ");"
      continue
    
    posterType = MapTypeToC(p.idlType, True);
    posterAddr = port + "_outport"

    print "if(" + posterId + " != NULL) {"
    print posterType + "* " + posterAddr + " = posterAddr(" + posterId + ");"
    copyTypeReverse(p.idlType, "(*" + posterAddr + ")", "SDI_F->" + p.name + "_outport")  
    print "posterGive(" + posterId + ");\n\n"
    print "}"

  for port in codel.inPorts:
    posterId = upper(comp.name()) + "_" + upper(port) + "_POSTER_ID"
    print "posterGive(" + posterId + ");"


# try to find an init service
def findInitService():
  i=-1
  for name,service in servicesDict.iteritems():
    i += 1
    if service.type == ServiceType.Init:
      return service, i
  return 0,-1

initService,initServiceNb = findInitService()

# other vars
nbServices = len(servicesDict)
abortRequestNum = nbServices;
internalDataType = MapTypeToC(IDSType,True)
periodicFlag = isPeriodic()

shouldGenerateOpenPRS = "#" # do not generate openrps related code
shouldGenerateTcl = ""
shouldGenerateServer = ""
pkgname = comp.name()

if shouldGenerateOpenPRS == "":
  genOpenprs = "yes"
else:
  genOpenprs = "no"

if shouldGenerateTcl == "":
  genTcl = "yes"
else:
  genTcl = "no"

