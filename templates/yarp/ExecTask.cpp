<?
currentTask = comp.task(currentTaskName)
?>

#include "<!comp.name()!><!currentTaskName!>.hpp"

#include "<!comp.name()!>Module.h"
#include "ExecServices.h"

<!comp.name()!><!currentTaskName!>::<!comp.name()!><!currentTaskName!>(<!comp.name()!>ControlData *data)
<?
if currentTask.period:?>
: RateThread(<!currentTask.period!>), m_data(data)
<?
else:?>
: Thread(), m_data(data)
<?
?>
{
} 

<?
if currentTask.hasCodel("init"):
  ?>
bool <!comp.name()!><!currentTaskName!>::threadInit()
{
  int res = <!codel_call(currentTask.codel("init"))!>;
  return (res >= 0); // error
}
<?
?>

void <!comp.name()!><!currentTaskName!>::run()
{
<?
for s in comp.servicesMap():
  service = s.data()
  if service.type != ServiceType.Exec or service.taskName != currentTaskName:
    continue
  ?>
  for(<!service.name!>Service::List::iterator it = m_data-><!service.name!>Services.begin();
	it != m_data-><!service.name!>Services.end();) {
    if(!(*it)->step()) { // delete the service
<?
  if service.output:
    ?>
      // get the output result
      <!service.name!>OutStruct s;
      s.id = (*it)->id();
      s.data = (*it)->result();
      m_data-><!service.name!>_outport.write(s);
<?
  ?>
      it = m_data-><!service.name!>Services.erase(it);
    } else
      ++it;
  }
<?
?>
}
