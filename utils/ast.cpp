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
#include "ast.h"

#include <iostream>

#include "dumptypes.h"

using namespace std;
using namespace G3nom;
using namespace Idl;

void replaceAll(string &str, const string &find_what, const string &replace_with)
{
	string::size_type pos=0;
	while((pos=str.find(find_what, pos))!=string::npos) {
		str.erase(pos, find_what.length());
		str.insert(pos, replace_with);
		pos+=replace_with.length();
	}
}

/******** Component ***************/

Component::Component()
{}

Component::~Component()
{
}


void Component::debug()
{
	cout << "Component: " << m_name << " v. " << version << endl;
	cout << "\tLanguage (plugin) : " << pluginLanguage << endl;
	cout << "\tIDS Struct: " << endl;
	if (IDSType)
		cout << DumpType::dumpType(IDSType);
	cout << endl;

	cout << endl << "Services: " << services.size() << endl;
	for (Service::Map::const_iterator it = services.begin();
			it != services.end(); ++it) {
		cout << "\t" << it->first << " : " << endl;
		it->second->debug();
		cout << endl;
	}

	cout << endl << "Tasks: " << tasks.size() << endl;
	for (Task::Map::const_iterator it = tasks.begin(); 
			it != tasks.end(); ++it) {
		cout << "\t-- " << it->first << " : " << endl;
		it->second->debug();
		cout << endl;
	}

	cout << endl << "Ports: " << ports.size() << endl;
	for (Port::Map::const_iterator it = ports.begin();
			it != ports.end(); ++it) {
		cout << "\t* " << it->first << " : ";
		it->second->debug();
		cout << endl;
	}

	cout << endl << "Events: " << events.size() << endl;
	for (Event::Map::const_iterator it = events.begin();
			it != events.end(); ++it) {
		cout << "\t* " << it->first;
		Event::Ptr e = it->second->asNamedEvent()->aliasEvent();
		if(e)
			cout << " : " << e->identifier();
		cout << endl;
	}

	cout << endl << "Types: " << m_types.size() << endl;
	for (IdlType::Vector::const_iterator it = m_types.begin();
			it != m_types.end(); ++it) {
		cout << "* ";
		cout << DumpType::dumpType(*it);
		cout << endl;
	}

	cout << endl << "Types Includes: " << m_typesIncludes.size() << " : ";
	for (std::vector<std::string>::const_iterator it = m_typesIncludes.begin();
			it != m_typesIncludes.end(); ++it) {
			cout << *it << ", ";
	}

	cout << endl << "Const Values: " << m_constValues.size() << endl;
	for (ConstValue::Map::iterator it = m_constValues.begin();
			it != m_constValues.end(); ++it) {
		cout << "* ";
		cout << DumpType::dumpConstValue(it->second);
		cout << endl;
	}

	debugProperties();
}

void Component::addTask(Task::Ptr task)
{
	/// \todo throw exception ? return code ?
	if (tasks.find(task->name) != tasks.end())
		cerr << "Warning: already existing task name: " << task->name << endl;
	tasks[task->name] = task;
}

void Component::addService(Service::Ptr s)
{
	if (services.find(s->name) != services.end())
		cerr << "Warning: already existing service name: " << s->name << endl;
	services[s->name] = s;
}

void Component::addPort(Port::Ptr port)
{
	if (ports.find(port->name) != ports.end())
		cerr << "Warning: already existing port name: " << port->name << endl;
	ports[port->name] = port;
}

void Component::addEvent(Event::Ptr ev)
{
	if (events.find(ev->identifier()) != events.end())
		cerr << "Warning: already existing event name: " << ev->identifier() << endl;
	events[ev->identifier()] = ev;
}

void Component::addType(IdlType::Ptr type)
{
	m_types.push_back(type);
}

void Component::addConstValue(const Idl::ConstValue &val)
{
	m_constValues[val.identifier()] = val;
}

Task::Ptr Component::task(const std::string &name)
{
	Task::Map::iterator it = tasks.find(name);
	if (it != tasks.end())
		return it->second;
	return Task::Ptr();
}

Task::Map& Component::tasksMap()
{
	return tasks;
}

Service::Map& Component::servicesMap()
{
	return services;
}

std::vector<std::string> Component::tasksList()
{
	vector<string> vec;

	Task::Map::const_iterator it;
	for (it = tasks.begin(); it != tasks.end(); ++it)
		vec.push_back(it->first);

	return vec;
}

int Component::taskIndex(const std::string &name) const
{
	int i = 0;
	Task::Map::const_iterator it = tasks.begin();
	for(; it != tasks.end(); ++it, ++i) {
		  if(it->first == name)
			  return i;
	}
	return -1;
}

Service::Ptr Component::service(const std::string &name)
{
	Service::Map::iterator it = services.find(name);
	if (it != services.end())
		return it->second;
	return Service::Ptr();
}

int Component::serviceIndex(const std::string &name) const
{
	int i = 0;
	Service::Map::const_iterator it = services.begin();
	for(; it != services.end(); ++it, ++i) {
		  if(it->first == name)
			  return i;
	}
	return -1;
}

Port::Ptr Component::port(const std::string &name)
{
	Port::Map::const_iterator it = ports.find(name);
	if (it != ports.end())
		return it->second;
	return Port::Ptr();
}

int Component::portIndex(const std::string &name) const
{
	int i = 0;
	Port::Map::const_iterator it = ports.begin();
	for(; it != ports.end(); ++it, ++i) {
		  if(it->first == name)
			  return i;
	}
	return -1;
}

IdlType::Ptr Component::typeFromName(const std::string &name)
{
// 	cout << "Searching type " << name << endl;
	IdlType::Ptr res;
	IdlType::Vector::const_iterator it4;
	for (it4 = m_types.begin(); it4 != m_types.end(); ++it4) {
		switch ((*it4)->kind()) {
			case IdlType::Struct: {
					StructType *s = (*it4)->asType<StructType>();
					if (s->identifier() == name)
						res = *it4;
					break;
				}
			case IdlType::Typedef: {
					TypedefType *t = (*it4)->asType<TypedefType>();
					if (t->hasIdentifier(name))
						res = *it4;
					break;
				}
			case IdlType::Enum: {
					EnumType *e = (*it4)->asType<EnumType>();
					if(e->identifier() == name)
						res = *it4;
						break;
				}
			default:
				break;
		}
	}

	// create a new named refering to the found type
	if(res) {
		IdlType::Ptr p(new NamedType(name, res));
		return p;
	} else
		return IdlType::Ptr();
}

IdlType::Ptr Component::typeFromIdsName(const std::string &name)
{
	if(!IDSType) {
		cout << "Warning: Trying to access an IDS type but there is no IDS type defined yet" << endl;
		return IdlType::Ptr();
	}
  
	IdlType::Ptr ids = IDSType->unalias();
	switch(ids->kind()) {
		case IdlType::Struct:{
			StructType *s = ids->asType<StructType>();
			if(s)
				return s->member(name);
		}
		default:
			cerr << "Error: there is no " << name << " member in the ids" << endl;
			return IdlType::Ptr();
	}
}

void Component::addImportedComponent(const std::string &s)
{
	m_importedComponents.push_back(s);
}

void Component::addNativeTypeInclude(const std::string &s)
{
	m_typesIncludes.push_back(s);
}

Event::Ptr Component::event(const std::string &name)
{
	Event::Map::const_iterator it = events.find(name);
	if (it != events.end())
		return it->second;
	return Event::Ptr();
}

std::vector<std::string> Component::eventsForPort(const std::string &name)
{
	std::vector<std::string> v;
	Service::Map::const_iterator it = services.begin();
	for(; it != services.end(); ++it) {
		Event::RevMap::const_iterator it2 = it->second->events().begin();
		for(; it2 != it->second->events().end(); ++it2) {
			if(it2->first->kind() == Event::PortEv) {
				PortEvent *pev = it2->first->asPortEvent();
				if(pev->portName() == name)
					v.push_back(pev->kindAsString());
			}
		}
	}

	return v;
}

std::vector<std::string> Component::eventsForService(const std::string &name)
{
	std::vector<std::string> v;
	Event::Map::const_iterator it = events.begin();
	for(; it != events.end(); ++it) {
		NamedEvent *nev = it->second->asNamedEvent();
		if(!nev || !nev->aliasEvent())
			continue;

		if(nev->aliasEvent()->kind() == Event::ServiceEv) {
			if(nev->aliasEvent()->asServiceEvent()->serviceName() == name)
				v.push_back(nev->aliasEvent()->asServiceEvent()->kindAsString());
		}
	}

	return v;
}

/******** Port ***************/

void Port::debug()
{
	if (type == Incoming)
		cout << "Inport; ";
	else {
		cout << "Outport; ";
		if(sizeCodel) {
			cout << endl << "Size codel: ";
			sizeCodel->debug();
			cout << endl;
		}
	}

	cout << "type: " << DumpType::dumpType(idlType) << endl;
	debugProperties();
}

/******** Task ***************/

void Task::debug()
{
	cout << "priority: " << priority << ", stackSize: " << stackSize << ", period: " << period << ", delay: " << delay << endl;
	cout << "Error messages: ";
	vector<string>::const_iterator it2 = m_errorMessages.begin();
	for(; it2 != m_errorMessages.end(); ++it2)
		cout << *it2 << " ";
	cout << endl;

	cout << "Codels:" << endl;
	Codel::Map::const_iterator it;
	for (it = m_codels.begin(); it != m_codels.end(); ++it) {
		cout << "\t** " << it->first << " : ";
		it->second->debug();
		cout << endl;
	}

	debugProperties();
}

void Task::addCodel(const std::string& name, G3nom::Codel::Ptr c)
{
	m_codels.insert(make_pair(name,c));
}

Codel::Ptr Task::codel(const std::string &name)
{
	Codel::Map::const_iterator it = m_codels.find(name);
	if(it == m_codels.end())
		return Codel::Ptr();
	return it->second;
}

bool Task::hasCodel(const std::string &name)
{
	Codel::Map::const_iterator it = m_codels.find(name);
	return (it != m_codels.end());
}

void Task::addErrorMessage(const std::string &s)
{
	m_errorMessages.push_back(s);
}

/******** Service ***************/

bool ServiceInput::operator==(const ServiceInput &rhs)
{
	return kind == rhs.kind && identifier == rhs.identifier && defaultValue == rhs.defaultValue && type.get() == rhs.type.get();
}

void Service::debug()
{
	cout << "doc: " << doc << endl;
	cout << "Parent task: " << taskName << endl;

	cout << "Inputs: ";
	for(ServiceInput::Vect::const_iterator it = m_inputs.begin(); it != m_inputs.end(); ++it) {
		if(it->kind == ServiceInput::IDSMember)
			cout << "IDS:" << it->identifier;
		else
			cout << DumpType::dumpType(it->type) << " " << it->identifier;
		if(!it->defaultValue.isEmpty())
			cout << " = " << it->defaultValue.print() << ", ";
	}

	cout << endl << "Output: ";
	if(output.identifier.empty())
		cout << "None" << endl;
	else if(output.kind == ServiceInput::IDSMember)
		cout << "IDS:" << output.identifier << endl;
	else
		cout << DumpType::dumpType(output.type) << " " << output.identifier << endl;

	cout << "Error messages: ";
	for(vector<string>::const_iterator it2 = m_errorMessages.begin(); it2 != m_errorMessages.end(); ++it2)
		cout << *it2 << " ";
	cout << endl;

	cout << "Incompatible services: " ;
	if(m_incompatibleServices.empty())
		cout << "none" << endl;
	else {
		vector<string>::const_iterator it2 = m_incompatibleServices.begin();
		for(; it2 != m_incompatibleServices.end(); ++it2)
			cout << *it2 << " ";
		cout << endl;
	}

	cout << "Codels:" << endl;
	Codel::Map::const_iterator it;
	for (it = m_codels.begin(); it != m_codels.end(); ++it) {
		cout << "\t** " << it->first << " : ";
		it->second->debug();
		cout << endl;
	}

	cout << "Events: " << endl;
	Event::RevMap::const_iterator it3;
	for (it3 = m_events.begin(); it3 != m_events.end(); ++it3) {
		cout << "\t" << it3->first->identifier() << " -> " << it3->second << endl;
	}

	debugProperties();
}

// void Service::addInput(const std::string &s, Idl::IdlType::Ptr t, const Idl::Literal &defaultValue)
// {
// 	Input i;
// 	if(!t)
// 		i.kind = Input::IDSMember;
// 	else {
// 		i.kind = Input::Type;
// 		i.type = t;
// 	}
// 
// 	i.identifier = s;
// 	i.defaultValue = defaultValue;
// 	m_inputs.push_back(i);
// }

void Service::addInput(const ServiceInput &i)
{
	m_inputs.push_back(i);
}

Idl::Literal Service::inputDefaultArg(const std::string &n)
{
	for(ServiceInput::Vect::const_iterator it = m_inputs.begin(); it != m_inputs.end(); ++it) {
		if(it->identifier == n)
			return it->defaultValue;
	}
	return Literal();
}

void Service::addCodel(const std::string &name, Codel::Ptr c)
{
	string fixedName = name;
	// make sure the name doesn't contain '.' (for events codels)
	replaceAll(fixedName, ".", "_");
	m_codels.insert(make_pair(fixedName,c));
}

Codel::Ptr Service::codel(const std::string &name)
{
	Codel::Map::const_iterator it = m_codels.find(name);
	if(it == m_codels.end())
		return Codel::Ptr();
	return it->second;
}

bool Service::hasCodel(const std::string &name)
{
	Codel::Map::const_iterator it = m_codels.find(name);
	return (it != m_codels.end());
}

void Service::addErrorMessage(const std::string &s)
{
	m_errorMessages.push_back(s);
}

void Service::addIncompatibleService(const std::string &name)
{
	m_incompatibleServices.push_back(name);
}

void Service::addEvent(Event::Ptr event, const std::string &target)
{
// 	m_events.push_back(make_pair(event, target));
	m_events[event] = target;
}

/******** Codel ***************/

void Codel::debug()
{
	cout << "(" << m_name;

	if(!inPorts.empty()) {
		cout << "inports: ";
		for (vector<string>::const_iterator it = inPorts.begin(); it != inPorts.end(); ++it)
			cout << *it << ", ";
	}

	if(!outPorts.empty()) {
		cout << "; outports: ";
		for (vector<string>::const_iterator it = outPorts.begin(); it != outPorts.end(); ++it)
			cout << *it << ", ";
	}

	if(!inTypes.empty()) {
		cout << "; intType: ";
		for (vector<string>::const_iterator it = inTypes.begin(); it != inTypes.end(); ++it)
			cout << *it << ", ";
	}

	if(!outTypes.empty()) {
		cout << "; outTypes: ";
		for (vector<string>::const_iterator it = outTypes.begin(); it != outTypes.end(); ++it)
			cout << *it << ", ";
	}

	if(!nextCodels.empty()) {
		cout << "; Next codels: ";
		for (vector<string>::const_iterator it = nextCodels.begin(); it != nextCodels.end(); ++it)
			cout << *it << ", ";
	}

	cout << ")";
}

void Codel::addInType(const std::string &t)
{
	inTypes.push_back(t);
}

void Codel::addOutType(const std::string &t)
{
	outTypes.push_back(t);
}

void Codel::addInPort(const std::string &t)
{
	inPorts.push_back(t);
}

void Codel::addOutPort(const std::string &t)
{
	outPorts.push_back(t);
}

/******** Event ***************/

PortEvent* Event::asPortEvent()
{
	return static_cast<PortEvent*>(this);
}

NamedEvent* Event::asNamedEvent()
{
	return static_cast<NamedEvent*>(this);
}

ServiceEvent* Event::asServiceEvent()
{
	return static_cast<ServiceEvent*>(this);
}

/******** PortEvent ***************/

std::string PortEvent::kindAsString() const
{
	switch(m_portKind) {
		case OnRead:
			return "onRead";
		case OnWrite:
			return "onWrite";
		case OnUpdate:
			return "onUpdate";
		case OnInitialize:
			return "onInitialize";
	}
	return "";
}


/******** ServiceEvent ***************/

std::string ServiceEvent::kindAsString() const
{
	switch(m_serviceKind) {
		case OnCalled:
			return "control";
		case OnStart:
			return "start";
		case OnEnd:
			return "end";
		case OnInter:
			return "inter";
		case OnCodel:
			return m_codelName;
	}
	return "";
}

std::string ServiceEvent::identifier() const 
{
	if(m_service.empty())
		return kindAsString();
	else
		return m_service + "." + kindAsString(); 
}

/******** IObjectProperties ***************/

void IObjectProperties::addProperty(const std::string &name, const Idl::Literal &value)
{
	m_properties[name] = value;
}

void IObjectProperties::debugProperties() const
{
	cout << "Properties :" << endl;
	Literal::Map::const_iterator it = m_properties.begin();
	for(; it != m_properties.end(); ++it)
		cout << "\t* " << it->first << " : " <<it->second.print() << endl;
}

Idl::Literal& IObjectProperties::property(std::string s)
{
	return m_properties[s];
}

// kate: indent-mode cstyle; replace-tabs off; tab-width 4;  replace-tabs off;
