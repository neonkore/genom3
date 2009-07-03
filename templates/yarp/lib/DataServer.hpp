/********************************/
/* Copyright (c) 2007 LAAS/CNRS */
/* Maxime Ransan - oct 2007     */
/* All rights reserved.         */
/********************************/


#ifndef DATA_SERVER_HPP
#define DATA_SERVER_HPP

#include <auto_ptr.h>
#include <stdexcept>
#include <ace/Condition_T.h>
#include <ace/Synch.h>

#include "Header.hpp"

using namespace std;

namespace GenomYarp{ 
  
  class DataCondition : public ACE_Condition<ACE_Recursive_Thread_Mutex> {
  public:
    
    DataCondition(ACE_Recursive_Thread_Mutex &mutex)
      :
      ACE_Condition<ACE_Recursive_Thread_Mutex> (mutex)
    {}
  };



  template <class T_DATA>
  class DataServer 
    : public yarp::os::BufferedPort<yarp::os::Bottle>, 
      public EventSender
{    
  protected:
    T_DATA*             data;
    yarp::os::Semaphore dataSem;
    std::auto_ptr<DataCondition> dataCondition;
    
  public:
    DataServer() 
      : yarp::os::BufferedPort<yarp::os::Bottle>()
    {
      
      data = 0;
    };
    
    bool open(const char* name)
    {
      if (!yarp::os::BufferedPort<yarp::os::Bottle>::open(name)){
// 	throw DataServerExceptionOpen(clientName,cmpntName,dataName);
	return false;
      }  
      
      ACE_Recursive_Thread_Mutex *mutex  = new ACE_Recursive_Thread_Mutex();
      dataCondition.reset(new DataCondition(*mutex));
	
      this->useCallback()        ;
    }

    
    virtual void onRead(yarp::os::Bottle& b)
    {      
      dataSem.wait();
      if(!data)
	data = new T_DATA;
      else // deallocate previously used memory
	YarpCodec<T_DATA>::freeAllocatedMemory(data);

      YarpCodec<T_DATA>::decode(&b,*data,0);
      dataSem.post();
      
      dataCondition->broadcast();
      sendEvent("onUpdate");
    }
    
    virtual void printData(){
      dataSem.wait();
      YarpCodec<T_DATA>::print(*data);
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

    void wait()
    {
	dataSem.wait();
    }

    void post()
    {
	dataSem.post();
    }

    T_DATA* getDataPtr()
    {
	sendEvent("onRead");
	return data;
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
