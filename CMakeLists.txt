###############################################################
#
#
# Makefile for nrCascadeSim
###############################################################
cmake_minimum_required(VERSION 3.10.2) #Just setting this to my version since I know that works.
project(nrCascadeSim)

# everybody here needs root
SET(rootFlags `root-config --cflags`)
#trick for getting the git version in the code
SET(gitVersion "$(shell sh -c 'git describe --abrev=4 --always')")
message("git version: ${GIT_VERSION}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__GIT_VERSION=\\\"${GIT_VERSION}\\\"")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__GIT_VERSION=\\\"${GIT_VERSION}\\\"")
# other variables
#SET(topDir $(shell pwd))    #obsolete - we will mark this using CMake
#SET(buildDir $(topDir)/bin) #obsolete - we will mark this using CMake
SET(incDirOut include)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/)
SET(testBin ${PROJECT_SOURCE_DIR}/tests/bin)
SET(incDir ${PROJECT_SOURCE_DIR}/inc/)
SET(srcDir ${PROJECT_SOURCE_DIR}/src/)
SET(binDir ${PROJECT_SOURCE_DIR}/bin/)
SET(CMAKE_STATIC_LIBRARY_SUFFIX ".o")
SET(CMAKE_STATIC_LIBRARY_PREFIX "")
SET(CMAKE_SHARED_LIBRARY_PREFIX "")
#SET(libFLAG "-L. -L$(CMAKE_LIBRARY_OUTPUT_DIRECTORY)")
SET(INSTALL_DIR /usr/local/bin)
SET(LIB_INSTALL_DIR /usr/local/lib)
# Make necessary bin/lib directory
# SET(runScript "$(shell mkdir -p 'bin/lib')") #hopefully don't need this anymore
# Send files to the right places
include_directories(${incDir})

#Get linker to work for make install
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")



file(GLOB sources ${srcDir}/*.cpp)
file(GLOB headers ${incDir}/*.h)

#Coverage
if(USE_GCOV)
	SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
	SET(GCC_COVERAGE_LINK_FLAGS    "-coverage")
	SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
 	SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
endif(USE_GCOV)

#----------------------------------------------------------------------------
#Find ROOT

#list(APPEND CMAKE_PREFIX_PATH "$ENV{ROOTSYS}") //only if ROOT made with CMake
list(APPEND CMAKE_MODULE_PATH "$ENV{ROOTSYS}/etc/cmake/")
list(APPEND CMAKE_PREFIX_PATH "$ENV{ROOTSYS}")
message(STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})
message(STATUS "CMAKE_MODULE_PATH: " ${CMAKE_PREFIX_PATH})
get_directory_property(_vars_before VARIABLES)
find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${ROOT_INCLUDE_DIRS})
get_directory_property(_vars VARIABLES)


if (ROOT_FOUND)
    message(STATUS "Libraries for ROOT found")
    message(STATUS "Package Version: " ${ROOT_VERSION})
    message(STATUS "Include DIRS: " ${ROOT_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Failed to find ROOT.")
endif()

#list(REMOVE_ITEM _vars _vars_before ${_vars_before})
#foreach(_var IN LISTS _vars)
#    message(STATUS "${_var} = ${${_var}}")
#endforeach()

#----------------------------------------------------------------------------
#Setup the ROOT include directories 

#include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIR})

#And set the c++ standard to match whatever was used for root

execute_process(COMMAND bash "-c" "root-config --cflags | awk '{print $2}' | cut -c9-10" OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE CMAKE_CXX_STANDARD)
message("C++ Standard: ${CMAKE_CXX_STANDARD}. (Detected from ROOT.)")

#-----------------------------------------------------------------------------
#Main part


add_library(isotope_info ${srcDir}/isotope_info.cpp ${incDir}/isotope_info.h)
add_library(rootUtil ${srcDir}/rootUtil.cpp ${incDir}/rootUtil.h)
add_library(edepmath ${srcDir}/edepmath.cpp ${incDir}/edepmath.h)
add_library(weisskopf ${srcDir}/weisskopf.cpp ${incDir}/weisskopf.h)
add_library(lindhard ${srcDir}/lindhard.cpp ${incDir}/lindhard.h ${incDir}/edepmath.h)
add_library(cascadeProd ${srcDir}/cascadeProd.cpp ${incDir}/cascadeProd.h ${headers})

add_executable(regexPlayground ${binDir}/regexPlayground.cpp)

link_libraries(weisskopf lindhard cascadeProd edepmath rootUtil)
add_library(libncap SHARED ${srcDir}/isotope_info.cpp ${srcDir}/rootUtil.cpp ${srcDir}/edepmath.cpp ${srcDir}/weisskopf.cpp ${srcDir}/lindhard.cpp ${srcDir}/cascadeProd.cpp ${incDir}/isotope_info.h ${incDir}/rootUtil.h ${incDir}/edepmath.h ${incDir}/weisskopf.h ${incDir}/lindhard.h ${incDir}/cascadeProd.h)

link_libraries(libncap ${ROOT_LIBRARIES})

add_executable(realizeCascades ${binDir}/realizeCascades.cpp)
add_executable(fetchYieldModel EXCLUDE_FROM_ALL ${testBin}/fetchYieldModel.cpp)
add_executable(printIsotopeInfo EXCLUDE_FROM_ALL ${testBin}/printIsotopeInfo.cpp)
add_executable(readLevelfile EXCLUDE_FROM_ALL ${testBin}/readLevelfile.cpp)
add_executable(realizeAndSave EXCLUDE_FROM_ALL ${testBin}/realizeAndSave.cpp ${incDir}/cascadeProd.h)

install(TARGETS realizeCascades regexPlayground DESTINATION ${INSTALL_DIR})
install(TARGETS libncap DESTINATION ${LIB_INSTALL_DIR})
#install(TARGETS fetchYieldModel printIsotopeInfo readLevelfile realizeAndSave DESTINATION ${testBin})

set_property(GLOBAL PROPERTY ADDITIONAL_CLEAN_FILES ${INSTALL_DIR}/realizeCascades ${INSTALL_DIR}/regexPlayground ${testBin}/fetchYieldModel ${testBin}/printIsotopeInfo ${testBin}/readLevelfile ${testBin}/realizeAndSave)

add_custom_target(tests 
                WORKING_DIRECTORY ${testBin}
                DEPENDS fetchYieldModel printIsotopeInfo readLevelfile realizeAndSave)