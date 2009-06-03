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
  static  int RQST_RQST_NAME_POS    = 1;
  static  int RQST_RQST_ID_POS      = 2;
  static  int RQST_INPUT_POS        = 3;
}


class GenomYarp::RqstReader{
  
public:

  static std::string readClientName(const yarp::os::Bottle& b)
  {
    std::string clientName;
    YarpCodec<std::string>::decode(&b,clientName,RQST_CLIENT_NAME_POS);
    return clientName;
  }

  static std::string readRequestName(const yarp::os::Bottle& b)
  {
    std::string requestName;
    YarpCodec<std::string>::decode(&b,requestName,RQST_RQST_NAME_POS);
    return requestName;
  }
  
  static int readRqstId(const yarp::os::Bottle& b)
  {
    return b.get(RQST_RQST_ID_POS).asInt();
  }

  template<typename T> 
  static T readRqstInput(const yarp::os::Bottle& b) 
  {
    T input;
    yarp::os::Bottle *c = b.get(RQST_INPUT_POS).asList();
    if(c)
      YarpCodec<T>::decode(c,input,0);
    else
      throw YarpCodecExceptionBlob();
    return input;
  }
};

#endif
