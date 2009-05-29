/********************************/
/* Copyright (c) 2007 LAAS/CNRS */
/* Maxime Ransan - oct 2007     */
/* All rights reserved.         */
/********************************/


#ifndef SERVICE_RQST_READER_HPP
#define SERVICE_RQST_READER_HPP


#include "YarpCodec.hpp"


namespace GenomYarp {
  class RqstReader;
  
  static  int RQST_CLIENT_NAME_POS  = 0;
  static  int RQST_RQST_ID_POS      = 1;
  static  int RQST_INPUT_POS        = 2;
}


class GenomYarp::RqstReader{
  
public:

  static std::string readClientName(yarp::os::Bottle* b)
  {
    std::string clientName;
    YarpCodec<std::string>::decode(b,clientName,RQST_CLIENT_NAME_POS);
    return clientName;
  }
  
  static int readRqstId(yarp::os::Bottle* b)
  {
    int rqstId;
    rqstId = b->get(RQST_RQST_ID_POS).asInt();
    return rqstId;
  }

  template<typename T> 
  static T readRqstInput(yarp::os::Bottle *b) 
  {
    T input;
    YarpCodec<T>::decode(b->get(RQST_INPUT_POS).asList(),input,0);
    return input;
  }
};







#endif
