 # Date: 20.05.16 D.Bertini
# Create a library called "libFairDbDS" 
#
# All SQL drivers "includes" and "libraries" are not imeediately set 
# because of the problem i have with CMAKE and variable set to not-found
# Instead  those are added only if they have been previously set  


set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${ROOT_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/dbase/
${CMAKE_SOURCE_DIR}/dbase/dbDs
${CMAKE_SOURCE_DIR}/dbase/dbDS/core
${CMAKE_SOURCE_DIR}/dbase/dbDS/utils
${CMAKE_SOURCE_DIR}/dbase/dbDS/procs
)

include_directories(${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
    ${ROOT_LIBRARY_DIR}
    ${Boost_LIBRARY_DIRS}
) 

link_directories(${LINK_DIRECTORIES})

set(SRCS
  core/FairDbLogFile.cxx
)


CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")
set(HEADERS ${HEADERS})
set(LINKDEF FairDbDSLinkDef.h)
set(LIBRARY_NAME FairDbDS)

if (Boost_FOUND)
  set(DEPENDENCIES
    Base FairDB ParBase FairMQ ${ZMQ_LIBRARY_SHARED} boost_thread boost_system boost_serialization)
else (Boost_FOUND)
  set(DEPENDENCIES Base  FairDB ParBase)
endif (Boost_FOUND)

GENERATE_LIBRARY()

#if (Boost_FOUND)
#  set(Exe_Names
#    db_dispatcher
#    db_client 
#    db_worker
# )


#  set(Exe_Source
#    procs/db_dispatcher.cxx		
#    procs/db_client.cxx
#    procs/db_worker.cxx
#    )

 # list(LENGTH Exe_Names _length)
 # math(EXPR _length ${_length}-1)

  #foreach(_file RANGE 0 ${_length})
  #  list(GET Exe_Names ${_file} _name)
  #  list(GET Exe_Source ${_file} _src)
  #  set(EXE_NAME ${_name})
  #  set(SRCS ${_src})
  #  set(DEPENDENCIES FairDB ParBase FairDbMQ)
  #  GENERATE_EXECUTABLE()
  #endforeach(_file RANGE 0 ${_length})
#endif (Boost_FOUND)
