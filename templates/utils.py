# -*- coding: utf-8 -*-
# This file groups some functions used by multiple templates

comp = Config.getInstance().component()

def is_dynamic(t):
  """ Checks whether the type t is dynamic (ie contains a sequence type)"""
  if t.kind() == IdlType.Sequence:
    return True
  elif t.kind() == IdlType.Named or t.kind() == IdlType.Typedef:
    return is_dynamic(t.unalias())
  elif t.kind() == IdlType.Struct:
    s = t.asStructType()
    for m in s.members():
      if is_dynamic(m.data):
	return True
    return False
  else:
    return False

def is_dynamic_port(port):
  """ Checks whether the port type is dynamic (ie contains a sequence type)"""
  return is_dynamic(port.idlType)

def dynamic_members(t, name, recursive = False):
  """ Returns a list of all sequences inside the type t. It returns 
  pairs containing the type and the full identifier of the member.
  The recursive parameter indicates whether to include nested sequences or not.
  name is the name of the type instance."""
  if t.kind() == IdlType.Sequence:
    if recursive:
      s = t.asSequenceType()
      l = [(t, name)]
      l.extend(dynamic_members(s.seqType(), name + ".data", True))
      return l
    else:
      return [(t, name)]
  elif t.kind() == IdlType.Named or t.kind() == IdlType.Typedef:
    return dynamic_members(t.unalias(), name, recursive)
  elif t.kind() == IdlType.Struct:
    s = t.asStructType()
    l = []
    for m in s.members():
      l.extend(dynamic_members(m.data, name + "." + m.key, recursive))
    return l
  else:
    return []

def to_identifier(n):
  """ Transforms an variable name (containg dots, arrays subscripts and pointers)
  into a identifier suitable for use in the program."""
  res = n.replace(".", "_")
  res = res.replace("[", "")
  res = res.replace("]", "")
  res = res.replace("(*", "")
  res = res.replace(")", "")
  res = res.replace("->", "")
  return res

def counter_name(n):
  """ Returns a counter name that uniquely identifies the var n."""
  return to_identifier(n) + "_counter"

def length_var(n):
  """ Returns a length var name that uniquely identifies the var n."""
  return to_identifier(n) + "_length"

def is_empty_var(n):
  """ Returns a name for a special var that uniquely identifies the var n."""
  return to_identifier(n) + "_is_empty"

def find_init_service():
  """ Try to find an init service. Returns the Service object and its index. """
  i=-1
  for s in comp.servicesMap():
    i += 1
    if s.data().type == Service.Init:
      return s.data(), i
  return 0,-1

initService,initServiceNb = find_init_service()

def flat_struct(t, name, separator = "_", defValue = None, docStruct = None):
    """ Creates a flat list of the structure of a type. The list is composed
    of pairs containing the member type and identifier."""
    if t.kind() == IdlType.Named:
	n = t.asNamedType()
	return flat_struct(n.type(), name, separator, defValue, docStruct)
    elif t.kind() == IdlType.Struct:
	s = t.asStructType()
	l = []
	if defValue is None: 
	  if docStruct is None:
	    for m in s.members():
	      l.extend(flat_struct(m.data, name + separator + m.key, separator, None, None))
	  else:
	      for (m, d) in map(None, s.members(), docStruct.members):
		l.extend(flat_struct(m.data, name + separator + m.key, separator, None, d))
	else:
	  if docStruct is None:
	    for (m, value) in map(None, s.members(), defValue.members()):
	      l.extend(flat_struct(m.data, name + separator + m.key, separator, value, None))
	  else:
	    for (m, value, d) in map(None, s.members(), defValue.members(), docStruct.members):
	      l.extend(flat_struct(m.data, name + separator + m.key, separator, value, d))
	return l
    else:
	return [(t, name, defValue, docStruct)]  

def input_list(service):
  """ Creates a flat list of all the service inputs."""
  res = []
  for i in service.inputs():
    res.extend(flat_struct(input_type(i), i.identifier, '.'))
  return res

def input_type(i):
  """ Returns the idl type of a ServiceInput. """
  if i.kind == ServiceInput.IDSMember:
    return comp.typeFromIdsName(i.identifier)
  else:
    return i.type

# codel related functions
def pointer_to(t):
  """ Pointer to an IdlType. """
  s = MapTypeToC(t,True)
  if t.kind() == IdlType.String:
    return s
  else:
    return s+"*"

def address_of(t, s):
 """ Address of an IdlType. """
 if t.kind() == IdlType.String:
   return s
 else:
    return "&" + s

def real_codel_signature(codel, service=None):
  """ Returns the full prototype of a codel. service arg is
  None only for task-related codels (init/end). """
  proto = ""
  if service is not None:
    for s in service.inputs():
	idstype = input_type(s);
	proto += "const " + pointer_to(idstype) + " in_" + s.identifier + ", ";
    if service.output.identifier:
	idstype = input_type(service.output);
	proto += pointer_to(idstype) + " out_" + service.output.identifier + ", "; 

  for type in codel.inTypes:
    idstype = comp.typeFromIdsName(type);
    proto += "const " + pointer_to(idstype) + " in_" + type + ", ";
  for type in codel.outTypes:
    idstype = comp.typeFromIdsName(type);
    proto += pointer_to(idstype) + " out_" + type + ", ";
  for port in codel.outPorts:
    p = comp.port(port)
    if p is not None:
      proto += pointer_to(p.idlType) + " outport_" + port + ", "; 
    else:
      proto += port + ", "
  for port in codel.inPorts:
    p = comp.port(port)
    if p is not None:
	proto += pointer_to(p.idlType) + " inport_" + port + ", "; 
    else:
	proto += port + ", "
  proto = "return_t " + codel.name + "(" + proto[:-2] + ")"
  return proto

def size_codel_signature(port):
  """ Returns the full prototype of the size codel corresponding to the port. """
  sizeCodelArgs = ""
  for x in dynamic_members(port.idlType, port.name + "_outport", True):
    sizeCodelArgs += "size_t *" + length_var(x[1]) + ", "

  for s in port.sizeCodel.inTypes:
    idstype = comp.typeFromIdsName(s);
    sizeCodelArgs += pointer_to(idstype) + " in_" + s + ", "
  for s in port.sizeCodel.outTypes:
    idstype = comp.typeFromIdsName(s);
    sizeCodelArgs += pointer_to(idstype) + " out_" + s + ", "
  for s in port.sizeCodel.inPorts:
    p = comp.port(s)
    sizeCodelArgs += pointer_to(port.idlType) + " " + s + "_inport, "
  for s in port.sizeCodel.outPorts:
    p = comp.port(s)
    sizeCodelArgs += pointer_to(port.idlType) + " " + s + "_outport, "

  return "return_t " + port.sizeCodel.name + "(" + sizeCodelArgs[:-2] + ")"


# create a list of outports and inports
outports = []
inports = []
for p in comp.portsMap():
    if p.data().type == Port.Outgoing:
	outports.append(p.data())
    else:
	inports.append(p.data())

def create_error_list():
  """ Create a set of all errors defined in the component."""
  l = []
  for s in comp.servicesMap():
    service = s.data()
    for e in service.errorMessages():
	l.append(e)
  for t in comp.tasksMap():
    for e in t.data().errorMessages():
	l.append(e)
  return set(l)
errorList = create_error_list();

class PythonSimpleVisitor(TypeVisitor):
  def visitTypedefType(self, t):
    t.aliasType().accept(self)

  def visitNamedType(self, n):
    n.type().accept(self)

  def visitStructType(self, s):
    for m in s.members():
      m.data.accept(self)

  def visitArrayType(self, a):
    a.type().accept(self)

