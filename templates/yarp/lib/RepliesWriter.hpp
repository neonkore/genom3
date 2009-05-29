/********************************/
/* Copyright (c) 2007 LAAS/CNRS */
/* Maxime Ransan - oct 2007     */
/* All rights reserved.         */
/********************************/


#ifndef REPLY_WRITER_HPP
#define REPLY_WRITER_HPP


namespace GenomYarp {

#include "gurus/cmpntStruct.hpp"

/**
 *Template class used to write and read replies over the network.
 */	
template<class T_OUTPUT>
class ReplyWriter { 
  
public:
  static int write(yarp::os::Bottle &bottle, const std::string &clientName,
	     int rqst_id, const std::string &requestName, const std::string &report, T_OUTPUT *output)
  {
    bottle.clear();

    bottle.addString(clientName.c_str() );
    bottle.addInt(rqst_id);
    bottle.addString(report.c_str());
    
    yarp::os::Bottle& b_output = bottle.addList();
    cmpnt::YarpCodec<T_OUTPUT>::encode(&b_output,*output);
  }

  static int send(yarp::os::BufferedPort<yarp::os::Bottle> &port, const std::string &clientName,
	     int rqst_id, const std::string &requestName, const std::string &report, T_OUTPUT *output)
  {
    yarp::os::Bottle& bottle = port.prepare();
    write(bottle, clientName, rqst_id, requestName, report, output);ss
        
    port.writeStrict();      
    return 0;
  }
};

}

#endif
