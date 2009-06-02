/********************************/
/* Copyright (c) 2007 LAAS/CNRS */
/* Maxime Ransan - oct 2007     */
/* All rights reserved.         */
/********************************/

#ifndef REPLY_READER_HPP
#define REPLY_READER_HPP

#include <iostream>
#include <yarp/os/all.h>

#include "Struct.hpp"

namespace GenomYarp {

  const int REPLY_CLIENT_NAME_POS  = 1;
  const int REPLY_SERVICE_NAME_POS = 2;
  const int REPLY_RQST_ID_POS      = 3;
  const int REPLY_REPORT_POS       = 4;
  const int REPLY_OUTPUT_POS       = 5;

/**
 *Template class used to read replies over the network.
 */	
class ReplyReader { 

  
public:

  static std::string readClientName(const yarp::os::Bottle* b){
    std::string entityName ;
    YarpCodec<std::string>::decode(b,entityName,REPLY_CLIENT_NAME_POS);
    return entityName;
  }
  

  static std::string readRequestName(const yarp::os::Bottle* b){
    std::string clientName ;
    YarpCodec<std::string>::decode(b,clientName,REPLY_SERVICE_NAME_POS);
    return clientName;
  }
  
  static int readRqstID(const yarp::os::Bottle* b){
    int rqstId = b->get(REPLY_RQST_ID_POS).asInt();
    return rqstId;
  }

  static std::string  readReport(const yarp::os::Bottle *b){
    std::string report;
    YarpCodec<std::string>::decode(b,report,REPLY_REPORT_POS);
    return report;
  }

  template<typename T_input>
  static T_input readInput(const yarp::os::Bottle *b) {
	  T_input input;
	  YarpCodec<T_input>::decode(b->get(REPLY_OUTPUT_POS).asList(), input, 0);
	  return input;
  }
};

class BadAnswerException {
	private:
		const char * why;

	public: 
		BadAnswerException(const char * why_) : why(why_) {};

		virtual const char* what() const throw()
		{
			return why;
		};
};


template <typename input_type>
class ReplyAnswer { 
	private:
		std::string entityName;
		std::string clientName;
		int rqstId;
		input_type input;
		std::string report;

	public:
		ReplyAnswer(const yarp::os::Bottle *b) {
			// All this code can throw exception if the packet is not valide
			entityName = cmpnt::ReplyReader::readEntityName(b);
			clientName = cmpnt::ReplyReader::readClientName(b);
			rqstId = cmpnt::ReplyReader::readRqstID(b);
			report = cmpnt::ReplyReader::readReport(b);
			input = cmpnt::ReplyReader::readInput<input_type> (b);
		};

		const std::string & get_client_name() const {
			return clientName;
		};

		int get_rqst_id() const {
		   return rqstId;
		};

		const input_type & get_input() const {
			return input;
		};

		const std::string & get_report() const {
			return report;
		};

		bool is_valid() const {
			return (report == "OK");
		}
};


template <typename input_type>
std::ostream& operator << (std::ostream& oss, const ReplyAnswer<input_type>& ans)
{
	oss << "( RPLY ) ";
	oss << "(entity-name "   << ans.get_entity_name() << ") ";
	oss << "(client-name "   << ans.get_client_name() << ") ";
	oss << "(rqst-id "       << ans.get_rqst_id() << ") ";
	oss << "(input "		<< ans.get_input() << ") ";
	oss << "(report "   << ans.get_report() << ") ";
	return oss;
}

}

#endif
