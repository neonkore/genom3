cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

FIND_PACKAGE(OpenRTM)

IDL_TARGET(<!capCompName!>ControlIdl ${PROJECT_SOURCE_DIR}/ControlTask.idl)
<?
for t in comp.tasksMap():
  task = t.data()
  ?>
IDL_TARGET(<!capCompName!><!task.name!>Idl ${PROJECT_SOURCE_DIR}/I<!comp.name()!><!task.name!>.idl)
<?
?>

SET(<!capCompName!>_SRC 
      ${IDL_<!capCompName!>ControlIdl_OUTPUTS}
<?
for t in comp.tasksMap():
  task = t.data()
  ?>
      ${IDL_<!capCompName!><!task.name!>Idl_OUTPUTS}
      <!comp.name()!>Control.cpp 
      <!comp.name()!>ControlImpl.cpp  
      ExecServices.cpp
)

ADD_LIBRARY(<!capCompName!>Control SHARED ${<!capCompName!>_SRC})
TARGET_LINK_LIBRARIES( ${RTM_LIBRARIES})

SET(<!capCompName!>Comp_SRC <!comp.name()!>ControlComp.cpp)

ADD_EXECUTABLE(<!comp.name()!>Comp ${<!capCompName!>Comp_SRC})
TARGET_LINK_LIBRARIES(<!comp.name()!>Comp <!capCompName!>Control)
