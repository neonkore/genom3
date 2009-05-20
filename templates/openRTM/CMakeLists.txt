cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

FIND_PACKAGE(OpenRTM)

IDL_TARGET(<!capCompName!>ControlIdl ${PROJECT_SOURCE_DIR}/ControlTask.idl)

# Component shared library
SET(<!upperCompName!>_SRC 
      ${IDL_<!capCompName!>ControlIdl_OUTPUTS}
      <!comp.name()!>Control.cpp 
      <!comp.name()!>ControlImpl.cpp  
      ExecServices.cpp
      <!comp.name()!>UserCntrlTaskCodels.cpp<?
for t in comp.tasksMap():
  ?>
      <!comp.name()!>User<!t.data().name!>Codels.cpp
      <!comp.name()!><!t.data().name!>.cpp
<?
?>
)

ADD_LIBRARY(<!capCompName!>Control SHARED ${<!upperCompName!>_SRC})
TARGET_LINK_LIBRARIES(${RTM_LIBRARIES})

#  Standalone version of the component
SET(<!capCompName!>Comp_SRC <!comp.name()!>ControlComp.cpp)

ADD_EXECUTABLE(<!comp.name()!>Comp ${<!capCompName!>Comp_SRC})
TARGET_LINK_LIBRARIES(<!comp.name()!>Comp <!capCompName!>Control ${RTM_LIBRARIES})

# Test component
SET(<!capCompName!>TestComp_SRC 
    ${IDL_<!capCompName!>ControlIdl_OUTPUTS}
    <!comp.name()!>TestComp.cpp
    <!comp.name()!>Test.cpp
)

ADD_EXECUTABLE(<!comp.name()!>TestComp ${<!capCompName!>TestComp_SRC})
TARGET_LINK_LIBRARIES(<!comp.name()!>TestComp ${RTM_LIBRARIES})