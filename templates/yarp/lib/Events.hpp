#ifndef YARP_EVENTS_HPP
#define YARP_EVENTS_HPP

#include <string>
#include <list>
#include <map>

#include <yarp/os/all.h>

namespace GenomYarp {

typedef yarp::os::BufferedPort<yarp::os::Bottle> EventPort;

class EventReceiver 
{
  public:
    typedef std::list<EventReceiver*> List;

    EventReceiver() {}
    ~EventReceiver() {}

    virtual void addEvent(const std::string sender, const std::string &ev);

    std::string takeEvent();
    bool isEmpty();

  private:
    std::list<std::string> m_eventsStack;
};

class EventSender 
{
  public: 
    EventSender() {}
    EventSender(const std::string name) : m_name(name) {}
    ~EventSender() {}

    void setName(const std::string &name) { m_name = name; }

    void sendEvent(const std::string &ev);
    void registerReceiver(const std::string &event, EventReceiver *r);
    void unregisterReceiver(const std::string &event, EventReceiver *r);

  private:
    std::string m_name;
    std::map<std::string, EventReceiver::List> m_receivers;
    std::map<std::string, std::string> m_eventsAlias;
};

class EventInPort: public EventSender, public EventPort 
{
  public:
    EventInPort();
    ~EventInPort();

    virtual void onRead(yarp::os::Bottle &t);
};

class EventOutPort : public EventReceiver, public EventPort 
{
  public:
    EventOutPort() {}
    ~EventOutPort() {}

    /** Forwards the internal event to the event port.*/
    virtual void addEvent(const std::string sender, const std::string &ev);

    void addAlias(const std::string &ev, const std::string &alias);

  private:
    std::map<std::string, std::string> m_eventsAlias;
};

class EventProxy : public yarp::os::TypedReaderCallback<yarp::os::Bottle>
{
  public:
    EventProxy();
    ~EventProxy();

    void open(const std::string &name);
    virtual void onRead(yarp::os::Bottle &t);

    void connectPorts(const std::string &in, const std::string &out);
    void connectEvents(const std::string &in, const std::string &out);

  private:
    EventPort m_inPort;
    EventPort m_outPort;
    std::map<std::string, std::string> m_eventsAlias;
    std::string m_name;
};

}

#endif
