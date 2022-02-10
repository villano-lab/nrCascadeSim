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
#RPATH shennanigans 



# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
list( APPEND CMAKE_INSTALL_RPATH ${ROOT_LIB_DIRS})
set(RPATH "${CMAKE_BINARY_DIR}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")
message("RPATH for Install: ${CMAKE_INSTALL_RPATH}.")
message("RPATH for Build: ${RPATH}.")



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
	SET(GCC_COVERAGE_LINK_FLAGS    "--coverage")
	SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
 	SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
    message("Set up coverage flags.")
endif(USE_GCOV)
message("CXX Flags: ${CMAKE_CXX_FLAGS}")
message("Linker Flags: ${CMAKE_EXE_LINKER_FLAGS}")

#----------------------------------------------------------------------------
#Main part

set(caplibsrc ${srcDir}/isotope_info.cpp ${srcDir}/rootUtil.cpp ${srcDir}/edepmath.cpp  ${srcDir}/weisskopf.cpp ${srcDir}/lindhard.cpp ${srcDir}/cascadeProd.cpp)
link_libraries(ROOT::Core ROOT::RIO ROOT::Tree)
add_library(libncap_obj OBJECT ${caplibsrc})
set_property(TARGET libncap_obj PROPERTY POSITION_INDEPENDENT_CODE 1)
add_library(libncap SHARED $<TARGET_OBJECTS:libncap_obj>)

add_executable(regexPlayground ${binDir}/regexPlayground.cpp)

add_executable(realizeCascades ${binDir}/realizeCascades.cpp)
target_link_libraries(realizeCascades PUBLIC libncap) #libncap includes ROOT stuff I think
add_executable(fetchYieldModel EXCLUDE_FROM_ALL ${testBin}/fetchYieldModel.cpp)
target_link_libraries(fetchYieldModel PUBLIC libncap)
add_executable(printIsotopeInfo EXCLUDE_FROM_ALL ${testBin}/printIsotopeInfo.cpp)
target_link_libraries(printIsotopeInfo PUBLIC libncap)
add_executable(readLevelfile EXCLUDE_FROM_ALL ${testBin}/readLevelfile.cpp)
target_link_libraries(readLevelfile PUBLIC libncap)
add_executable(realizeAndSave EXCLUDE_FROM_ALL ${testBin}/realizeAndSave.cpp ${incDir}/cascadeProd.h)
target_link_libraries(realizeAndSave PUBLIC libncap)

install(TARGETS realizeCascades regexPlayground RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS libncap LIBRARY DESTINATION ${CMAKE_LIB_PREFIX}/lib)
#install(TARGETS fetchYieldModel printIsotopeInfo readLevelfile realizeAndSave DESTINATION ${testBin})

add_custom_target(tests 
                DEPENDS fetchYieldModel printIsotopeInfo readLevelfile realizeAndSave
)

add_custom_target(uninstall
                COMMAND xargs rm < install_manifest.txt 
)

#----------------------------------------------------------------------------
#scrub target

# Create OBJECT_DIR variable
#set(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/realizeCascades.dir/bin ${CMAKE_BINARY_DIR}/CMakeFiles/libncap_obj.dir/src)
set(OBJECT_DIR ${CMAKE_BINARY_DIR}/CMakeFiles/realizeCascades.dir/bin/*.gcda
${CMAKE_BINARY_DIR}/CMakeFiles/realizeCascades.dir/bin/*.gcno
${CMAKE_BINARY_DIR}/CMakeFiles/regexPlayground.dir/bin/*.gcda
${CMAKE_BINARY_DIR}/CMakeFiles/regexPlayground.dir/bin/*.gcno
${CMAKE_BINARY_DIR}/CMakeFiles/fetchYieldModel.dir/tests/bin/*.gcda
${CMAKE_BINARY_DIR}/CMakeFiles/fetchYieldModel.dir/tests/bin/*.gcno
${CMAKE_BINARY_DIR}/CMakeFiles/printIsotopeInfo.dir/tests/bin/*.gcda
${CMAKE_BINARY_DIR}/CMakeFiles/printIsotopeInfo.dir/tests/bin/*.gcno
${CMAKE_BINARY_DIR}/CMakeFiles/readLevelfile.dir/tests/bin/*.gcda
${CMAKE_BINARY_DIR}/CMakeFiles/readLevelfile.dir/tests/bin/*.gcno
${CMAKE_BINARY_DIR}/CMakeFiles/realizeAndSave.dir/tests/bin/*.gcda
${CMAKE_BINARY_DIR}/CMakeFiles/realizeAndSave.dir/tests/bin/*.gcno
${CMAKE_BINARY_DIR}/CMakeFiles/libncap_obj.dir/src/*.gcda
${CMAKE_BINARY_DIR}/CMakeFiles/libncap_obj.dir/src/*.gcno
)
message("-- Object files will be output to: ${OBJECT_DIR}")

# Create the gcov-clean target. This cleans the build as well as generated 
# .gcda and .gcno files.
add_custom_target(scrub
COMMAND ${CMAKE_MAKE_PROGRAM} clean
#COMMAND rm -f ${OBJECT_DIR}/*.gcno
#COMMAND rm -f ${OBJECT_DIR}/*.gcda
COMMAND rm -f ${OBJECT_DIR}

WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
