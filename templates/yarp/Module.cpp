#include "module.hpp"

<!comp.name()!>Module::<!comp.name()!>Module()
: m_data(0)
{
    setName("<!comp.name()!>");
} 

bool open(Searchable& config)
{
    // initialize data
    m_data = new <!comp.name()!>ControlData();

    // open data ports
<?
for p in outports: ?>
    m_data-><!p.name!>_outport.open("/<!comp.name()!>/OutPorts/<!p.name!>");
<?
for p in inports: ?>
    m_data-><!p.name!>_inport.open("/<!comp.name()!>/InPorts/<!p.name!>");
<?
?>
    m_data->request_port.open("/<!comp.name()!>/Services/Control");

    // create exec task and start them

    return true;
}

bool interruptModule()
{
<?
for p in portsMap: ?>
    <!p.key()!>.interrupt();
<?
?>
    return true;
}

bool updateModule()
{
    return true;
}

