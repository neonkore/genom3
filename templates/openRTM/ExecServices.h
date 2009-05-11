
#ifndef <!upperCompName!>_EXECSERVICES_H
#define <!upperCompName!>_EXECSERVICES_H

class <!capCompName!>ControlData;

<?
for s in comp.servicesMap():
  service = s.data()
  if service.type == ServiceType.Control:
    continue

  statusStr = ""
  first = True
  for c in service.codels():
    if first:
      first = False
    else:
      statusStr += ", "
    statusStr += upper(service.name) + "_" + upper(c.key())
  
    inputStr = ""
    first = True
    for s in service.inputs():
      if first:
	first = False
      else:
	inputStr += ", "
      t = comp.typeFromIdsName(s)
      inputStr += MapTypeToCpp(t) + " " + s
  ?>
class <!service.name!>Service {
  public:
//     enum Status { <!statusStr!>}

    <!service.name!>Service(<!capCompName!>ControlData *data, <!inputStr!>);

    bool step();
    demoState result() { return out_state; }

<?
  for c in service.codels():
    if c.key() == "control":
      continue
    print "    int " + c.key() + "();" 
  ?>

  private:
    int m_id;
    Status m_status;
<?
  for s in service.inputs():
    t = comp.typeFromIdsName(s)
    print "    " + MapTypeToCpp(t) + " in_" + s + ";"    
  if len(service.output) > 0:
    t = comp.typeFromIdsName(service.output)
    print "    " + MapTypeToCpp(t) + " out_" + service.output + ";"
  ?>

    <!capCompName!>ControlData *m_data, 
}; 

<?
?>

#endif
