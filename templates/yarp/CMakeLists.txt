cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

FIND_PACKAGE(YARP)

include_directories(lib ${YARP2_INCLUDE_DIRS})

# Component shared library
SET(<!upperCompName!>_SRC 
      <!comp.name()!>Module.cpp 
      <!comp.name()!>YarpCodec.cpp  
      ExecServices.cpp
      <!comp.name()!>UserCntrlTaskCodels.c<?
for t in comp.tasksMap():
  ?>
      <!comp.name()!>User<!t.data().name!>Codels.c
      <!comp.name()!><!t.data().name!>.cpp
      lib/logger.c
      lib/Events.cpp
<?
?>
)

ADD_LIBRARY(<!capCompName!>Module SHARED ${<!upperCompName!>_SRC})
TARGET_LINK_LIBRARIES(<!capCompName!>Module ${YARP_OS_LIBRARY})

#  Standalone version of the component
SET(<!capCompName!>Comp_SRC <!comp.name()!>Comp.cpp)

ADD_EXECUTABLE(<!comp.name()!>Comp ${<!capCompName!>Comp_SRC})
TARGET_LINK_LIBRARIES(<!comp.name()!>Comp <!capCompName!>Module ${YARP_OS_LIBRARY})

# Test component
SET(<!capCompName!>Test_SRC 
    <!comp.name()!>Test.cpp
    <!comp.name()!>YarpCodec.cpp 
    lib/Events.cpp     
)

ADD_EXECUTABLE(<!comp.name()!>Test ${<!capCompName!>Test_SRC})
TARGET_LINK_LIBRARIES(<!comp.name()!>Test ${YARP_OS_LIBRARY})