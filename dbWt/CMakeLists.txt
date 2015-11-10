 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Date: 18.02.15 D.Bertini
# Create a library called "libFairDbWt" 

Message("-- Wt is FOUND .... compiling DbWt Project starting ... ")

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${ZMQ_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/parbase
${Wt_INCLUDE_DIR}
${Wt_INCLUDE_DIR}/Wt
${CMAKE_SOURCE_DIR}/dbase/dbWt
${CMAKE_SOURCE_DIR}/dbase/dbWt/treeview
${CMAKE_SOURCE_DIR}/dbase/dbWt/graphics
${CMAKE_SOURCE_DIR}/dbase/dbWt/examples/filetree
${CMAKE_SOURCE_DIR}/dbase/dbWt/examples/mandelbrot
${CMAKE_SOURCE_DIR}/dbase/dbInterface
${CMAKE_SOURCE_DIR}/dbase/dbValidation
${CMAKE_SOURCE_DIR}/dbase/dbInput
${CMAKE_SOURCE_DIR}/dbase/dbIO
${CMAKE_SOURCE_DIR}/dbase/dbUtils
${CMAKE_SOURCE_DIR}/example/Tutorial5/src
)

include_directories(${INCLUDE_DIRECTORIES})

if (Boost_FOUND)
  Message("copy file ")
  file(COPY  ${CMAKE_SOURCE_DIR}/dbase/dbExamples/cbm_sts/macros/dbconfig.sh DESTINATION ${CMAKE_BINARY_DIR}/bin/ )
endIf (Boost_FOUND)

set(LINK_DIRECTORIES
    ${ROOT_LIBRARY_DIR}
    ${Boost_LIBRARY_DIRS}
) 

link_directories(${LINK_DIRECTORIES})

set(SRCS
FairDbWtApplication.cxx
FairDbWtMainWidget.cxx
FairDbWtConnMySQLWidget.cxx
FairDbWtItemsTpl.cxx
FairDbWtParFactory.cxx
treeview/FairDbWtFileModel.cxx
treeview/FairDbWtFileEditDialog.cxx
treeview/FairDbWtTreeView.cxx
treeview/FairDbWtParTreeTable.cxx
treeview/FairDbWtParTreeTableNode.cxx
treeview/FairDbWtPaintedImage.cxx
treeview/CsvUtil.cxx
treeview/FolderView.cxx
graphics/FairDbWtTCanvas.cxx
graphics/FairDbWtTCanvasWidget.cxx
graphics/FairDbWtRootBrowser.cxx
auth/model/User.C
auth/model/Session.C
)

set(LIBRARY_NAME FairDbWt)

if (Boost_FOUND)
  set(DEPENDENCIES
    Base FairDB ParBase 
    boost_thread boost_system boost_serialization boost_signals 
    boost_date_time  boost_regex boost_program_options boost_signals boost_random boost_system
    ${Wt_LIBRARY} ${Wt_HTTP_LIBRARY} ${Wt_LIBRARIES}
)
else (Boost_FOUND)
  # will not work anyway without boost !
  set(DEPENDENCIES Base FairDB ParBase Tutorial5 
    ${Wt_LIBRARY} ${Wt_HTTP_LIBRARY} 
)
endif (Boost_FOUND)

GENERATE_LIBRARY()

if (Boost_FOUND)

  set(Exe_Names
    fairdbwt.wt
  )

  set(Exe_Source
    fairdbwt.cxx    
  )

  list(LENGTH Exe_Names _length)
  math(EXPR _length ${_length}-1)

  foreach(_file RANGE 0 ${_length})
    list(GET Exe_Names ${_file} _name)
    list(GET Exe_Source ${_file} _src)
    set(EXE_NAME ${_name})
    set(SRCS ${_src})
    set(DEPENDENCIES FairDbWt Tutorial5)
    GENERATE_EXECUTABLE()
  endforeach(_file RANGE 0 ${_length})
endif (Boost_FOUND)

#add_subdirectory(examples)


