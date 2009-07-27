<?
def inputFormat(type, name, value, serviceInput = None):
  str = "    lappend format [list "

  defValue = ""
  if type.kind() == IdlKind.Short or type.kind() == IdlKind.Char:
      str += "short"
      defValue = "0"
  elif type.kind() == IdlKind.Long:
      str += "int"
      defValue = "0"
  elif type.kind() == IdlKind.LongLong:
      str += "wide"
      defValue = "0"
  elif type.kind() == IdlKind.String: 
      str += "string"
      defValue = "\"\""
  elif type.kind() == IdlKind.Float: 
      str += "float"
      defValue = "0.0"
  elif type.kind() == IdlKind.Double: 
      str += "double"
      defValue = "0.0"
  elif type.kind() == IdlKind.Enum: 
      defValue = "\"\""
      str += "{"
      for en in type.asEnumType().enumerators():
	str += "\"" + en + "\" "
      str += "}"

  if not value is None:
    defValue = value.toString()

  if serviceInput is None:
    doc = name
  else:
#    defValue = serviceInput.defaultValue.toString()
    doc = serviceInput.doc

  if not defValue.startswith("\""):
    defValue = "\"" + defValue + "\"" 

  str += " \"" + doc + "\" " + defValue + " ]"
  return str

def outputFormat(type, name):
  if type is None:
    return ""
  if type.kind() == IdlKind.String:
    str = "{1} " # dimension
  else:
    str = "{} "

  if type.kind() == IdlKind.Struct:
    str += "{ " + name + " "
    for m in type.asStructType().members():
      str += outputFormat(m.data, m.key) + " "
    str += "}"
  else:
    str += name
  return str
?>

#
# Copyright (c) 1999-2003 LAAS/CNRS
# All rights reserved.
#
# Redistribution  and  use in source   and binary forms,  with or without
# modification, are permitted provided that  the following conditions are
# met:
#
#   1. Redistributions  of  source code must  retain  the above copyright
#      notice, this list of conditions and the following disclaimer.
#   2. Redistributions in binary form must  reproduce the above copyright
#      notice,  this list of  conditions and  the following disclaimer in
#      the  documentation   and/or  other  materials   provided with  the
#      distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE  AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY  EXPRESS OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES   OF MERCHANTABILITY AND  FITNESS  FOR  A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO  EVENT SHALL THE AUTHOR OR  CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT,  INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING,  BUT  NOT LIMITED TO, PROCUREMENT  OF
# SUBSTITUTE  GOODS OR SERVICES;  LOSS   OF  USE,  DATA, OR PROFITS;   OR
# BUSINESS  INTERRUPTION) HOWEVER CAUSED AND  ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE  USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

#
# This file was automagically generated.
# Manual editing not recommended. Changes will be overridden.
#

if { [lsearch [interp hidden] <!comp.name()!>Install] >= 0 } {
    interp expose {} <!comp.name()!>Install
}

<?
serviceList = ""
for name, service in servicesDict.iteritems():
  serviceList += "\"" + name + "\" "
for port in outports:
  serviceList += "\"" + port.name + "PosterRead\" " 
?>
proc ::<!comp.name()!>Install { name } {
    foreach rqst { <!serviceList!> } {
	interp alias {} ${name}::$rqst {} interp invoke {} <!comp.name()!>$rqst $name
    }
}

interp hide {} <!comp.name()!>Install

<?
for name, service in servicesDict.iteritems():
  serviceInfo = services_info_dict[name]
  if serviceInfo.outputFlag:
    outputFormatStr = outputFormat(serviceInfo.outputType, serviceInfo.outputName)
  else:
    outputFormatStr = ""
  ?>
# <!service.name!> ----------------------------------------------------------

if { [lsearch [interp hidden] <!comp.name()!><!service.name!>] >= 0 } {
    interp expose {} <!comp.name()!><!service.name!>
}
if { [lsearch [interp hidden] <!comp.name()!><!service.name!>FormatOutput] >= 0 } {
    interp expose {} <!comp.name()!><!service.name!>FormatOutput
}

proc ::<!comp.name()!><!service.name!> { name args } {
    set format {}
<?
  if serviceInfo.inputFlag:
    for i in service.inputs():
      flat = flatStruct(inputType(i), i.identifier, ".", i.defaultValue)
      if len(flat) == 1:
	for x in flat:
	  print inputFormat(x[0], x[1], x[2], i)
  ?>
    return [ ::cs::rqstSend "${name}" "<!service.name!>" \
	"<!service.doc!>" \
	"service.usage" \
	<!len(serviceInfo.inputFlatList)!> \
	$format \
	$args ]
}


proc ::<!comp.name()!><!service.name!>FormatOutput { list } {
    set format { {} status <!outputFormatStr!>}

    set out ""
    interp invokehidden {} rqstFormatOuput $format out list ""
    return [join $out "\n"]
}

interp hide {} <!comp.name()!><!service.name!>
interp hide {} <!comp.name()!><!service.name!>FormatOutput

<?

for port in outports:
  ?>
# <!port.name!> ----------------------------------------------------------

if { [lsearch [interp hidden] <!comp.name()!><!port.name!>PosterRead] >= 0 } {
    interp expose {} <!comp.name()!><!port.name!>PosterRead
}
if { [lsearch [interp hidden] <!comp.name()!><!port.name!>PosterFormatOutput] >= 0 } {
    interp expose {} <!comp.name()!><!port.name!>PosterFormatOutput
}

proc ::<!comp.name()!><!port.name!>PosterRead { name args } {

    return [ ::cs::posterRead "${name}" "<!port.name!>" \
	"port.doc" \
	$args ]
}


proc ::<!comp.name()!><!port.name!>PosterFormatOutput { list } {
    set format { {} status <!outputFormat(port.idlType.unalias(), port.name)!>}

    set out ""
    interp invokehidden {} rqstFormatOuput $format out list ""
    return [join $out "\n"]
}

interp hide {} <!comp.name()!><!port.name!>PosterRead
interp hide {} <!comp.name()!><!port.name!>PosterFormatOutput

<?
?>
