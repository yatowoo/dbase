# Create a library called "Advanced" 

set(INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
${Boost_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/dbase/dbExamples/advanced/src  
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
) 

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

link_directories( ${LINK_DIRECTORIES})

set(SRCS
FairDbExPar1.cxx
FairDbExPar2.cxx
FairDbExPar3.cxx
FairDbExContFact.cxx
FairDbTests.cxx
)

set(HEADERS
FairDbExPar1.h
FairDbExPar2.h
FairDbExPar3.h  
FairDbExContFact.h
FairDbTests.h
${CMAKE_SOURCE_DIR}/dbase/dbInterface/FairDbReader.h
${CMAKE_SOURCE_DIR}/dbase/dbInterface/FairDbWriter.h
${CMAKE_SOURCE_DIR}/dbase/dbInterface/FairDbGenericParSet.h
)


set(LINKDEF FairDbExLinkDef.h)
set(LIBRARY_NAME DBAdvanced)
set(DEPENDENCIES
      Base ParBase FairDB boost_system boost_filesystem boost_regex
)

GENERATE_LIBRARY()



