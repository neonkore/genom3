/********************************/
/* Copyright (c) 2007 LAAS/CNRS */
/* Maxime Ransan - oct 2007     */
/* All rights reserved.         */
/********************************/


#ifndef CMPNT_STRUCT_HPP
#define CMPNT_STRUCT_HPP

#include <iostream>
#include <map>
#include <string>

#include <yarp/os/all.h>

namespace GenomYarp {


  /**
   *Typedef used when there is no input/output in the service. 
   */
  typedef struct VoidIO{
    int n;
  } VoidIO;

  typedef yarp::os::BufferedPort<yarp::os::Bottle> RequestPort;
  class RequestPortMap : public std::map<std::string, RequestPort*>
  {
    public:
	~RequestPortMap()
	{
	    for(iterator it = begin(); it != end(); ++it)
	      delete it->second;
	}
  };

}

#endif 
