

#ifndef G3NOM_DRIVER_H
#define G3NOM_DRIVER_H

#include <string>
#include <vector>
#include "ast.h"

namespace G3nom {

class Lexer;
class IdlType;
// class Task;
// class Service;

/** The Driver class brings together all components. It creates an instance of
 * the Parser and Scanner classes and connects them. Then the input stream is
 * fed into the scanner object and the parser gets it's token
 * sequence. Furthermore the driver object is available in the grammar rules as
 * a parameter. Therefore the driver class contains a reference to the
 * structure into which the parsed data is saved. */
class Driver
{
public:
    /// construct a new parser driver context
    Driver();

    /** Invoke the scanner and parser for a stream.
     * @param in	input stream
     * @param sname	stream name for error messages
     * @return		true if successfully parsed
     */
    bool parseStream(std::istream& in,
		      const std::string& sname = "stream input");

    /** Invoke the scanner and parser on an input string.
     * @param input	input string
     * @param sname	stream name for error messages
     * @return		true if successfully parsed
     */
    bool parseString(const std::string& input,
		      const std::string& sname = "string stream");

    /** Invoke the scanner and parser on a file. Use parse_stream with a
     * std::ifstream if detection of file reading errors is required.
     * @param filename	input file name
     * @return		true if successfully parsed
     */
    bool parseFile(const char* filename);

    void setDebug(bool verbose);

    /** Error handling with associated line number. This can be modified to
     * output the error e.g. to a dialog box. */
    void error(const class location& l, const std::string& m);

    /** General error handling. This can be modified to output the error
     * e.g. to a dialog box. */
    void error(const std::string& m);

    std::string* streamNamePtr() { return &m_streamName; }
    Lexer* lexer() { return m_lexer; }
    Component& component() { return m_component; }

    void setCurrentTask(Task *t) { m_currentTask = t; }
    /** Returns the current task, creating it if necessary
    */
    Task* currentTask();

    void setCurrentService(Service *s) { m_currentService = s; }
    /** Returns the current service, creating it if necessary
    */
    Service* currentService();

    IdlType* currentType() const { return m_currentType; }
    void setCurrentType(IdlType *t) { m_currentType = t; }

private:
    /** Pointer to the current lexer instance, this is used to connect the
     * parser to the scanner. It is used in the yylex macro. */
    Lexer* m_lexer;
    /// stream name (file or input stream) used for error messages.
    std::string m_streamName;
    /// enable debug output in the flex scanner
    bool m_verboseLexing;
    /// enable debug output in the bison parser
    bool m_verboseParsing;

    Component m_component;
    Task *m_currentTask;
    Service *m_currentService;
    IdlType *m_currentType;
};

} // namespace example

#endif // G3NOM_DRIVER_H
