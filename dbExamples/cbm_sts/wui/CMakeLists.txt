cmake_minimum_required(VERSION 2.6)
project(wui)

enable_testing()

set(VERSION_MAJOR 3)
set(VERSION_MINOR 0)
set(VERSION_PATCH 9)

set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(ADMIN_EMAIL d.bertini@gsi.de CACHE STRING "email of admin")


file(GLOB wui_sources
    "${PROJECT_SOURCE_DIR}/src/*.cxx"
    "${PROJECT_SOURCE_DIR}/src/*/*.cxx"
    "${PROJECT_SOURCE_DIR}/src/*/*/*.cxx"
)

file(GLOB wui_headers
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*/*.h"
    "${PROJECT_SOURCE_DIR}/src/*/*/*.h"
)

subdirs(src)
#subdirs(locales)
#subdirs(files)

option(BUILD_DOC "Build documentation" ON)
if(BUILD_DOC)
    subdirs(doc)
endif()

#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/thechess-run
#    ${CMAKE_CURRENT_BINARY_DIR}/thechess-run)
