#include "Events.hpp"

#include <iostream>

using namespace GenomYarp;

/****** EventSender *************/

void EventReceiver::addEvent(const std::string sender, const std::string &ev)
{
  if(sender.empty())
    m_eventsStack.push_back(ev);
  else
    m_eventsStack.push_back(sender + "." + ev);
}

std::string EventReceiver::takeEvent()
{
  std::string s = m_eventsStack.front();
  m_eventsStack.pop_front();
  return s;
}

bool EventReceiver::isEmpty()
{
  return m_eventsStack.empty();
}

/****** EventSender *************/

void EventSender::sendEvent(const std::string &ev)
{
  if(m_receivers.find(ev) == m_receivers.end()) {
//     std::cout << "No receiver for this event: " << m_name << "." << ev << std::endl;
    return;
  }

  EventReceiver::List::const_iterator it = m_receivers[ev].begin();
  for(; it != m_receivers[ev].end(); ++it)
    (*it)->addEvent(m_name, ev);
}

void EventSender::registerReceiver(const std::string &event, EventReceiver *r)
{
  std::cout << "Registered receiver: " << m_name << "." << event << std::endl;
  m_receivers[event].push_back(r);
}

void EventSender::unregisterReceiver(const std::string &event, EventReceiver *r)
{
   if(m_receivers.find(event) == m_receivers.end()) {
     std::cout << "Unregistered event: " << m_name << "." << event << std::endl;
    return;
  } 

  m_receivers[event].remove(r);

}

/****** EventOutPort *************/

void EventOutPort::addEvent(const std::string sender, const std::string &ev)
{
  yarp::os::Bottle &b = prepare();
  b.addString(m_eventsAlias[sender + "." + ev].c_str());

  writeStrict();
}

void EventOutPort::addAlias(const std::string &ev, const std::string &alias)
{
  m_eventsAlias[ev] = alias;
}

/****** EventInPort *************/

EventInPort::EventInPort()
{
  useCallback();
}

void EventInPort::onRead(yarp::os::Bottle &b)
{
  std::string s = b.get(0).asString().c_str();
  sendEvent(s);
}

/****** EventProxy *************/

void EventProxy::open(const std::string &name)
{
  std::string in_name = name + "/In";
  std::string out_name = name + "/Out";
  m_inPort.open(in_name.c_str());
  m_outPort.open(out_name.c_str());
  m_name = name;
}

void EventProxy::connectPorts(const std::string &in, const std::string &out)
{
  std::string in_name = m_name + "/In";
  std::string out_name = m_name + "/Out";
  yarp::os::Network::connect(in.c_str(), in_name.c_str());
  yarp::os::Network::connect(out_name.c_str(), out.c_str());
}

void EventProxy::connectEvents(const std::string &in, const std::string &out)
{
  m_eventsAlias[in] = out;
}

void EventProxy::onRead(yarp::os::Bottle& b)
{
  std::string s = b.get(0).asString().c_str();
  
  yarp::os::Bottle &out = m_outPort.prepare();
  out.addString(m_eventsAlias[s].c_str());
  m_outPort.writeStrict();
}
