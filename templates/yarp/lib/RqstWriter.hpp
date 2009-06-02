/********************************/
/* Copyright (c) 2007 LAAS/CNRS */
/* Maxime Ransan - oct 2007     */
/* All rights reserved.         */
/********************************/


#ifndef SERVICE_RQST_WRITER_HPP
#define SERVICE_RQST_WRITER_HPP

#include <yarp/os/all.h>
#include "YarpCodec.hpp"

namespace GenomYarp {

template<class T_INPUT>
class RqstWriter{
  
  public:
    static void write(yarp::os::Bottle &b, const std::string &clientName, int rqstId, const T_INPUT &input)
    {
	b.clear();
	
	b.addString(clientName.c_str());
	b.addInt(rqstId);
	
	yarp::os::Bottle& b_data = b.addList();
	YarpCodec<T_INPUT>::encode(&b_data,input);
    }

    static void send(yarp::os::BufferedPort<yarp::os::Bottle> &p, const std::string &clientName, int rqstId, const T_INPUT &input)
    {
	yarp::os::Bottle& b = p.prepare();    
	write(b, clientName, rqstId, input);

	p.writeStrict();  
	std::cout << "( RQST ) " << b.toString() << std::endl; 
    }
   
};

}

#endif
