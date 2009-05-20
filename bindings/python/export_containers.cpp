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
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

#include "utils/ast.h"
#include "utils/idltype.h"
#include "utils/idlvalues.h"

using namespace G3nom;
using namespace Idl;
using namespace boost::python;

struct Essai {
  bool operator==(const Essai &rhs) {
      return a == rhs.a && b == rhs.b;
  }

  int a;
  int b;
};

void export_containers()
{
	// Vectors
	class_<std::vector<int> >("IntVec")
	.def(vector_indexing_suite<std::vector<int> >());
	class_<std::vector<std::string> >("StringVec")
	.def(vector_indexing_suite<std::vector<std::string> >());
	class_<IdlType::Vector>("IdlTypeVec")
	.def(vector_indexing_suite<IdlType::Vector, true>());
	class_<Service::Input::Vect>("ServiceInputVec")
	.def(vector_indexing_suite<Service::Input::Vect>());

	// Maps
	class_<Task::Map>("TaskMap")
	.def(map_indexing_suite<Task::Map, true>());
	class_<Service::Map>("ServiceMap")
	.def(map_indexing_suite<Service::Map, true>());
	class_<Codel::Map>("CodelMap")
	.def(map_indexing_suite<Codel::Map, true>());
	class_<Port::Map>("PortMap")
	.def(map_indexing_suite<Port::Map, true>());
	class_<IdlType::Map>("IdlTypeMap")
	.def(map_indexing_suite<IdlType::Map, true>());
	class_<ConstValue::Map>("ConstValueMap")
	.def(map_indexing_suite<ConstValue::Map, true>());
}
