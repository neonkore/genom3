/********************************/
/* Copyright (c) 2007 LAAS/CNRS */
/* Maxime Ransan - oct 2007     */
/* All rights reserved.         */
/********************************/


#ifndef DATA_EXPORTER_HPP
#define DATA_EXPORTER_HPP

#include <YarpCodec.hpp>

namespace GenomYarp {

template<class T_DATA>
class OutPort :public cmpnt::GenericExporter {

public:  
  OutPort() {}
  
  open(const std::string &portName)
  {
    out_port.open(portName.c_str());
  }

  virtual ~OutPort() {}
  

  /**
   *Set the data Pt. Watch Out the type of the Data must T_DATA
   *@param _data: the pointer to the T_DATA to be exported
   @return 0;
  */
  int setData(T_DATA *_data)
  {
    data =  _data;
    return 0;
  }
  
  /**
   *Export the Data through the yarp port created by the constructor;
   *@return 0;
   */
  int exportData()
  {
    yarp::os::Bottle& b = out_port.prepare();
    b.clear();
    cmpnt::YarpCodec<T_DATA>::encode(&b,
				     *data);
    out_port.writeStrict();
    return 0;
  }

  int exportData(T_DATA *data)
  {
    setData(data);
    eportData();
  }

  private:
    std::string portName;
    T_DATA* data;
};

}

#endif
