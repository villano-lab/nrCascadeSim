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
execute_process (
    COMMAND bash -c "cd ${PROJECT_SOURCE_DIR} ; git describe --abbrev=4 --always|xargs echo -n"
    OUTPUT_VARIABLE GIT_VERSION 
)
message("git version: ${GIT_VERSION}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__GIT_VERSION=\"\\\"${GIT_VERSION}\\\"\"")
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
if(NOT CMAKE_INSTALL_PREFIX)
    SET(CMAKE_INSTALL_PREFIX ~/)
endif(NOT CMAKE_INSTALL_PREFIX)
if(NOT CMAKE_LIB_PREFIX)
    SET(CMAKE_LIB_PREFIX ${CMAKE_INSTALL_PREFIX})
endif(NOT CMAKE_LIB_PREFIX)
message("Install prefix: ${CMAKE_INSTALL_PREFIX}")
#SET(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/../lib)
# Make necessary bin/lib directory
# SET(runScript "$(shell mkdir -p 'bin/lib')") #hopefully don't need this anymore
# Send files to the right places
include_directories(${incDir})

#----------------------------------------------------------------------------
#Find ROOT

get_directory_property(_vars_before VARIABLES)
find_package(ROOT REQUIRED COMPONENTS RIO Net)
#SET(ROOT_LIB_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib)
get_directory_property(_vars VARIABLES)


if (ROOT_FOUND)
    message(STATUS "Libraries for ROOT found")
    message(STATUS "Package Version: " ${ROOT_VERSION})
    message(STATUS "Include DIRS: " ${ROOT_INCLUDE_DIRS})
    message(STATUS "Lib DIRS: " ${ROOT_LIBRARIES})
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
include_directories(${ROOT_INCLUDE_DIRS})

#And set the c++ standard to match whatever was used for root

execute_process(COMMAND bash "-c" "root-config --cflags | awk '{for(i=1; i<=NF; i++){if(match($i, /^\-std\=c\+\+/)) print $i}}' | cut -c9-10" OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE CMAKE_CXX_STANDARD)
message("C++ Standard: ${CMAKE_CXX_STANDARD}. (Detected from ROOT.)")

#-----------------------------------------------------------------------------




#Get linker to work for make install
#list( APPEND CMAKE_INSTALL_RPATH ${CMAKE_LIB_PREFIX}/lib)
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_LIB_PREFIX}/lib" isSystemDir)
#if("${isSystemDir}" STREQUAL "-1")
#    list(APPEND CMAKE_INSTALL_RPATH "${CMAKE_LIB_PREFIX}/lib")
#endif("${isSystemDir}" STREQUAL "-1")

# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
list( APPEND CMAKE_INSTALL_RPATH ${ROOT_LIB_DIRS})

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")
message("RPATH for Install: ${CMAKE_INSTALL_RPATH}.")



#[[ no longer used
if(NOT TEST_OUTPUT_DIR)
    SET(TEST_OUTPUT_DIR ${testBin})
endif(NOT TEST_OUTPUT_DIR) #]]
message("Tests output directory: ${TEST_OUTPUT_DIR} (Default: ${testBin})")

file(GLOB sources ${srcDir}/*.cpp)
file(GLOB headers ${incDir}/*.h)

#----------------------------------------------------------------------------
#Coverage
if(NOT USE_GCOV)
    SET(USE_GCOV OFF)
endif(NOT USE_GCOV)
message("Use GCOV? ${USE_GCOV}")
if(USE_GCOV)
	SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
	SET(GCC_COVERAGE_LINK_FLAGS    "-coverage")
	SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
 	SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
    message("Set up coverage flags.")
endif(USE_GCOV)
message("CXX Flags: ${CMAKE_CXX_FLAGS}")
message("Linker Flags: ${CMAKE_EXE_LINKER_FLAGS}")

#----------------------------------------------------------------------------
#Main part


add_library(isotope_info ${srcDir}/isotope_info.cpp ${incDir}/isotope_info.h)
add_library(rootUtil ${srcDir}/rootUtil.cpp ${incDir}/rootUtil.h)
add_library(edepmath ${srcDir}/edepmath.cpp ${incDir}/edepmath.h)
add_library(weisskopf ${srcDir}/weisskopf.cpp ${incDir}/weisskopf.h)
add_library(lindhard ${srcDir}/lindhard.cpp ${incDir}/lindhard.h ${incDir}/edepmath.h)
add_library(cascadeProd ${srcDir}/cascadeProd.cpp ${incDir}/cascadeProd.h ${headers})

add_executable(regexPlayground ${binDir}/regexPlayground.cpp)

link_libraries(weisskopf lindhard cascadeProd edepmath rootUtil ${ROOT_LIBRARIES})
add_library(libncap SHARED ${srcDir}/isotope_info.cpp ${srcDir}/rootUtil.cpp ${srcDir}/edepmath.cpp ${srcDir}/weisskopf.cpp ${srcDir}/lindhard.cpp ${srcDir}/cascadeProd.cpp ${incDir}/isotope_info.h ${incDir}/rootUtil.h ${incDir}/edepmath.h ${incDir}/weisskopf.h ${incDir}/lindhard.h ${incDir}/cascadeProd.h)

link_libraries(libncap ${ROOT_LIBRARIES})

add_executable(realizeCascades ${binDir}/realizeCascades.cpp)
add_executable(fetchYieldModel EXCLUDE_FROM_ALL ${testBin}/fetchYieldModel.cpp)
add_executable(printIsotopeInfo EXCLUDE_FROM_ALL ${testBin}/printIsotopeInfo.cpp)
add_executable(readLevelfile EXCLUDE_FROM_ALL ${testBin}/readLevelfile.cpp)
add_executable(realizeAndSave EXCLUDE_FROM_ALL ${testBin}/realizeAndSave.cpp ${incDir}/cascadeProd.h)

install(TARGETS realizeCascades regexPlayground RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS libncap LIBRARY DESTINATION ${CMAKE_LIB_PREFIX}/lib)
#install(TARGETS fetchYieldModel printIsotopeInfo readLevelfile realizeAndSave DESTINATION ${testBin})

add_custom_target(tests 
                DEPENDS fetchYieldModel printIsotopeInfo readLevelfile realizeAndSave
)

add_custom_target(uninstall
                COMMAND xargs rm < install_manifest.txt 
)
