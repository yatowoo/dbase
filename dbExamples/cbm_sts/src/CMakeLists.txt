# Create a library called "CbmStsDbQa" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.


Set(INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${CMAKE_SOURCE_DIR}/dbase/dbExamples/cbm_sts/src
)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIR}
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)
 
Link_Directories(${LINK_DIRECTORIES})

set(SRCS
CbmStsDbQaContFact.cxx
CbmStsDbQaIvPar.cxx
CbmStsDbQaSensorPar.cxx
CbmStsDbQaPar.cxx
)

set(HEADERS
CbmStsDbQaContFact.h
CbmStsDbQaIvPar.h
CbmStsDbQaSensorPar.h
CbmStsDbQaPar.h
${CMAKE_SOURCE_DIR}/dbase/dbInterface/FairDbReader.h
${CMAKE_SOURCE_DIR}/dbase/dbInterface/FairDbWriter.h
)


set(LINKDEF CbmStsDbQaLinkDef.h)
Set(LIBRARY_NAME CbmStsDbQa)
Set(DEPENDENCIES Base ParBase FairDB boost_system boost_filesystem boost_regex)

GENERATE_LIBRARY()





