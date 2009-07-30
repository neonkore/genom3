/*
 * Copyright (c) 2009 LAAS/CNRS
 * All rights reserved.
 *
 * Redistribution and use  in source  and binary  forms,  with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   1. Redistributions of  source  code must retain the  above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice,  this list of  conditions and the following disclaimer in
 *      the  documentation  and/or  other   materials provided  with  the
 *      distribution.
 *
 * THIS  SOFTWARE IS PROVIDED BY  THE  COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND  ANY  EXPRESS OR IMPLIED  WARRANTIES,  INCLUDING,  BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES  OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR  PURPOSE ARE DISCLAIMED. IN  NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR      CONTRIBUTORS  BE LIABLE FOR   ANY    DIRECT, INDIRECT,
 * INCIDENTAL,  SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE   OF THIS SOFTWARE, EVEN   IF ADVISED OF   THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

#ifndef G3NOM_AST_H
#define G3NOM_AST_H

#include <string>
#include <vector>
#include <map>

#include "idltype.h"
#include "idlvalues.h"

/** \short Base namespace for all classes related to G3nom.*/
namespace G3nom
{
  
/** 
* A codel represents a user function called during a service execution.
* \li #inTypes (resp. #outTypes) are names of IDS members, that the codel can read (resp. read and write to).
* \li #inPorts (resp. #outPorts) are names of inports the codel can read (resp. write to).
* \short Class representing a codel
*/
class Codel
{
	public:
		typedef boost::shared_ptr<Codel> Ptr;
		typedef std::map<std::string, Ptr> Map;

		Codel() {}
		Codel(const std::string &id)
		: m_name(id)
		{}

		void debug();

		void addInType(const std::string &t);
		void addOutType(const std::string &t);
		void addInPort(const std::string &t);
		void addOutPort(const std::string &t);

		void setName(const std::string &n) { m_name = n; }
		std::string name() { return m_name; }

		std::vector<std::string> inTypes;
		std::vector<std::string> outTypes;
		/** List of in ports used by this codel*/
		std::vector<std::string> inPorts;
		std::vector<std::string> outPorts;
		std::vector<std::string> nextCodels;

	private:
			std::string m_name;
};

class PortEvent;
class ServiceEvent;
class NamedEvent;

/** There are 3 types of events: Service events, Ports events and Named events
* which correspond to external events.
* \short An internal event
*/
class Event 
{
	public:
		typedef boost::shared_ptr<Event> Ptr;
		typedef std::map<std::string, Ptr> Map;
		typedef std::map<Ptr, std::string> RevMap;
		typedef std::vector<Ptr> Vect;

		enum Kind { NamedEv, PortEv, ServiceEv};

		Event(Kind k) : m_kind(k) {}
		virtual ~Event() {}

		/** \return The string representing the event (the same as in the .gnm file) */
		virtual std::string identifier() const = 0;
		Kind kind() { return m_kind; }

		PortEvent* asPortEvent();
		NamedEvent* asNamedEvent();
		ServiceEvent* asServiceEvent();

	protected:
		Kind m_kind;
};

typedef std::vector< std::pair<Event::Ptr, std::string> > EventCodelVect;

/** 
* \short A named event (triggered externally)*/
class NamedEvent : public Event 
{
	public:
		NamedEvent(const std::string &id, Event::Ptr event = Event::Ptr()) 
		: Event(Event::NamedEv), m_identifier(id), m_event(event) 
		{}

		virtual std::string identifier() const { return m_identifier; }
		Event::Ptr aliasEvent() const { return m_event; }

	protected:
		std::string m_identifier;
		Event::Ptr m_event;
};

/** 
* \short A Port event*/
class PortEvent : public Event
{
	public:
		enum Kind { OnUpdate, OnWrite, OnRead, OnInitialize };

		PortEvent(const std::string &portName, Kind k) 
		: Event(Event::PortEv), m_port(portName), m_portKind(k)
		{}

		virtual std::string identifier() const { return m_port + "." + kindAsString(); }
		std::string portName() const { return m_port; }
		std::string kindAsString() const;

	protected:
		std::string m_port;
		Kind m_portKind;
};


/** 
* \short A Service event*/
class ServiceEvent : public Event
{
	public:
		enum Kind { OnCalled, OnStart, OnEnd, OnInter, OnCodel };

		ServiceEvent(const std::string &serviceName, Kind k) 
		: Event(Event::ServiceEv), m_service(serviceName), m_serviceKind(k)
		{}
		ServiceEvent(const std::string &serviceName, const std::string codelName)
		: Event(Event::ServiceEv), m_service(serviceName), m_codelName(codelName), m_serviceKind(OnCodel)
		{}

		/** Internal service events (ie used inside the same service) do not have
		* the service name in their identifier. */
		virtual std::string identifier() const;
		std::string serviceName() const { return m_service; }
		std::string kindAsString() const;

	protected:
		std::string m_service;
		std::string m_codelName;
		Kind m_serviceKind;
};

/** \short Interface representing objects with properties */
class IObjectProperties {
	public:
		const Idl::Literal::Map &properties() const { return m_properties; }
		void addProperty(const std::string &name, const Idl::Literal &value);
		Idl::Literal& property(std::string s);

		void debugProperties() const;

	protected:
		Idl::Literal::Map m_properties;
};

/**
*  The #sizeCodel is a regular codel (ie it can have tha same arguments as a regular codel). It will
* be called before a dynamic port (ie a port containing a sequence) is initialized. A int* parameter
* is added to the codel args for each sequence in the port type, in which the user writes the desired size for 
* the corresponding sequence.
* \short A data port
*/
class Port : public IObjectProperties
{
	public:
		enum Type { Incoming, Outgoing};
		typedef boost::shared_ptr<Port> Ptr;
		typedef std::map<std::string, Ptr> Map;

		Port() {}
		Port(std::string name, Idl::IdlType::Ptr idlType, bool incoming)
				: name(name), type(incoming ? Incoming : Outgoing), idlType(idlType) {}

		void debug();

		std::string name;
		Type type;
		Idl::IdlType::Ptr idlType;
		Codel::Ptr sizeCodel;
};

/** \short An execution task
* 
*/
class Task : public IObjectProperties
{
	public:
		typedef boost::shared_ptr<Task> Ptr;
		typedef std::map<std::string, Ptr> Map;

		Task() {}
		Task(const std::string id)
		: name(id), period(0)
		{}

		void debug();

		void addCodel(const std::string &name, Codel::Ptr c);
		Codel::Ptr codel(const std::string &name);
		bool hasCodel(const std::string &name);

		std::vector<std::string>& errorMessages() { return m_errorMessages; }
		void addErrorMessage(const std::string &s);

		std::string name;
		int priority;
		int period; // in ms
		int delay; //in ms
		int stackSize; // in kbytes

	private:
		Codel::Map m_codels;
		std::vector<std::string> m_errorMessages;
		//std::string connectedPort;
};

class InputDoc {
	public:
		typedef std::vector<InputDoc> Vector;
		InputDoc() {}
		InputDoc(std::string s) : doc(s) {}

		bool operator==(const InputDoc& rhs) { return doc == rhs.doc; }

		std::string doc;
		Vector members;
};

/** 
* It can either be an IDS member (eg IDS:speedRef) in which case #type is null, or 
* defined by its type (eg double d). In both cases, it can have a default value.
* \short A service's input or output parameter
*/
struct ServiceInput {
	typedef std::vector<ServiceInput> Vect;
	enum Kind { IDSMember, Type};
	
	ServiceInput() : kind(IDSMember) {}

	/** It is necessary to overload this operator because boost.python
	* expects types stored in a vect to be comparable with operator==()
	*/
	bool operator==(const ServiceInput &rhs); // necessary for boost.python

	Kind kind;
	std::string identifier;
	Idl::IdlType::Ptr type;
	Idl::Literal defaultValue;
	InputDoc doc;
};

/** \short A service*/
class Service : public IObjectProperties
{
	public:
		typedef boost::shared_ptr<Service> Ptr;
		typedef std::map<std::string, Ptr> Map;
		enum Type { Control, Exec, Init };

		Service() {}
		Service(const std::string &id) 
		: name(id) {}

		void debug();

// 		void addInput(const std::string &s, Idl::IdlType::Ptr t, const Idl::Literal &defaultValue = Idl::Literal());
		void addInput(const ServiceInput &i);
		ServiceInput::Vect& inputs() { return m_inputs; }
		Idl::Literal inputDefaultArg(const std::string &n); 

		std::vector<std::string>& errorMessages() { return m_errorMessages; }
		void addErrorMessage(const std::string &s);

		void addCodel(const std::string &name, Codel::Ptr c);
		Codel::Map& codels() { return m_codels; }
		Codel::Ptr codel(const std::string &name);
		bool hasCodel(const std::string &name);

		void addIncompatibleService(const std::string &name);
		std::vector<std::string> & incompatibleServices() { return m_incompatibleServices; }

		void addEvent(Event::Ptr event, const std::string &target);
		const Event::RevMap & events() const { return m_events; } 

		std::string name;
		Type type;
		std::string doc;
		std::string taskName;
		ServiceInput output;

	private:
		Codel::Map m_codels;
		ServiceInput::Vect m_inputs;
		std::vector<std::string> m_incompatibleServices;
		std::vector<std::string> m_errorMessages;
		Event::RevMap m_events;
};

/** \short A component
* 
*/
class Component : public IObjectProperties
{
	public:
		Component();
		~Component();

		void debug();

		void setName(const std::string &n) { m_name = n; }
		std::string name() { return m_name; }

		void addTask(Task::Ptr task);
		void addService(Service::Ptr task);
		void addPort(Port::Ptr port);
		void addEvent(Event::Ptr ev);

		Task::Map& tasksMap();
		std::vector<std::string> tasksList();
		Task::Ptr task(const std::string &name);
		int taskIndex(const std::string &name) const;
		
		Service::Map& servicesMap();
		/** \return the service named \a name*/
		Service::Ptr service(const std::string &name);
		/** \return the index of the service named \a name inside the services map*/
		int serviceIndex(const std::string &name) const;

		Port::Ptr port(const std::string &name);
		Port::Map& portsMap() { return ports; }
		/** \return the index of the port named \a name inside the ports map*/
		int portIndex(const std::string &name) const;

		Event::Ptr event(const std::string &ev);
		std::vector<std::string> eventsForPort(const std::string &name);
		std::vector<std::string> eventsForService(const std::string &name);
		Event::Map& eventsMap() { return events; }

		void addConstValue(const Idl::ConstValue &val);
		void addType(Idl::IdlType::Ptr type);
		void addNativeTypeInclude(const std::string &s);
		const std::vector<std::string>& nativeTypesIncludes() const { return m_typesIncludes; }

		Idl::IdlType::Vector& typesVect() { return m_types; }
		Idl::ConstValue::Map& valuesMap() { return m_constValues; }

		void addImportedComponent(const std::string &s);
		std::vector<std::string>& importedComponents() { return m_importedComponents; }

		/** This function is used to search a type by its identifier.
		* It returns either a new NamedType pointing to the found type, 
		* or an invalid IdlType::Ptr if no such type is found.
		*/
		Idl::IdlType::Ptr typeFromName(const std::string &name);
		/** \return the type of the IDS element \a name */
		Idl::IdlType::Ptr typeFromIdsName(const std::string &name);

		std::string pluginLanguage;
		std::string version;
		Idl::IdlType::Ptr IDSType;
		int uniqueId;

	private:
		std::string m_name;

		Task::Map tasks;
		Service::Map services;
		Port::Map ports;
		Event::Map events;
		Idl::IdlType::Vector m_types;
		std::vector<std::string> m_typesIncludes;
		Idl::ConstValue::Map m_constValues;
		std::vector<std::string> m_importedComponents;
};

}

#endif // G3NOM_AST_H
// kate: indent-mode cstyle; replace-tabs off; tab-width 4; 
