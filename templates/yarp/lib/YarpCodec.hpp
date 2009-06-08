/********************************/
/* Copyright (c) 2007 LAAS/CNRS */
/* Maxime Ransan - oct 2007     */
/* All rights reserved.         */
/********************************/


#ifndef CMPNT_YARP_CODEC_HPP
#define CMPNT_YARP_CODEC_HPP


#include "Struct.hpp"

#include <stdexcept>
#include <string>
#include <string.h>

namespace GenomYarp {

  class YarpCodecException : public std::runtime_error {
    
  protected:
    std::string data;
  public:
    YarpCodecException() : runtime_error("YarpCodecException"){
      data = ">>Expecting Unknown<< ";
    }
    
    virtual const char* what() const throw(){
      char* ret = new char[data.size() + 128];
      sprintf(ret,"Data Serialization Error:\n %s",
	      data.c_str());
      return ret;
    }
    
    virtual void add(const std::string var_name,const std::string var_type){
      data += "\n-> " + var_name + " (" + var_type + ")";
    } 
    
    
    virtual ~YarpCodecException() throw(){};
  };
  
  class YarpCodecExceptionInt : public YarpCodecException{
  public:
    YarpCodecExceptionInt() : YarpCodecException(){
      data = ">>Expecting Int<<";
    };
    
    virtual ~YarpCodecExceptionInt() throw(){};
  };
  
  
  class YarpCodecExceptionDouble : public YarpCodecException{
  private:
    std::string type;
  public:
    YarpCodecExceptionDouble() : YarpCodecException(){
      data = ">>Expecting Double<<";
    };
    
    virtual ~YarpCodecExceptionDouble() throw(){};
  };

  class YarpCodecExceptionString : public YarpCodecException{
  private:
    std::string type;
  public:
    YarpCodecExceptionString() : YarpCodecException(){
      data = ">>Expecting string<<";
    };
    
    virtual ~YarpCodecExceptionString() throw(){};
  };

  class YarpCodecExceptionBlob : public YarpCodecException{
  private:
    std::string type;
  public:
    YarpCodecExceptionBlob() : YarpCodecException(){
      data = ">>Expecting Blob<<";
    };
    
    virtual ~YarpCodecExceptionBlob() throw(){};
  };
  

  
  template<class T_STRUCT>
  class YarpCodec{

  public:
    
    static int  encode (yarp::os::Bottle *b, const T_STRUCT& v);
    static int  decode (const yarp::os::Bottle *b, T_STRUCT& v,int it); 
    static int  print  (const T_STRUCT& v);

  };

  
  template <> class YarpCodec<int>{
    
  public:
    static int encode  (yarp::os::Bottle* b,const int& v){
      b->addInt(v);
      return 0;
    }
    static int decode (const yarp::os::Bottle *b,int& v,int it){
      if (!b->get(it).isInt())
	throw YarpCodecExceptionInt();
      
      v = b->get(it).asInt();
      it++;
      return it;
    }
    static int print  (const int &v){
      std::cout << v ;
      return 0;
    }
    
  };

  template <> class YarpCodec<unsigned int>{
    
  public:
    static int encode  (yarp::os::Bottle* b,const unsigned int& v){
      b->addInt(v);
      return 0;
    }
    static int decode (const yarp::os::Bottle *b, unsigned int& v,int it){
      if (!b->get(it).isInt())
	throw YarpCodecExceptionInt();
      
      v = b->get(it).asInt();
      it++;
      return it;
    }
    static int print  (const unsigned int &v){
      std::cout << v ;
      return 0;
    }
    
  };
  
  template <> class YarpCodec<char>{
    
  public:
    static int encode  (yarp::os::Bottle* b,const char& v){
      b->addInt(v);
      return 0;
    }
    static int decode (const yarp::os::Bottle *b,char& v,int it){
      if (!b->get(it).isInt())
	throw YarpCodecExceptionInt();
      
      v = (char)b->get(it).asInt();
      it++;
      return it;
    }
    static int print  (const char &v){
      std::cout << v ;
      return 0;
    }
    
  };

  template <> class YarpCodec<VoidIO>{
    
  public:
    static int encode  (yarp::os::Bottle* b,const VoidIO& v){
      return 0;
    }
    static int decode (const yarp::os::Bottle *b,VoidIO& v,int it){
      return it;
    }
    static int print  (const VoidIO &v){
      std::cout << "VoidIO" ;
      return 0;
    }
    
  };

  
  template <> class YarpCodec<double>{
  public:
    static int encode (yarp::os::Bottle* b,const double& v){
      b->addDouble(v);
      return 0;
    }
    static int decode (const yarp::os::Bottle *b,double& v,int it){
      if (!b->get(it).isDouble())
	throw YarpCodecExceptionDouble();

      v = b->get(it).asDouble();
      it++;
      return it;
    }
    
    static int print  (const double& v){
      std::cout << v ;
	  return 0;
    }
  };

  template <> class YarpCodec<float>{
  public:
    static int encode (yarp::os::Bottle* b,const float& v){
      b->addDouble(v);
      return 0;
    }
    static int decode (const yarp::os::Bottle *b,float& v,int it){
      if (!b->get(it).isDouble())
	throw YarpCodecExceptionDouble();

      v = (float) b->get(it).asDouble();
      it++;
      return it;
    }
    
    static int print  (const float& v){
      std::cout << v ;
	  return 0;
    }
  };
  
  template<> class YarpCodec <std::string> {
  public:

    static    int encode(yarp::os::Bottle* b, const std::string& v){
      b->addString(v.c_str());
      return 0;
    } 

    static    int encode(yarp::os::Bottle* b, const char* v){
      b->addString(v);
      return 0;
    } 
    
    static    int decode(const yarp::os::Bottle *b,std::string& v, int it){
      if (!b->get(it).isString())
	throw YarpCodecExceptionString();
      
      v = b->get(it).asString().c_str();
      it++;
      return it;
    }

    static    int decode(const yarp::os::Bottle *b,char *v, int it){
      if (!b->get(it).isString())
	throw YarpCodecExceptionString();
      
      yarp::os::ConstString s = b->get(it).asString();
      strncpy(v, s.c_str(),s.length()+1);
      it++;
      return it;
    }
    
    static int print  (const std::string& v){
      std::cout << v ;
      return 0;
    }
      
  };

}

#endif
