# -*- coding: utf-8 -*-
from string import upper

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
       return "%f";
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
    return prefix + t.identifier()

def sizeOfIdsMember(name):
    type =  comp.typeFromIdsName(name)
    if type == None:
	return "0"
    return "sizeof(" + type.toCType(True) + ")"

# try to find an init service
def findInitService():
  i=-1
  for s in comp.servicesMap():
    i += 1
    if s.data().type == ServiceType.Init:
      return s.data(), i
  return 0,-1

initService,initServiceNb = findInitService()

# create a list of out ports, because we don't use inports
outports = []
for p in comp.portsMap():
    if p.data().type == PortType.Outgoing:
	outports.append(p.data())

# error related functions
def createErrorList():
  l = []
  for s in comp.servicesMap():
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
    for it in comp.servicesMap():
	ss = it.data()
	if ss.name == service.name:
	    break; 
	if ss.output == inputName:
	    l.append(ss)
    return l

# creates the signature of the function corresponding to a codel
def codel_signature(codel):
  proto = codel.name + "_codel(";
  for type in codel.inTypes:
    idstype = comp.typeFromIdsName(type);
    proto = proto + idstype.toCType() + " *in_" + type + ", ";
  for type in codel.outTypes:
    idstype = comp.typeFromIdsName(type);
    proto = proto + idstype.toCType() + " *out_" + type + ", ";
  proto = proto + "int *report)"
  return proto

def nbExecService():
    count = 0
    for s in comp.servicesMap():
	if s.data().type != ServiceType.Control:
	    count += 1
    return count

# other vars
nbServices = len(comp.servicesMap())
abortRequestNum = nbServices + 1;
internalDataType = comp.IDSType.toCType(True)
periodicFlag = isPeriodic()
