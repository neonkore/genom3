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

namespace G3nom
{

class Port
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
};

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
		std::vector<std::string> inPorts;
		std::vector<std::string> outPorts;

	private:
			std::string m_name;
};

class Task
{
	public:
		typedef boost::shared_ptr<Task> Ptr;
		typedef std::map<std::string, Ptr> Map;

		Task() {}
		Task(const std::string id)
		: name(id)
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

class Service
{
	public:
		typedef boost::shared_ptr<Service> Ptr;
		typedef std::map<std::string, Ptr> Map;
		enum Type { Control, Exec, Init };

		struct Input {
			typedef std::vector<Input> Vect;
			enum Kind { IDSMember, Type};

			bool operator==(const Input &rhs); // necessary for boost.python

			Kind kind;
			std::string identifier;
			Idl::IdlType::Ptr type;
			Idl::Literal defaultValue;
		};

		Service() {}
		Service(const std::string &id) 
		: name(id) {}

		void debug();

// 		void addInput(const std::string &s, Idl::IdlType::Ptr t, const Idl::Literal &defaultValue = Idl::Literal());
		void addInput(const Service::Input &i);
		Input::Vect& inputs() { return m_inputs; }
		Idl::Literal inputDefaultArg(const std::string &n); 

		std::vector<std::string>& errorMessages() { return m_errorMessages; }
		void addErrorMessage(const std::string &s);

		void addCodel(const std::string &name, Codel::Ptr c);
		Codel::Map& codels() { return m_codels; }
		Codel::Ptr codel(const std::string &name);
		bool hasCodel(const std::string &name);

		void addIncompatibleService(const std::string &name);
		std::vector<std::string> & incompatibleServices() { return m_incompatibleServices; }

		std::string name;
		Type type;
		std::string doc;
		std::string taskName;
		std::string output;

	private:
		Codel::Map m_codels;
		Input::Vect m_inputs;
		std::vector<std::string> m_incompatibleServices;
		std::vector<std::string> m_errorMessages;
};

class Component
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

		void addConstValue(const Idl::ConstValue &val);
		void addType(Idl::IdlType::Ptr type);

		Task::Map& tasksMap();
		std::vector<std::string> tasksList();
		Task::Ptr task(const std::string &name);
		int taskIndex(const std::string &name) const;
		
		Service::Map& servicesMap();
		Service::Ptr service(const std::string &name);
		int serviceIndex(const std::string &name) const;

		Port::Ptr port(const std::string &name);
		Port::Map& portsMap() { return ports; }

		Idl::IdlType::Vector& typesVect() { return m_types; }
		Idl::ConstValue::Map& valuesMap() { return m_constValues; }

		void addImportedComponent(const std::string &s);
		std::vector<std::string>& importedComponents() { return m_importedComponents; }

		Idl::IdlType::Ptr typeFromName(const std::string &name);
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
		Idl::IdlType::Vector m_types;
		Idl::ConstValue::Map m_constValues;
		std::vector<std::string> m_importedComponents;
};

}

#endif // G3NOM_AST_H
// kate: indent-mode cstyle; replace-tabs off; tab-width 4; 
