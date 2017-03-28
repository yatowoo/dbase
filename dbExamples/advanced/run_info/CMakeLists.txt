# Create a library called "DbRunInfo" 

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${Boost_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/include/
${CMAKE_SOURCE_DIR}/dbase/dbExamples/advanced/run_info  
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
) 

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

link_directories( ${LINK_DIRECTORIES})

set(SRCS
FairDbRunInfoContFact.cxx
FairDbRunInfo.cxx
)

set(HEADERS
FairDbRunInfoContFact.h
FairDbRunInfo.h
${CMAKE_SOURCE_DIR}/dbase/dbInterface/FairDbReader.h
${CMAKE_SOURCE_DIR}/dbase/dbInterface/FairDbWriter.h
${CMAKE_SOURCE_DIR}/dbase/dbInterface/FairDbGenericParSet.h
)


set(LINKDEF FairDbRunInfoLinkDef.h)
set(LIBRARY_NAME DbRunInfo)
set(DEPENDENCIES
      Base ParBase FairDB boost_system boost_filesystem boost_regex
)

GENERATE_LIBRARY()



