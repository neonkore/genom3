/********************************/
/* Copyright (c) 2007 LAAS/CNRS */
/* Maxime Ransan - oct 2007     */
/* All rights reserved.         */
/********************************/


#ifndef DATA_SERVER_HPP
#define DATA_SERVER_HPP

#include "Header.hpp"
#include <stdexcept>

namespace GenomYarp{ 
  
  class DataServerException : public runtime_error{
    
  protected:
    std::string clientName;
    std::string cmpntName ;
    std::string dataName  ;
  
  public:
    DataServerException(const std::string&  _clientName, 
			const std::string&  _cmpntName ,
			const std::string&  _dataName 
			) : runtime_error("DataServerException"){
      clientName = _clientName;
      cmpntName  = _cmpntName ;
      dataName   = _dataName  ;
    }
    virtual const char* what() const throw() = 0;
    
    virtual ~DataServerException() throw(){};
  };
  
  class DataServerExceptionOpen : public DataServerException{
    
  
  public:
    DataServerExceptionOpen(const std::string&  _clientName, 
			    const std::string&  _cmpntName , 
			    const std::string&  _dataName 
			    ) : DataServerException(_clientName,_cmpntName,_dataName){}
    
    virtual const char* what() const throw(){
      char* ret = new char[clientName.size() + cmpntName.size() + dataName.size() + 128];
      sprintf(ret,"error opening data server (data-name: %s) port on client %s for component %s ",
	      dataName.c_str(),
	      clientName.c_str(),
	      cmpntName.c_str());
      return ret;
    }
    virtual ~DataServerExceptionOpen() throw(){};
  };
  
  
  class DataCondition : public ACE_Condition<ACE_Recursive_Thread_Mutex> {
  public:
    
    DataCondition(ACE_Recursive_Thread_Mutex &mutex)
      :
      ACE_Condition<ACE_Recursive_Thread_Mutex> (mutex)
    {}
  };



  template <class T_DATA>
  class DataServer 
    : public 
    yarp::os::BufferedPort<yarp::os::Bottle>{
    
  protected:
    
    T_DATA*             data;

    yarp::os::Semaphore dataSem;
    
    std::string   dataName;
    
    std::string   clEntityName;
    std::string   seEntityName;

    std::auto_ptr<DataCondition> dataCondition;
    
  public:
    DataServer (const std::string& _dataName,
		const std::string& _clEntityName,
		const std::string& _seEntityName) 
      :
      yarp::os::BufferedPort<yarp::os::Bottle>(){
      
      data       = new T_DATA()   ;
      
      dataName     = _dataName    ;
      clEntityName = _clEntityName;
      seEntityName = _seEntityName;
      
      /**
       *URUS naming convention
       */
      
      std::string clientDataPortName = cmpnt::PortNamingConvention::clientDataFeedInput(clEntityName,
											seEntityName,
											dataName);
      
      if (!this->open(clientDataPortName.c_str())){
	throw DataServerExceptionOpen(clientName,cmpntName,dataName);
      }  
      
      if (!yarp::os::Network::connect(cmpntDataPortName.c_str(),clientDataPortName.c_str())){
	throw DataServerExceptionConnect(clientName,cmpntName,dataName);
      }
      
      ACE_Recursive_Thread_Mutex *mutex  = new ACE_Recursive_Thread_Mutex();
      dataCondition.reset(new DataCondition(*mutex));
	
      this->useCallback()        ;
    };
    

    
    virtual void onRead(yarp::os::Bottle& b){
      
      dataSem.wait();
      YarpCodec<T_DATA>::decode(&b,*data,0);
      dataSem.post();
      
      dataCondition->broadcast();
    }
    
    virtual void printData(){
      std::cout << "data-feed: " << dataName << std::endl;
      dataSem.wait();
      cmpnt::YarpCodec<T_DATA>::print(*data);
      std::cout << std::endl;
      dataSem.post();
    }
    
    
    int getLatestData(T_DATA& d){
      
      dataSem.wait();
      d  = *data; 
      dataSem.post();
      
      return 0;
    }

    int waitForNewData(T_DATA& d){
      
      ACE_Recursive_Thread_Mutex& _mutex = dataCondition->mutex();
      _mutex.acquire();
      dataCondition->wait();
      _mutex.release();
      
      dataSem.wait();
      d  = *data; 
      dataSem.post();
      
      return 0;
    }

    
    T_DATA& getLatestData(){
      T_DATA* ret = new T_DATA();
      
      dataSem.wait();
      *ret  = *data; 
      dataSem.post();
      
      return *ret;
    }

    T_DATA& waitForNewData(){
      T_DATA* ret = new T_DATA();
      
      ACE_Recursive_Thread_Mutex& _mutex = dataCondition->mutex();
      _mutex.acquire();
      dataCondition->wait();
      _mutex.release();
      
      dataSem.wait();
      *ret  = *data; 
      dataSem.post();

      return *ret;
    }

  };

}

#endif
