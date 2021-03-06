 ################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             #
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# Date: 18.02.15 D.Bertini
# Create a library called "libCbmStsQaWt"

Message("-- Wt is FOUND .... compiling  CbmStsQaWt Project starting ... ")

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
   ${CMAKE_SOURCE_DIR}/dbase/dbWt/auth
   ${CMAKE_SOURCE_DIR}/dbase/dbWt/auth/model
   ${CMAKE_SOURCE_DIR}/dbase/dbInterface
   ${CMAKE_SOURCE_DIR}/dbase/dbValidation
   ${CMAKE_SOURCE_DIR}/dbase/dbInput
   ${CMAKE_SOURCE_DIR}/dbase/dbIO
   ${CMAKE_SOURCE_DIR}/dbase/dbUtils
   ${CMAKE_SOURCE_DIR}/dbase/dbExamples/cbm_sts/src
)

include_directories(${INCLUDE_DIRECTORIES})


set(LINK_DIRECTORIES
    ${ROOT_LIBRARY_DIR}
    ${Boost_LIBRARY_DIRS}
)

link_directories(${LINK_DIRECTORIES})

set(SRCS
  CbmStsQaWApplication.cxx
  CbmStsQaWMainWidget.cxx
  CbmStsQaWTreeView.cxx
  CbmStsQaWParTreeTable.cxx
  CbmStsQaWParTreeTableNode.cxx
  CbmStsQaSensorDataModel.cxx
  CbmStsQaSensorDataModelNew.cxx
  CbmStsQaSensorTableView.cxx
  CbmStsQaSensorTableViewNew.cxx
  CbmStsQaSensorEditDialog.cxx
  CbmStsQaSensorIvModel.cxx
  CbmStsQaSensorIvView.cxx
  CbmStsQaIvChartConfig.cxx
  CbmStsQaIvChartView.cxx
  CbmStsQaIvPanelList.cxx
  CbmStsQaSensorGeoView.cxx
  CbmStsQaSensorGeoDataModel.cxx
  CbmStsQaSensorResultsView.cxx
  CbmStsQaSensorResultsDataModel.cxx
  CbmStsQaSensorOwnerView.cxx
  CbmStsQaSensorOwnerDataModel.cxx
  CbmStsQaChMapChartView.cxx
)

set(LIBRARY_NAME CbmStsQaWt)

if (Boost_FOUND)
  set(DEPENDENCIES
    RHTTP
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
    cbmstsqawt.wt
  )

  set(Exe_Source
    cbmstsqawt.cxx
  )

  list(LENGTH Exe_Names _length)
  math(EXPR _length ${_length}-1)

  foreach(_file RANGE 0 ${_length})
    list(GET Exe_Names ${_file} _name)
    list(GET Exe_Source ${_file} _src)
    set(EXE_NAME ${_name})
    set(SRCS ${_src})
    set(DEPENDENCIES CbmStsQaWt CbmStsDbQa FairDbWt)
    GENERATE_EXECUTABLE()
  endforeach(_file RANGE 0 ${_length})
endif (Boost_FOUND)
