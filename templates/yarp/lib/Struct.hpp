/********************************/
/* Copyright (c) 2007 LAAS/CNRS */
/* Maxime Ransan - oct 2007     */
/* All rights reserved.         */
/********************************/


#ifndef CMPNT_STRUCT_HPP
#define CMPNT_STRUCT_HPP

#include <iostream>
#include <yarp/os/all.h>

#include <list>



namespace GenomYarp {


  /**
   *Typedef used when there is no input/output in the service. 
   */
  typedef struct cmpntVoid{
    int n;
  }cmpntVoid;

};


#endif 
