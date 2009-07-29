# -*- coding: utf-8 -*-
from string import upper

IDSType = comp.IDSType
servicesMap = comp.servicesMap()
capCompName = comp.name().capitalize()
upperCompName = upper(comp.name())

def needs_conversion(t):
  """ Returns whether it is necessary to convert from the CORBA type to the type
  used by codels """
  if t.isNative():
    return True
  if t.kind() == IdlType.Long:
    return True
  elif t.kind() == IdlType.Struct:
    return True
  elif t.kind() == IdlType.Enum:
    return True
  elif t.kind() == IdlType.Typedef or t.kind() == IdlType.Named:
    return needs_conversion(t.unalias())
  else:
    return False

def needs_conversion_fun(t):
  """ Returns whether it is necessary to used a generated function to convert
  from CORBA to codel type. True for structured types"""
  if t.kind() == IdlType.Struct:
    return True
  elif t.kind() == IdlType.Enum:
    return True
  elif t.kind() == IdlType.Typedef or t.kind() == IdlType.Named:
    return needs_conversion_fun(t.unalias())
  else: 
    return False

def service_idl_signature(service):
    """ Returns the idl function corresponding to a service."""
    # find the service output type
    if service.type != Service.Control:
      outputType = BaseType.longType
    elif service.output.identifier:
      outputType = input_type(service.output)
    else:
      outputType = BaseType.voidType

    # then create the args list
    args = ""
    for i in service.inputs():
	t = input_type(i)
	args += "in " + MapTypeToIdl(t) + " " + i.identifier + ", "
 
    exceptions = ""
    for e in service.errorMessages():
      exceptions += e + ", "
    if exceptions:
      exceptions = " raises(" + exceptions[:-2] + ") "
      
    return MapTypeToIdl(outputType) + " " + service.name + "(" + args[:-2] + ")" + exceptions + ";"

def cpp_arg(t,name):
  cppType = MapTypeToCorbaCpp(t)
  if t.kind() == IdlType.Struct:  
    return "const " + cppType + " &" + name
  elif t.kind() == IdlType.Named or t.kind() == IdlType.Typedef:
    if t.unalias().kind() == IdlType.Struct:
      return "const " + cppType + " &" + name
    else:
      return cppType + " " + name
  elif t.kind() == IdlType.String:
    return "const " + cppType + " " + name
  else:
    return cppType + " " + name

def service_cpp_args(service, className=""):
    """ Create the args list for the service constructor (for exec services) or functions (control services). """
    args = ""
    for i in service.inputs():
	t = input_type(i)
	args += cpp_arg(t," in_" + i.identifier) + ", "
    return service.name + "(" + args[:-2] + ")"

def service_cpp_signature(service, className=""):
    """ Create the full prototype for a (control) service function."""
    # find the service output type
    if service.type != Service.Control:
      outputType = BaseType.longType
    elif service.output.identifier:
      outputType = input_type(service.output)
    else:
	outputType = BaseType.voidType

    output = MapTypeToCorbaCpp(outputType, True)
    if is_corba_dynamic(outputType):
      output += "*"

    if className:
      return output + " " + className + "::" + service_cpp_args(service, className)
    else:
      return output + " " + service_cpp_args(service, className)

def real_codel_call(codel, data_prefix="", service=None, useCopiedArgs = True):
  """ Creates the string to call the user codel. useCopiedArgs indicates whether to use 
  copied vars created with codelLock."""
  proto = ""
  if service is not None:
    for i in service.inputs():
      t = input_type(i)
      if needs_conversion(t) and useCopiedArgs:
	proto += address_of(input_type(i), i.identifier) + ", "
      else:
	proto += address_of(input_type(i), " in_" + i.identifier) + ", "
    if service.output.identifier:
	proto += " &out_" + service.output.identifier + ", " 

  for type in codel.inTypes:
    t = comp.typeFromIdsName(type)
    proto += address_of(t, data_prefix + type) + ", "
  for type in codel.outTypes:
    t = comp.typeFromIdsName(type)
    proto += address_of(t, data_prefix + type) + ", "
  for port in codel.outPorts:
    p = comp.port(port)
    if is_dynamic(p.idlType):
      proto += "&" + port + "_outport, "
    elif needs_conversion(p.idlType):
      proto += "&" + port + ", "
    else:
      proto += "&" + data_prefix + port + "_data, " 
  for port in codel.inPorts:
    p = comp.port(port)
    if is_dynamic(p.idlType):
      proto += "&" + port + "_inport, "
    elif needs_conversion(p.idlType):
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

def service_outports_set(service):
  """ Returns a set of all the outports used by all the service's codels. """
  portList = []
  for c in service.codels():
    for port in c.data().outPorts:
      portList.append(port)
  return set(portList)

def service_inports_set(service):
  """ Returns a set of all the inports used by all the service's codels. """
  portList = []
  for c in service.codels():
    for port in c.data().inPorts:
      portList.append(port)
  return set(portList)

def start_state_for_service(service):
  if service.hasCodel("start"):
    return upper(service.name) + "_START"
  else:
    return upper(service.name) + "_MAIN"

def output_ports_map():
  """ Returns a map, matching services name with its output type (or an empty string)
  to create the output ports. """
  m = {}
  for s in servicesMap:
    service = s.data()
    if service.type == Service.Control:
      continue
    if not service.output.identifier:
      typeName = ""
    else:
      typeName = MapTypeToIdl(input_type(service.output))
    m[service.name] = typeName
  return m

output_ports_map = output_ports_map()

def codel_needs_lock(codel, service=None):
  """ Returns whether it is necessary to lock the IDS when calling the codel. 0 means no locking,
  1 means lock for read and 2 read for write. """
  if codel.outTypes:
    return 2
  elif not service is None and service.output.identifier and service.output.kind == ServiceInput.IDSMember:
    return 2
  elif codel.inTypes:
    return 1
  else:
    if not service is None:
      for i in service.inputs():
	if i.kind == ServiceInput.IDSMember:
	  return 1
    return 0

def is_corba_dynamic(t):
  """ returns whether the type t is dynamic from CORBA point of view
  (ie contains a string). These types have to be allocated dynamically."""
  if t.kind() == IdlType.String:
    return True
  elif t.kind() == IdlType.Named or t.kind() == IdlType.Typedef:
    return is_corba_dynamic(t.unalias())
  elif t.kind() == IdlType.Struct:
    s = t.asStructType()
    for m in s.members():
      if is_corba_dynamic(m.data):
	return True
    return False
  else:
    return False

def copy_type(t, dest, src):
    """ Copy the CORBA data into the var created for use in the codels.""" 
    if t.kind() == IdlType.Sequence:
      s = t.asSequenceType()
      if is_dynamic(s.seqType()):
	print dest + ".data = new " + MapTypeToCpp(s.seqType(), True) + "[" + src + ".length()];"
	print dest + ".length = " + src + ".length();"

	counter = counter_name(dest)
	print "for(int " + counter + " =0; " + counter + "<" + src + ".length(); ++" + counter + ") {"
	copy_type(s.seqType(), dest + ".data[" + counter + "]", src + "[" + counter + "]")
	print "}"
      else:
	print dest + ".length = " + src + ".length();"
	print dest + ".data = (" + MapTypeToCpp(s.seqType())  + "*) " + src + ".get_buffer();"

    elif t.kind() == IdlType.Array:
      s = t.asArrayType()
      counter = counter_name(dest)
      print "for(int " + counter + "=0; " + counter + " < " + str(s.bounds()[0])  + "; ++" + counter + ") {"
      copy_type(s.type(), dest + "[" + counter + "]", src + "[" + counter + "]")
      print "}"
    elif t.kind() == IdlType.Struct:
      s = t.asStructType()
      for m in s.members():
	copy_type(m.data, dest + "." + m.key, src + "." + m.key)
    elif t.kind() == IdlType.Named or t.kind() == IdlType.Typedef:
      copy_type(t.unalias(), dest, src)
    else:
      copy_type_from_corba(t, src, dest, False)
      #if needs_conversion_fun(t):
	#print "convertFromCorba_" + t.identifier() + "(&" + src + ", &" + dest + ");"
      #else:
	#print dest + " = " + src + ";"

def copy_type_reverse(t, dest, src, useIsEmptyVar = True, parentIsEmpty = False):
    """ Copy data back to the CORBA data structures."""
    if t.kind() == IdlType.Sequence:
      s = t.asSequenceType()
      if not parentIsEmpty or useIsEmptyVar:
	if is_dynamic(s.seqType()):
	  counter = counter_name(src)
	  print "for(int " + counter + "=0; " + counter + " < " + src + ".length; ++" + counter  + ") {"
	  copy_type_reverse(s.seqType(), dest + "[" + counter + "]", src + ".data[" + counter + "]", False)
	  print "}"
	  print "delete[] " + src + ".data;"

    elif t.kind() == IdlType.Array:
      s = t.asArrayType()
      counter = counter_name(src)
      print "for(int " + counter + "=0; " + counter + "<" + str(s.bounds()[0]) + "; ++" + counter + ") {"
      copy_type_reverse(s.type(), dest + "[" + counter + "]", src + "[" + counter + "]", useIsEmptyVar, parentIsEmpty)
      print "}"
    elif t.kind() == IdlType.Struct:
      s = t.asStructType()
      for m in s.members():
	copy_type_reverse(m.data, dest + "." + m.key, src + "." + m.key, useIsEmptyVar, parentIsEmpty)
    elif t.kind() == IdlType.Named or t.kind() == IdlType.Typedef:
      copy_type_reverse(t.unalias(), dest, src, useIsEmptyVar, parentIsEmpty)
    else:
      copy_type_from_corba(t, src, dest, True)
      #if needs_conversion_fun(t):
	#print "convertFromCorbaReverse_" + t.identifier() + "(&" + src + ", &" + dest + ");"
      #else:
	#print dest + " = " + src + ";"

def free_memory(t, src):
    """ free the memory allocated dynamically in codelLock taht won't be needed anymore. """
    if t.kind() == IdlType.Sequence:
      s = t.asSequenceType()
      if is_dynamic(s.seqType()):
	counter = counter_name(src)
	print "for(int " + counter + "=0; " + counter + " < " + src + ".length; ++" + counter  + ") {"
	free_memory(s.seqType(), src + ".data[" + counter + "]")
	print "}"
	print "delete[] " + src + ".data;"

    elif t.kind() == IdlType.Struct:
      s = t.asStructType()
      for m in s.members():
	free_memory(m.data, src + "." + m.key)
    elif t.kind() == IdlType.Named or t.kind() == IdlType.Typedef:
      free_memory(t.unalias(), src)


def allocate_memory(t, dest, scopedName):
    """ Allocate memory (on CORBA side) after the size codel has been called. """
    if t.kind() == IdlType.Sequence:
      s = t.asSequenceType()
      seqType = MapTypeToCorbaCpp(s.seqType(), True)

      print dest + ".replace(" + length_var(scopedName) + ", " + length_var(scopedName) + ", new " + seqType + "[" + length_var(scopedName) + "]);"
      print ""

      if is_dynamic(s.seqType()):
	counter = counter_name(dest)
	print "int " + counter + " = 0;"
	print "for(; " + counter + "<" + dest + ".length(); ++" + counter + ") {"
	allocate_memory(s.seqType(), dest + "[" + counter + "]", scopedName + ".data")
	print "}"
      else:
	print "memset(" + dest + ".get_buffer() , 0, " + length_var(scopedName) + " * sizeof(" + seqType + "));"

    elif t.kind() == IdlType.Struct:
      s = t.asStructType()
      for m in s.members():
	allocate_memory(m.data, dest + "." + m.key, scopedName + "." + m.key)
    elif t.kind() == IdlType.Named or t.kind() == IdlType.Typedef:
      allocate_memory(t.unalias(), dest, scopedName)

def call_size_codel(port):
  """ Create the string to call the size codel."""
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

def copy_codel_args(codel, service):
    """ Copy the codel args to types suitable for the codels."""
    if not service is None:
      for i in service.inputs():
	t = input_type(i)
	if needs_conversion(t):
	  if needs_conversion_fun(t):
	    print MapTypeToCpp(t, True) + " " + i.identifier + ";"
	    print "convertFromCorba_" + t.identifier() + "(&in_" + i.identifier + ", &" + i.identifier + ");"
	  else:
	    print MapTypeToCpp(t, True) + " " + i.identifier + " = in_" + i.identifier + ";"

def codel_lock(codel, service=None):
    """ Prepare to run a codel. Lock the ids if necessary, copy data from CORBA data structures to the structures
    used by the codel, allocated in this function."""
    #for p in codel.inPorts:
      #print "  m_data->" + p + "_inport.wait();"
    for p in codel.outPorts:
      port = comp.port(p)
      if not is_dynamic(port.idlType):
	continue
      seqs = dynamic_members(port.idlType, p + "_data.data")
      newType = MapTypeToCpp(port.idlType, True)
      print "  " + newType + " " + p + "_outport;"

      str = ""
      for x in seqs:  
	str += "m_data->" + x[1] + ".length() == 0 && "
      print "bool " +  p + "_outport_is_empty = " + str[:-3] + ";"

      print "if(" + p + "_outport_is_empty) {"

      call_size_codel(port)

      print "  if(res >= 0) {"
      allocate_memory(port.idlType, "m_data->" + p + "_data.data", p + "_outport")
      print "  }"
      print "}\n\n"

      copy_type(port.idlType, p + "_outport", "m_data->" + p + "_data.data")


    for p in codel.inPorts:
      port = comp.port(p)
      if not is_dynamic(port.idlType):
	continue
      newType = MapTypeToCpp(port.idlType, True)
      print "  " + newType + " " + p + "_inport;"
      copy_type(port.idlType, p + "_inport", "m_data->" + p + "_data.data")


    res = codel_needs_lock(codel, service)
    if res == 2:
      print "  m_data->idsMutex.acquire_write();"
    elif res == 1:
      print "  m_data->idsMutex.acquire_read();"

    for p in codel.outPorts:
      port = comp.port(p)
      if is_dynamic(port.idlType):
	continue
      
      if needs_conversion(port.idlType):
	if needs_conversion_fun(port.idlType):
	  print MapTypeToCpp(port.idlType, True) + " " + port.name + ";"
	  print "convertFromCorba_" + port.idlType.identifier() + "(&m_data->" + port.name + "_data.data, &" + port.name + ");"
	else:
	  print MapTypeToCpp(port.idlType, True) + " " + port.name + " = m_data->" + port.name + "_data.data;"

    for p in codel.inPorts:
      port = comp.port(p)
      if is_dynamic(port.idlType):
	continue
      
      if needs_conversion(port.idlType):
	if needs_conversion_fun(port.idlType):
	  print MapTypeToCpp(port.idlType, True) + " " + port.name + ";"
	  print "convertFromCorba_" + port.idlType.identifier() + "(&m_data->" + port.name + "_data.data, &" + port.name + ");"
	else:
	  print MapTypeToCpp(port.idlType, True) + " " + port.name + " = m_data->" + port.name + "_data.data;"

def copy_codel_args_reverse(codel, service):
    if not service is None:
      for i in service.inputs():
	t = input_type(i)
	if needs_conversion(t):
	  print "//in_" + i.identifier + " = " + i.identifier + ";"

def codel_release(codel, service=None):
    """ Cleanup after a codel call. Release locks, update the CORBA data structures by copying the codel data structures."""
    # update dynamic ports
    for p in codel.outPorts:
      port = comp.port(p)
      if not is_dynamic(port.idlType):
	if needs_conversion(port.idlType):
	  if needs_conversion_fun(port.idlType):
	    print "convertFromCorbaReverse_" + port.idlType.identifier() + "(&" + port.name + ", &m_data->" + port.name + "_data.data);"
	  else:
	    print MapTypeToCpp(port.idlType, True) + " m_data->" + port.name + "_data = " + port.name + ";"
      else:
	copy_type_reverse(port.idlType,  "m_data->" + p + "_data.data", p + "_outport")
	free_memory(port.idlType, p + "_outport")

    for p in codel.outPorts:
      print "m_data->" + port.name + ".write();"

    for p in codel.inPorts:
      port = comp.port(p)
      print "m_data->" + p + ".read();"
      if not is_dynamic(port.idlType):
	if needs_conversion(port.idlType):
	  if needs_conversion_fun(port.idlType):
	    print "convertFromCorbaReverse_" + port.idlType.identifier() + "(&" + port.name + ", &m_data->" + port.name + "_data.data);"
	  else:
	    print "m_data->" + port.name + "_data.data = " + port.name + ";"
      else:
	free_memory(port.idlType, p + "_inport")

    #for p in codel.inPorts:
      #print "  m_data->" + p + "_inport.post();"
    res = codel_needs_lock(codel, service)
    if res:
      print "  m_data->idsMutex.release();"


