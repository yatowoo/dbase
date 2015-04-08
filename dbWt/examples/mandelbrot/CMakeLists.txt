################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Date: 18.02.15 D.Bertini
# Create a library called "libFairDbWtExt" 

Message("-- Wt is FOUND .... compiling DbWt exxamples starting ... ")

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${ZMQ_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/parbase
${Wt_INCLUDE_DIR}
${Wt_INCLUDE_DIR}/Wt
${CMAKE_SOURCE_DIR}/dbase/DbWt
${CMAKE_SOURCE_DIR}/dbase/DbWt/treeview
${CMAKE_SOURCE_DIR}/dbase/DbWt/example/mandelbrot
)

include_directories(${INCLUDE_DIRECTORIES})

#if (Boost_FOUND)
#  configure_file( ${CMAKE_SOURCE_DIR}/example/Tutorial6/macros/run_sps_init.sh.in ${CMAKE_BINARY_DIR}/bin/run_sps_init.sh )
#  file(COPY  ${CMAKE_SOURCE_DIR}/example/Tutorial6/macros/dbconfig.sh DESTINATION ${CMAKE_BINARY_DIR}/bin/ )
#endIf (Boost_FOUND)

set(LINK_DIRECTORIES
    ${ROOT_LIBRARY_DIR}
    ${Boost_LIBRARY_DIRS}
) 

link_directories(${LINK_DIRECTORIES})

set(SRCS
MandelbrotExample.cxx
MandelbrotImage.cxx
PaintedImage.cxx
)

set(LIBRARY_NAME FairDbWtMandel)

if (Boost_FOUND)
  set(DEPENDENCIES
    Base FairDB ParBase 
    boost_thread boost_system boost_serialization boost_signals 
    boost_date_time  boost_regex boost_program_options boost_signals boost_random boost_system
    ${Wt_LIBRARY} ${Wt_HTTP_LIBRARY} ${Wt_LIBRARIES}
)
else (Boost_FOUND)
  # will not work anyway without boost !
  set(DEPENDENCIES Base FairDB ParBase 
    ${Wt_LIBRARY} ${Wt_HTTP_LIBRARY} 
)
endif (Boost_FOUND)

GENERATE_LIBRARY()

if (Boost_FOUND)

  set(Exe_Names
    mandelbrot.wt
  )

  set(Exe_Source
  mandelbrot.cxx    
  )

  list(LENGTH Exe_Names _length)
  math(EXPR _length ${_length}-1)

  foreach(_file RANGE 0 ${_length})
    list(GET Exe_Names ${_file} _name)
    list(GET Exe_Source ${_file} _src)
    set(EXE_NAME ${_name})
    set(SRCS ${_src})
    set(DEPENDENCIES FairDbWtMandel)
    GENERATE_EXECUTABLE()
  endforeach(_file RANGE 0 ${_length})
endif (Boost_FOUND)
