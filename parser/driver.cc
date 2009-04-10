// $Id: driver.cc 39 2008-08-03 10:07:15Z tb $
/** \file driver.cc Implementation of the example::Driver class. */

#include <fstream>
#include <sstream>

#include "driver.h"
#include "lexer.h"
#include "parser.h"

namespace G3nom {

Driver::Driver(class CalcContext& _calc)
    : m_verboseLexing(false),
      m_verboseParsing(false)
{
}

bool Driver::parse_stream(std::istream& in, const std::string& sname)
{
    m_streamName = sname;

    m_lexer = new Lexer(&in);
    m_lexer.setDebug(verbose_lexing);

    Parser parser(*this);
    parser.setDebugLevel(m_verboseParsing);
    bool b = (parser.parse() == 0);

    delete m_lexer;
    m_lexer = 0;
    return b;
}

bool Driver::parseFile(const std::string &filename)
{
    std::ifstream in(filename.c_str());
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

} // namespace example
