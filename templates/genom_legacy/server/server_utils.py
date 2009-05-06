# -*- coding: utf-8 -*-
from string import upper

servicesMap = comp.servicesMap()
connectIDSMember = "_connect_str"

# we have to intercept this call because the connect service use 
# a field that is not really in the ids
def typeFromIdsName(name):
  if name == connectIDSMember:
    return StringType(256)
  else:
    return comp.typeFromIdsName(name)

# returns a flat list of the structure of a type
def flatStructRec(t, name_prefix):
    if t.kind() == IdlKind.Struct:
	s = t.asStructType()
	l = [] 
	for m in s.members():
	    l.extend(flatStruct(m.data(), name_prefix+"__"+m.key()))
	return l
    else:
	return (name_prefix, t)

def flatStruct(t, name):
    if t.kind() == IdlKind.Named:
	n = t.asNamedType()
	return flatStruct(n.type(), name)
    if t.kind() == IdlKind.Struct:
	s = t.asStructType()
	l = [] 
	for m in s.members():
	    l.extend(flatStruct(m.data(), m.key()))
	return l
    else:
	return [(t, name)]   

def convertFun(t):
    if t.kind() == IdlKind.Char or t.kind() == IdlKind.Octet:
        return ""
    elif t.kind() == IdlKind.Short or t.kind() == IdlKind.WChar or t.kind() == IdlKind.Long or t.kind() == IdlKind.LongLong:
        return "atoi"
    elif t.kind() == IdlKind.UShort or t.kind() == IdlKind.ULong or t.kind() == IdlKind.ULongLong:
        return "atoi"
    elif t.kind() == IdlKind.Float or t.kind() == IdlKind.Double:
        return "atof"
    return ""

def formatStringForType(t):
   if t.kind() == IdlKind.Char or t.kind() == IdlKind.Octet:
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

def typeProtoPrefix(t):
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
    elif t.kind() == IdlKind.Char or t.kind() == IdlKind.Octet:
        prefix = "char"
    elif t.kind() == IdlKind.Short or t.kind() == IdlKind.WChar or t.kind() == IdlKind.Long or t.kind() == IdlKind.LongLong:
        prefix = "int"
    elif t.kind() == IdlKind.UShort or t.kind() == IdlKind.ULong or t.kind() == IdlKind.ULongLong:
        prefix = "uint"
    elif t.kind() == IdlKind.Float or t.kind() == IdlKind.Double:
        prefix = "double"
    elif t.kind() == IdlKind.String or t.kind() == IdlKind.WString:
	prefix = "string"
    return prefix + t.identifier()

def sizeOfType(t):
    if t.kind() == IdlKind.String:
	s = t.asStringType()
	return str(s.bound())
    else:
	return "sizeof(" + t.toCType(True) + ")"

def sizeOfIdsMember(name):
    type = typeFromIdsName(name)
    if type == None:
	return "0"
    return sizeOfType(type)

# try to find an init service
def findInitService():
  i=-1
  for s in servicesMap:
    i += 1
    if s.data().type == ServiceType.Init:
      return s.data(), i
  return 0,-1

initService,initServiceNb = findInitService()

# create a list of out ports, because we don't use inports
outports = []
inports = []
for p in comp.portsMap():
    if p.data().type == PortType.Outgoing:
	outports.append(p.data())
    else:
	inports.append(p.data())

# error related functions
def createErrorList():
  l = []
  for s in servicesMap:
    for e in s.data().errorMessages():
	l.append(e)
  for t in comp.tasksMap():
    for e in t.data().errorMessages():
	l.append(e)
  return set(l)

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
    l = []
    # find another service with an output corresponding to the service's input
    for it in servicesMap:
	ss = it.data()
	if ss.name == service.name:
	    break; 
	if ss.output == inputName:
	    l.append(ss)
    return l

# creates the signature of the function corresponding to a codel
def pointerTo(t):
  s = t.toCType(True)
  if t.kind() == IdlKind.String:
    return s
  else:
    return s+"*"

def codel_signature(codel):
  proto = codel.name + "_codel(";
  for type in codel.inTypes:
    idstype = typeFromIdsName(type);
    proto = proto + pointerTo(idstype) + " in_" + type + ", ";
  for type in codel.outTypes:
    idstype = typeFromIdsName(type);
    proto = proto + pointerTo(idstype) + " out_" + type + ", ";
  proto = proto + "int *report)"
  return proto

def codelSignatureFull(codel, service):
    if service.type == ServiceType.Control or codel.key() == "control":
	return "STATUS " + codel_signature(codel.data())
    else:
	return "ACTIVITY_EVENT " + codel_signature(codel.data())

def real_codel_signature(codel):
  proto = ""
  for type in codel.inTypes:
    idstype = typeFromIdsName(type);
    proto += pointerTo(idstype) + " in_" + type + ", ";
  for type in codel.outTypes:
    idstype = typeFromIdsName(type);
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

def real_codel_call(codel):
  proto = ""
  for type in codel.inTypes:
    proto += "in_" + type + ", ";
  for type in codel.outTypes:
    proto += "out_" + type + ", ";
  for port in codel.outPorts:
    proto += "outport_" + port + ", "; 
  for port in codel.inPorts:
    proto += "inport_" + port + ", "; 
  proto = codel.name + "(" + proto[:-2] + ")"
  return proto

def nbExecService():
    count = 0
    for s in servicesMap:
	if s.data().type != ServiceType.Control:
	    count += 1
    return count

# create connect services for each inport


for port in inports:
  name = "connect" + port.name
  s = Service(name)
  s.type = ServiceType.Control
  s.addInput(connectIDSMember)
  c = Codel(name + "Exec")
  c.addInType(connectIDSMember)
  s.addCodel("control", c)
  servicesMap[name] = s

# add a member in the ids for connect services 
IDSType = comp.IDSType
if len(inports) > 0:
  t = comp.IDSType.unalias()
  s = t.asStructType()
  if s != None:
    # copy ids type and add a member
    IDSType = StructType()
    IDSType.setIdentifier(comp.IDSType.identifier())
    IDSType.addMember(StringType(256), connectIDSMember)
    for m in s.members():
	IDSType.addMember(m.data(), m.key())

# other vars
nbServices = len(servicesMap)
abortRequestNum = nbServices;
internalDataType = IDSType.toCType(True)
periodicFlag = isPeriodic()

shouldGenerateOpenPRS = "#" # do not generate openrps related code
shouldGenerateTcl = "#"
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

