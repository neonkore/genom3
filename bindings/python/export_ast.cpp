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

#include <boost/python.hpp>

#include "utils/ast.h"

using namespace G3nom;
using namespace boost::python;

void export_ast()
{

	class_<Component>("Component")
	.def("name", &Component::name)
	.def_readonly("IDSType", &Component::IDSType)
	.def_readonly("uniqueId", &Component::uniqueId)
	.def_readonly("language", &Component::pluginLanguage)
	.def_readonly("version", &Component::version)
	.def("task", &Component::task)
	.def("debug", &Component::debug)
	.def("tasksMap", &Component::tasksMap, return_value_policy<reference_existing_object>())
	.def("taskIndex", &Component::taskIndex)
	.def("servicesMap", &Component::servicesMap, return_value_policy<reference_existing_object>())
	.def("serviceIndex", &Component::serviceIndex)
	.def("portsMap", &Component::portsMap, return_value_policy<reference_existing_object>())
	.def("typesVect", &Component::typesVect, return_value_policy<reference_existing_object>())
	.def("importedComponents", &Component::importedComponents, return_value_policy<reference_existing_object>())
	.def("typeFromIdsName", &Component::typeFromIdsName);

	class_<Task, Task::Ptr>("Task")
	.def("debug", &Task::debug)
	.def("codel", &Task::codel)
	.def("hasCodel", &Task::hasCodel)
	.def_readonly("name", &Task::name)
	.def_readonly("stackSize", &Task::stackSize)
	.def_readonly("priority", &Task::priority)
	.def_readonly("delay", &Task::delay)
	.def_readonly("period", &Task::period)
	.def("errorMessages", &Task::errorMessages, return_value_policy<reference_existing_object>());

	class_<Service, Service::Ptr>("Service")
	.def("debug", &Service::debug)
	.def_readonly("name", &Service::name)
	.def_readonly("taskName", &Service::taskName)
	.def_readonly("type", &Service::type)
	.def("codel", &Service::codel)
	.def("hasCodel", &Service::hasCodel)
	.def_readonly("output", &Service::output)
	.def("codels", &Service::codels, return_value_policy<reference_existing_object>())
	.def("inputs", &Service::inputs, return_value_policy<reference_existing_object>())
	.def("errorMessages", &Service::errorMessages, return_value_policy<reference_existing_object>());

	enum_<Service::Type>("ServiceType")
	.value("Init", Service::Init)
	.value("Control", Service::Control)
	.value("Exec", Service::Exec);

	class_<Codel, Codel::Ptr>("Codel")
	.def_readonly("name", &Codel::name)
	.def_readonly("inTypes", &Codel::inTypes)
	.def_readonly("outTypes", &Codel::outTypes);

	class_<Port, Port::Ptr>("Port")
	.def_readonly("name", &Port::name)
	.def_readonly("idlType", &Port::idlType)
	.def_readonly("type", &Port::type);

	enum_<Port::Type>("PortType")
	.value("Incoming", Port::Incoming)
	.value("Outgoing", Port::Outgoing);
}
