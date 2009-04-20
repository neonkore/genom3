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

namespace G3nom {

class Port {
  public:
    enum Type { Incoming, Outgoing};
    typedef std::map<std::string, Port*> Map;

    Port() {}
    Port(std::string name, Idl::IdlType::Ptr idlType, bool incoming)
      : name(name), idlType(idlType), type(incoming ? Incoming: Outgoing)
    {}

    void debug();
    
    std::string name;
    Type type;
    Idl::IdlType::Ptr idlType;
};

class Codel {
  public:
    typedef std::map<std::string, Codel*> Map;

    Codel() {}

    void debug();

    std::string name;
    std::vector<std::string> inTypes;
    std::vector<std::string> outTypes;
    std::vector<std::string> inPorts;
    std::vector<std::string> outPorts;
};

class Task {
  public:
    typedef std::map<std::string, Task*> Map;

    Task() {}

    void debug();

    std::string name;
    int priority;
    int period; // in ms
    int stackSize; // in kbytes

  private:
    Codel::Map codels; 
    //std::string connectedPort;
};

class Service {
  public:
    typedef std::map<std::string, Service*> Map;
    enum Type { Control, Exec, Init };

    Service() {}

    void debug();

    void addInput(const std::string &s);

    std::string name;
    Type type;
    std::string doc;
    std::string taskName;
    std::string output;

  private:
    Codel::Map codels;
    std::vector<std::string> inputs;
    std::vector<std::string> incompatibleServices;
};

class Component {
  public:
    Component();
    ~Component();

    void debug();

    void addTask(const std::string& name, G3nom::Task* task);
    void addService(const std::string& name, G3nom::Service* task);
    void addPort(const std::string& name, G3nom::Port* port);
    void addType(Idl::IdlType::Ptr type);

    Task::Map& tasksMap();
    std::vector<std::string> tasksList();
    Task* task(const std::string &name);

    Idl::IdlType::Ptr typeFromName(const std::string &name);

    std::string name;
    std::string pluginLanguage;
    std::string version;
    Idl::IdlType::Ptr IDSType;

  private:
    /// @todo use smart pointers or smart maps ?
    Task::Map tasks;
    Service::Map services;
    Port::Map ports;
    Idl::IdlType::Vector types;
};

}

#endif // G3NOM_AST_H
