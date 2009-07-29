 
# - Try to find YARP2
# Once done this will define
#
#  YARP2_FOUND - system has YARP2
#  YARP2_INCLUDE_DIRS - the YARP2 include directory
#  YARP2_LIBRARIES - Link these to use YARP2
#  YARP2_DEFINITIONS - Compiler switches required for using YARP2
#
#  Copyright (c) 2008 Stefán Freyr Stefánsson <[EMAIL PROTECTED]>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (YARP2_LIBRARIES AND YARP2_INCLUDE_DIRS)
  # in cache already
  set(YARP2_FOUND TRUE)
else (YARP2_LIBRARIES AND YARP2_INCLUDE_DIRS)
  find_path(YARP2_INCLUDE_DIR
    NAMES
      yarp/os/all.h yarp/dev/all.h yarp/sig/all.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
  )

  find_library(YARP_OS_LIBRARY
    NAMES
      YARP_OS
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )
  find_library(YARP_DEV_LIBRARY
    NAMES
      YARP_dev
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )
  find_library(YARP_SIG_LIBRARY
    NAMES
      YARP_sig
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )

  if (YARP_OS_LIBRARY)
    set(YARP_OS_FOUND TRUE)
  endif (YARP_OS_LIBRARY)
  if (YARP_DEV_LIBRARY)
    set(YARP_DEV_FOUND TRUE)
  endif (YARP_DEV_LIBRARY)
  if (YARP_SIG_LIBRARY)
    set(YARP_SIG_FOUND TRUE)
  endif (YARP_SIG_LIBRARY)

  set(YARP2_INCLUDE_DIRS
    ${YARP2_INCLUDE_DIR}
  )

  if (YARP_OS_FOUND)
    set(YARP2_LIBRARIES
      ${YARP2_LIBRARIES}
      ${YARP_OS_LIBRARY}
    )
  endif (YARP_OS_FOUND)
  if (YARP_DEV_FOUND)
    set(YARP2_LIBRARIES
      ${YARP2_LIBRARIES}
      ${YARP_DEV_LIBRARY}
    )
  endif (YARP_DEV_FOUND)
  if (YARP_SIG_FOUND)
    set(YARP2_LIBRARIES
      ${YARP2_LIBRARIES}
      ${YARP_SIG_LIBRARY}
    )
  endif (YARP_SIG_FOUND)

  if (YARP2_INCLUDE_DIRS AND YARP2_LIBRARIES)
     set(YARP2_FOUND TRUE)
  endif (YARP2_INCLUDE_DIRS AND YARP2_LIBRARIES)

  if (YARP2_FOUND)
    if (NOT YARP2_FIND_QUIETLY)
      message(STATUS "Found YARP2: ${YARP2_LIBRARIES}")
    endif (NOT YARP2_FIND_QUIETLY)
  else (YARP2_FOUND)
    if (YARP2_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find YARP2")
    endif (YARP2_FIND_REQUIRED)
  endif (YARP2_FOUND)

  # show the YARP2_INCLUDE_DIRS and YARP2_LIBRARIES variables only in the  advanced view
  mark_as_advanced(YARP2_INCLUDE_DIRS YARP2_LIBRARIES)

endif (YARP2_LIBRARIES AND YARP2_INCLUDE_DIRS)
