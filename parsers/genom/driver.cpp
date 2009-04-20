// $Id: driver.cc 39 2008-08-03 10:07:15Z tb $
/** \file driver.cc Implementation of the example::Driver class. */

#include <fstream>
#include <sstream>

#include "driver.h"
#include "lexer.h"
#include "parser.hpp"

using namespace G3nom;
using namespace Idl;

Driver::Driver()
		: m_verboseLexing(false),
		m_verboseParsing(false)
{}

bool Driver::parseStream(std::istream& in, const std::string& sname)
{
	m_streamName = sname;

	m_lexer = new Lexer(&in);
	m_lexer->setDebug(m_verboseLexing);

	Parser parser(*this);
	parser.set_debug_level(m_verboseParsing);
	bool b = (parser.parse() == 0);

	delete m_lexer;
	m_lexer = 0;
	return b;
}

bool Driver::parseFile(const char *filename)
{
	std::ifstream in(filename);
	if (!in.good())
		return false;
	return parseStream(in, filename);
}

bool Driver::parseString(const std::string &input, const std::string& sname)
{
	std::istringstream iss(input);
	return parseStream(iss, sname);
}

void Driver::error(const class location& l, const std::string& m)
{
	std::cerr << l << ": " << m << std::endl;
}

void Driver::error(const std::string& m)
{
	std::cerr << m << std::endl;
}

void Driver::setDebug(bool verbose)
{
	m_verboseLexing = true;
	m_verboseParsing = true;
}

Service::Ptr Driver::currentService()
{
// 	if (!m_currentService)
// 		m_currentService = new Service();
	return m_currentService;
}

Task::Ptr Driver::currentTask()
{
// 	if (!m_currentTask)
// 		m_currentTask = new Task();
	return m_currentTask;
}
// kate: indent-mode cstyle; replace-tabs off; tab-width 4; 
