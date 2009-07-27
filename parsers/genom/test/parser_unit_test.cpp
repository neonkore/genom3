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

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "driver.h"
#include "idltype.h"

using namespace std;
using namespace G3nom;

string component_base = 
			   "struct DemoIDS { long a; double b; char c; };"
	           "component demo {\n"
	           "language:	\"C\";\n"
	           "ids: DemoIDS;\n"
	           "version: 	\"0.1\";\n"
	           "uniqueId: 9000;\n"
	           "requires: pilo, loco;\n"
	           "};\n\n";

BOOST_AUTO_TEST_CASE(testComponent)
{
	Driver d;
	string s = "struct DemoIDS { long s; };"
	           "component demo {\n"
	           "language:	\"C\";\n"
	           "ids: DemoIDS;\n"
	           "version: 	\"0.1\";\n"
	           "uniqueId: 9000;\n"
	           "requires: pilo, loco;\n"
	           "};\n";

	if (!d.parseString(s)) {
		BOOST_ERROR("Parsing failed.");
		return;
	}

	BOOST_CHECK_EQUAL(d.component().name(), "demo");
	BOOST_CHECK_EQUAL(d.component().pluginLanguage, "C");
	BOOST_CHECK_EQUAL(d.component().version, "0.1");
	BOOST_CHECK_EQUAL(d.component().uniqueId, 9000);
	BOOST_CHECK_EQUAL(d.component().importedComponents().size(), 2);
	BOOST_CHECK_EQUAL(d.component().importedComponents()[0], "pilo");
	BOOST_CHECK_EQUAL(d.component().importedComponents()[1], "loco");
	BOOST_CHECK_EQUAL(d.component().IDSType->identifier(), "DemoIDS");
}

BOOST_AUTO_TEST_CASE(testComponent2)
{
	Driver d;
	string s = "struct DemoIDS { long s; };"
	           "component demo {\n"
	           "requires: pilo;\n"
	           "};\n";

	if (!d.parseString(s)) {
		BOOST_ERROR("Parsing failed.");
		return;
	}

	BOOST_CHECK_EQUAL(d.component().name(), "demo");
	BOOST_CHECK_EQUAL(d.component().importedComponents().size(), 1);
	BOOST_CHECK_EQUAL(d.component().importedComponents()[0], "pilo");
}

BOOST_AUTO_TEST_CASE(testServiceInputs)
{
	Driver d;
	string str = component_base + "service SetSpeed {"
	           "type: control;\n"
	           "input: long a = 1 : \"First arg\", double x : \"Second arg\" = 2.3, string z = \"bla\", string y : \"Last arg\";\n"
			   "output: IDS.c;\n"
			   "codel control: SetSpeedControl();"
			   "taskName: MotionTask;"
			   "interrupts: SetSpeed, GetSpeed, Goto;"
			   "errors: OUT_OF_MEMORY, INVALID_ARG;"
	           "};\n";

	if (!d.parseString(str)) {
		BOOST_ERROR("Parsing failed.");
		return;
	}

	BOOST_CHECK_EQUAL(d.component().servicesMap().size(), 1);

	Service::Ptr s = d.component().service("SetSpeed");
	if (!s.get()) {
		BOOST_ERROR("component.service() failed for SetSpeed.");
		return;
	}
	BOOST_CHECK_EQUAL(s->name, "SetSpeed");
	BOOST_CHECK_EQUAL(s->type, Service::Control);
	BOOST_CHECK_EQUAL(s->taskName, "MotionTask");

	BOOST_CHECK_EQUAL(s->incompatibleServices().size(), 3);
	BOOST_CHECK_EQUAL(s->incompatibleServices()[0], "SetSpeed");
	BOOST_CHECK_EQUAL(s->incompatibleServices()[1], "GetSpeed");
	BOOST_CHECK_EQUAL(s->incompatibleServices()[2], "Goto");

	BOOST_CHECK_EQUAL(s->errorMessages().size(), 2);
	BOOST_CHECK_EQUAL(s->errorMessages()[0], "OUT_OF_MEMORY");
	BOOST_CHECK_EQUAL(s->errorMessages()[1], "INVALID_ARG");

	BOOST_CHECK_EQUAL(s->output.kind, ServiceInput::IDSMember);
	BOOST_CHECK(!s->output.type);
	BOOST_CHECK_EQUAL(s->output.identifier, "c");
	BOOST_CHECK(s->output.defaultValue.isEmpty());

	BOOST_CHECK_EQUAL(s->inputs().size(), 4);
	BOOST_CHECK_EQUAL(s->inputs()[0].kind, ServiceInput::Type);
	BOOST_CHECK_EQUAL(s->inputs()[0].type, Idl::BaseType::longType);
	BOOST_CHECK_EQUAL(s->inputs()[0].identifier, "a");
	BOOST_CHECK_EQUAL(s->inputs()[0].defaultValue.asInt(), 1);
	BOOST_CHECK_EQUAL(s->inputs()[0].doc, "First arg");

	BOOST_CHECK_EQUAL(s->inputs()[1].kind, ServiceInput::Type);
	BOOST_CHECK_EQUAL(s->inputs()[1].type, Idl::BaseType::doubleType);
	BOOST_CHECK_EQUAL(s->inputs()[1].identifier, "x");
	BOOST_CHECK_EQUAL(s->inputs()[1].defaultValue.asDouble(), 2.3);
	BOOST_CHECK_EQUAL(s->inputs()[1].doc, "Second arg");

	BOOST_CHECK_EQUAL(s->inputs()[2].kind, ServiceInput::Type);
	BOOST_CHECK_EQUAL(s->inputs()[2].type, Idl::StringType::unboundedStringType);
	BOOST_CHECK_EQUAL(s->inputs()[2].identifier, "z");
	BOOST_CHECK_EQUAL(s->inputs()[2].defaultValue.asString(), "\"bla\"");
	BOOST_CHECK(s->inputs()[2].doc.empty());

	BOOST_CHECK_EQUAL(s->inputs()[3].kind, ServiceInput::Type);
	BOOST_CHECK_EQUAL(s->inputs()[3].type, Idl::StringType::unboundedStringType);
	BOOST_CHECK_EQUAL(s->inputs()[3].identifier, "y");
	BOOST_CHECK(s->inputs()[3].defaultValue.isEmpty());
	BOOST_CHECK_EQUAL(s->inputs()[3].doc, "Last arg");
}

BOOST_AUTO_TEST_CASE(testServiceCodel)
{
	Driver d;
	string str = component_base + "service SetSpeed {"
	           "type: exec;\n"
	           "input: long a;"
			   "output: IDS.a;\n"
			   "interrupts: all;"
			   "errors: OUT_OF_MEMORY;"
			   "codel main: SetSpeedMain(in a, in b, out c, inport IntIn, outport IntOut) -> main, sleep, ether;"
	           "};\n";

	if (!d.parseString(str)) {
		BOOST_ERROR("Parsing failed.");
		return;
	}

	BOOST_CHECK_EQUAL(d.component().servicesMap().size(), 1);

	Service::Ptr s = d.component().service("SetSpeed");
	if (!s.get()) {
		BOOST_ERROR("component.service() failed for SetSpeed.");
		return;
	}
	BOOST_CHECK_EQUAL(s->name, "SetSpeed");
	BOOST_CHECK_EQUAL(s->type, Service::Exec);

	BOOST_CHECK_EQUAL(s->incompatibleServices().size(), 1);
	BOOST_CHECK_EQUAL(s->incompatibleServices()[0], "all");

	BOOST_CHECK_EQUAL(s->errorMessages().size(), 1);
	BOOST_CHECK_EQUAL(s->errorMessages()[0], "OUT_OF_MEMORY");

	Codel::Ptr c = s->codel("main");
	if (!c) {
		BOOST_ERROR("service.codel() failed for main.");
		return;
	}
	BOOST_CHECK_EQUAL(c->name(), "SetSpeedMain");
	BOOST_CHECK_EQUAL(c->inTypes.size(), 2);
	BOOST_CHECK_EQUAL(c->inTypes[0], "a");
	BOOST_CHECK_EQUAL(c->inTypes[1], "b");
	BOOST_CHECK_EQUAL(c->outTypes.size(), 1);
	BOOST_CHECK_EQUAL(c->outTypes[0], "c");
	BOOST_CHECK_EQUAL(c->nextCodels.size(), 3);
	BOOST_CHECK_EQUAL(c->nextCodels[0], "main");
	BOOST_CHECK_EQUAL(c->nextCodels[1], "sleep");
	BOOST_CHECK_EQUAL(c->nextCodels[2], "ether");
}

BOOST_AUTO_TEST_CASE(testServiceEvents)
{
	Driver d;
	string str = component_base +
			   "inport DemoIDS IntIn;\n"
			   "service SetSpeed {"
	           "type: init;\n"
			   "event onStart: SetSpeedStart();"
			   "event onInter: SetSpeedInter();"
			   "event IntIn.onUpdate: IntInUpdated();"
	           "};\n";

	if (!d.parseString(str)) {
		BOOST_ERROR("Parsing failed.");
		return;
	}

	BOOST_CHECK_EQUAL(d.component().servicesMap().size(), 1);

	Service::Ptr s = d.component().service("SetSpeed");
	if (!s.get()) {
		BOOST_ERROR("component.service() failed for SetSpeed.");
		return;
	}
	BOOST_CHECK_EQUAL(s->name, "SetSpeed");
	BOOST_CHECK_EQUAL(s->type, Service::Init);

	BOOST_CHECK_EQUAL(s->events().size(), 3);
}

BOOST_AUTO_TEST_CASE(testTask)
{
	Driver d;
	string s = "task main { \n"
	           "priority: 100;\n"
	           "period: 5;\n"
	           "delay: 0x2;\n"
	           "stackSize:  10000;\n"
	           "errors: NO_MORE_SPACE, INVALID_CMD, OTHER_ERROR;\n"
	           "};\n"
			   "task open { \n"
	           "errors: NO_MORE_SPACE;\n"
	           "};\n";

	if (!d.parseString(s)) {
		BOOST_ERROR("Parsing failed.");
		return;
	}

	BOOST_CHECK_EQUAL(d.component().tasksMap().size(), 2);

	Task::Ptr t = d.component().task("main");
	if (!t.get()) {
		BOOST_ERROR("component.task() failed for main.");
		return;
	}
	BOOST_CHECK_EQUAL(t->name, "main");
	BOOST_CHECK_EQUAL(t->period, 5);
	BOOST_CHECK_EQUAL(t->delay, 2);
	BOOST_CHECK_EQUAL(t->stackSize, 10000);
	BOOST_CHECK_EQUAL(t->errorMessages().size(), 3);
	BOOST_CHECK_EQUAL(t->errorMessages()[0], "NO_MORE_SPACE");
	BOOST_CHECK_EQUAL(t->errorMessages()[1], "INVALID_CMD");
	BOOST_CHECK_EQUAL(t->errorMessages()[2], "OTHER_ERROR");

	Task::Ptr tt = d.component().task("open");
	if (!t.get()) {
		BOOST_ERROR("component.task() failed for open.");
		return;
	}
	BOOST_CHECK_EQUAL(tt->errorMessages().size(), 1);
	BOOST_CHECK_EQUAL(tt->errorMessages()[0], "NO_MORE_SPACE");
}

BOOST_AUTO_TEST_CASE(testPorts)
{
	Driver d;
	string s = "struct Integer { long s; };"
	           "inport Integer IntIn;\n"
	           "outport Integer IntOut;\n"
	           "outport double DoubleOut;\n";

	if (!d.parseString(s)) {
		BOOST_ERROR("Parsing failed.");
		return;
	}

	BOOST_CHECK_EQUAL(d.component().portsMap().size(), 3);

	Port::Ptr intin = d.component().port("IntIn");
	if (!intin) {
		BOOST_ERROR("component.port() failed for IntIn.");
		return;
	}
	BOOST_CHECK_EQUAL(intin->name, "IntIn");
	BOOST_CHECK_EQUAL(intin->type, Port::Incoming);
	if(!intin->idlType.get()) {
		BOOST_ERROR("IntIn idlType is empty.");
		return;
	}
	BOOST_CHECK_EQUAL(intin->idlType->identifier(), "Integer");
	BOOST_CHECK_EQUAL(intin->sizeCodel, Codel::Ptr());

	Port::Ptr intout = d.component().port("IntOut");
	if (!intout) {
		BOOST_ERROR("component.port() failed for IntOut.");
		return;
	}
	BOOST_CHECK_EQUAL(intout->name, "IntOut");
	BOOST_CHECK_EQUAL(intout->type, Port::Outgoing);
	if(!intout->idlType) {
		BOOST_ERROR("IntOut idlType is empty.");
		return;
	}
	BOOST_CHECK_EQUAL(intout->idlType->identifier(), "Integer");
	BOOST_CHECK_EQUAL(intout->sizeCodel, Codel::Ptr());

	Port::Ptr dbl = d.component().port("DoubleOut");
	if (!dbl) {
		BOOST_ERROR("component.port() failed for DoubleOut.");
		return;
	}
	BOOST_CHECK_EQUAL(dbl->name, "DoubleOut");
	BOOST_CHECK_EQUAL(dbl->type, Port::Outgoing);
	if(!dbl->idlType) {
		BOOST_ERROR("DoubleOut idlType is empty.");
		return;
	}
	BOOST_CHECK_EQUAL(dbl->idlType, Idl::BaseType::doubleType);
	BOOST_CHECK_EQUAL(dbl->sizeCodel, Codel::Ptr());
}

BOOST_AUTO_TEST_CASE(testDynamicPorts)
{
	Driver d;
	string s = component_base + "typedef sequence<long> IntSeq;"
			   "struct Data { long s; IntSeq contents; };"
	           "outport Data DynOut { codel size: mySizeCodel(in a, out b); };\n"
	           "\n";

	if (!d.parseString(s)) {
		BOOST_ERROR("Parsing failed.");
		return;
	}

	BOOST_CHECK_EQUAL(d.component().portsMap().size(), 1);

	Port::Ptr out = d.component().port("DynOut");
	if (!out.get()) {
		BOOST_ERROR("component.port() failed for DynOut.");
		return;
	}
	BOOST_CHECK_EQUAL(out->name, "DynOut");
	BOOST_CHECK_EQUAL(out->type, Port::Outgoing);
	if(!out->idlType.get()) {
		BOOST_ERROR("DynOut idlType is empty.");
		return;
	}
	BOOST_CHECK_EQUAL(out->idlType->identifier(), "Data");
	BOOST_CHECK_EQUAL(out->sizeCodel->name(), "mySizeCodel");
	BOOST_CHECK_EQUAL(out->sizeCodel->inTypes.size(), 1);
	BOOST_CHECK_EQUAL(out->sizeCodel->inTypes[0], "a");
	BOOST_CHECK_EQUAL(out->sizeCodel->outTypes.size(), 1);
	BOOST_CHECK_EQUAL(out->sizeCodel->outTypes[0], "b");
}

BOOST_AUTO_TEST_CASE(testEvents)
{
	Driver d;
	string s = "event ExtTrigger;\n"
			   "inport long IntIn;\n"
	           "event EventOne, EventTwo = IntIn.onUpdate, EventThree;\n";

	if (!d.parseString(s)) {
		BOOST_ERROR("Parsing failed.");
		return;
	}

	BOOST_CHECK_EQUAL(d.component().eventsMap().size(), 4);

	Event::Ptr ev = d.component().event("EventTwo");
	BOOST_CHECK_EQUAL(ev->kind(), Event::NamedEv);
	BOOST_CHECK_EQUAL(ev->identifier(), "EventTwo");
	BOOST_CHECK_EQUAL(ev->asNamedEvent()->aliasEvent()->kind(), Event::PortEv);
	BOOST_CHECK_EQUAL(ev->asNamedEvent()->aliasEvent()->asPortEvent()->portName(), "IntIn");
}

BOOST_AUTO_TEST_CASE(testSimpleTypes)
{
	Driver d;
	string str = "struct Data {\n"
			   "long a;\n"
			   "long long b;\n"
			   "unsigned long c;\n"
			   "unsigned long long d;\n"
			   "short e;\n"
			   "unsigned short f;\n"
			   "char g;\n"
			   "octet h;\n"
			   "boolean i;\n"
			   "wchar j;\n"
			   "float k;\n"
			   "double l;\n"
			   "long double m;\n"
			   "};\n";

	if (!d.parseString(str)) {
		BOOST_ERROR("Parsing failed.");
		return;
	}

	BOOST_CHECK_EQUAL(d.component().typesVect().size(), 1);

	Idl::IdlType::Ptr t = d.component().typeFromName("Data")->unalias();
	if(!t) {
		BOOST_ERROR("Struct type parsing failed.");
		return;
	}
	
	BOOST_CHECK_EQUAL(t->kind(), Idl::IdlType::Struct);

	Idl::StructType *s = t->asType<Idl::StructType>();
	if(!s) {
		BOOST_ERROR("Main type is not a struct type.");
		return;
	}
	BOOST_CHECK_EQUAL(s->members().size(), 13);
	BOOST_CHECK_EQUAL(s->member("a"), Idl::BaseType::longType);
	BOOST_CHECK_EQUAL(s->member("b"), Idl::BaseType::longlongType);
	BOOST_CHECK_EQUAL(s->member("c"), Idl::BaseType::ulongType);
	BOOST_CHECK_EQUAL(s->member("d"), Idl::BaseType::ulonglongType);
	BOOST_CHECK_EQUAL(s->member("e"), Idl::BaseType::shortType);
	BOOST_CHECK_EQUAL(s->member("f"), Idl::BaseType::ushortType);
	BOOST_CHECK_EQUAL(s->member("g"), Idl::BaseType::charType);
	BOOST_CHECK_EQUAL(s->member("h"), Idl::BaseType::octetType);
	BOOST_CHECK_EQUAL(s->member("i"), Idl::BaseType::booleanType);
	BOOST_CHECK_EQUAL(s->member("j"), Idl::BaseType::wcharType);
	BOOST_CHECK_EQUAL(s->member("k"), Idl::BaseType::floatType);
	BOOST_CHECK_EQUAL(s->member("l"), Idl::BaseType::doubleType);
	BOOST_CHECK_EQUAL(s->member("m"), Idl::BaseType::longdoubleType);
}

BOOST_AUTO_TEST_CASE(testTemplatesTypes)
{
	Driver d;
	string str = "struct Data {\n"
			   "string a;\n"
			   "string<24> b;\n"
			   "sequence<long> c;\n"
			   "sequence<long, 19> d;\n"
			   "wstring e;\n"
			   "wstring<10> f;\n"
			   "};\n";

	if (!d.parseString(str)) {
		BOOST_ERROR("Parsing failed.");
		return;
	}

	BOOST_CHECK_EQUAL(d.component().typesVect().size(), 1);

	Idl::IdlType::Ptr t = d.component().typeFromName("Data")->unalias();
	if(!t) {
		BOOST_ERROR("Struct type parsing failed.");
		return;
	}
	
	BOOST_CHECK_EQUAL(t->kind(), Idl::IdlType::Struct);

	Idl::StructType *s = t->asType<Idl::StructType>();
	if(!s) {
		BOOST_ERROR("Main type is not a struct type.");
		return;
	}
	BOOST_CHECK_EQUAL(s->members().size(), 6);
	BOOST_CHECK_EQUAL(s->member("a")->kind(), Idl::IdlType::String);
	BOOST_CHECK_EQUAL(s->member("a")->asType<Idl::StringType>()->bound(), 0);
	BOOST_CHECK_EQUAL(s->member("b")->kind(), Idl::IdlType::String);
	BOOST_CHECK_EQUAL(s->member("b")->asType<Idl::StringType>()->bound(), 24);

	BOOST_CHECK_EQUAL(s->member("c")->kind(), Idl::IdlType::Sequence);
	BOOST_CHECK_EQUAL(s->member("c")->asType<Idl::SequenceType>()->bound(), 0);
	BOOST_CHECK_EQUAL(s->member("c")->asType<Idl::SequenceType>()->seqType(), Idl::BaseType::longType);
	BOOST_CHECK_EQUAL(s->member("d")->kind(), Idl::IdlType::Sequence);
	BOOST_CHECK_EQUAL(s->member("d")->asType<Idl::SequenceType>()->bound(), 19);
	BOOST_CHECK_EQUAL(s->member("d")->asType<Idl::SequenceType>()->seqType(), Idl::BaseType::longType);

	BOOST_CHECK_EQUAL(s->member("e")->kind(), Idl::IdlType::WString);
	BOOST_CHECK_EQUAL(s->member("e")->asType<Idl::WStringType>()->bound(), 0);
	BOOST_CHECK_EQUAL(s->member("f")->kind(), Idl::IdlType::WString);
	BOOST_CHECK_EQUAL(s->member("f")->asType<Idl::WStringType>()->bound(), 10);
}

BOOST_AUTO_TEST_CASE(testEnum)
{
	Driver d;
	string str = "enum Values {\n"
			   "Value1, OtherValue, LastValue\n"
			   "};\n";

	if (!d.parseString(str)) {
		BOOST_ERROR("Parsing failed.");
		return;
	}

	BOOST_CHECK_EQUAL(d.component().typesVect().size(), 1);

	Idl::IdlType::Ptr t = d.component().typeFromName("Values")->unalias();
	if(!t) {
		BOOST_ERROR("Struct type parsing failed.");
		return;
	}
	
	BOOST_CHECK_EQUAL(t->kind(), Idl::IdlType::Enum);

	Idl::EnumType *e = t->asType<Idl::EnumType>();
	if(!e) {
		BOOST_ERROR("Main type is not a enum type.");
		return;
	}
	BOOST_CHECK_EQUAL(e->enumerators().size(), 3);
	BOOST_CHECK_EQUAL(e->enumerators()[0], "Value1");
	BOOST_CHECK_EQUAL(e->enumerators()[1], "OtherValue");
	BOOST_CHECK_EQUAL(e->enumerators()[2], "LastValue");
}

BOOST_AUTO_TEST_CASE(testStruct)
{
	Driver d;
	string str = "struct Data {\n"
			   "long a, b;\n"
			   "double c[10];\n"
			   "float d[10][20];"
			   "};\n";

	if (!d.parseString(str)) {
		BOOST_ERROR("Parsing failed.");
		return;
	}

	BOOST_CHECK_EQUAL(d.component().typesVect().size(), 1);

	Idl::IdlType::Ptr t = d.component().typeFromName("Data")->unalias();
	if(!t) {
		BOOST_ERROR("Struct type parsing failed.");
		return;
	}
	
	BOOST_CHECK_EQUAL(t->kind(), Idl::IdlType::Struct);

	Idl::StructType *s = t->asType<Idl::StructType>();
	if(!s) {
		BOOST_ERROR("Main type is not a struct type.");
		return;
	}
	BOOST_CHECK_EQUAL(s->members().size(), 4);
	BOOST_CHECK_EQUAL(s->member("a"), Idl::BaseType::longType);
	BOOST_CHECK_EQUAL(s->member("b"), Idl::BaseType::longType);

	BOOST_CHECK_EQUAL(s->member("c")->kind(), Idl::IdlType::Array);
	Idl::ArrayType *a = s->member("c")->asType<Idl::ArrayType>();
	BOOST_CHECK_EQUAL(a->type(), Idl::BaseType::doubleType);
	BOOST_CHECK_EQUAL(a->bounds().size(), 1);
	BOOST_CHECK_EQUAL(a->bounds()[0], 10);

	BOOST_CHECK_EQUAL(s->member("d")->kind(), Idl::IdlType::Array);
	a = s->member("d")->asType<Idl::ArrayType>();
	BOOST_CHECK_EQUAL(a->type(), Idl::BaseType::floatType);
	BOOST_CHECK_EQUAL(a->bounds().size(), 2);
	BOOST_CHECK_EQUAL(a->bounds()[0], 10);
	BOOST_CHECK_EQUAL(a->bounds()[1], 20);
}

BOOST_AUTO_TEST_CASE(testTypedef)
{
	Driver d;
	string str = "typedef sequence<octet> OctetSeq;\n"
			   "typedef sequence<long> IntSeq, LongSeq;"
			   "\n";

	if (!d.parseString(str)) {
		BOOST_ERROR("Parsing failed.");
		return;
	}

	BOOST_CHECK_EQUAL(d.component().typesVect().size(), 2);

	Idl::IdlType::Ptr t = d.component().typeFromName("OctetSeq")->asType<Idl::NamedType>()->type();
	if(!t) {
		BOOST_ERROR("Type parsing failed.");
		return;
	}
	BOOST_CHECK_EQUAL(t->kind(), Idl::IdlType::Typedef);
	BOOST_CHECK_EQUAL(t->asType<Idl::TypedefType>()->aliasType()->kind(), Idl::IdlType::Sequence);

	t = d.component().typeFromName("IntSeq")->asType<Idl::NamedType>()->type();
	if(!t) {
		BOOST_ERROR("Type parsing failed.");
		return;
	}
	BOOST_CHECK_EQUAL(t->kind(), Idl::IdlType::Typedef);
	BOOST_CHECK_EQUAL(t->asType<Idl::TypedefType>()->aliasType()->kind(), Idl::IdlType::Sequence);

	t = d.component().typeFromName("LongSeq")->asType<Idl::NamedType>()->type();
	if(!t) {
		BOOST_ERROR("Type parsing failed.");
		return;
	}
	BOOST_CHECK_EQUAL(t->kind(), Idl::IdlType::Typedef);
	BOOST_CHECK_EQUAL(t->asType<Idl::TypedefType>()->aliasType()->kind(), Idl::IdlType::Sequence);
}


// kate: indent-mode cstyle; replace-tabs off; tab-width 4; 
