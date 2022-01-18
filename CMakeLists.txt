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
# other variables
#SET(topDir $(shell pwd))    #obsolete - we will mark this using CMake
#SET(buildDir $(topDir)/bin) #obsolete - we will mark this using CMake
SET(incDirOut include)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
SET(testBin ${PROJECT_SOURCE_DIR}/tests/bin)
SET(incDir ${PROJECT_SOURCE_DIR}/inc/)
SET(srcDir ${PROJECT_SOURCE_DIR}/src/)
SET(binDir ${PROJECT_SOURCE_DIR}/bin/)
SET(libFLAG "-L. -L$(CMAKE_LIBRARY_OUTPUT_DIRECTORY)")
SET(cFlags "-D__GIT_VERSION=\'$(GIT_VERSION)'")
# Make necessary bin/lib directory
# SET(runScript "$(shell mkdir -p 'bin/lib')") #hopefully don't need this anymore
# Send files to the right places


add_library(isotope_info.o ${srcDir}/isotope_info.cpp ${incDir}/isotope_info.h)
add_library(rootUtil.o ${srcDir}/rootUtil.cpp ${incDir}/rootUtil.h)
add_library(edepmath.o ${srcDir}/edepmath.cpp ${incDir}/edepmath.h)
add_library(weisskopf.o ${srcDir}/weisskopf.cpp ${incDir}/weisskopf.h)
add_library(lindhard.o ${srcDir}/lindhard.cpp ${incDir}/lindhard.h)
add_library(cascadeProd.o ${srcDir}/cascadeProd.cpp ${incDir}/cascadeProd.h)

add_executable(regexPlayground ${binDir}/regexPlayground.cpp)

link_libraries(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/weisskopf.o ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lindhard.o ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cascadeProd.o ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/edepmath.o ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/rootUtil.o)
add_library(libncap.so SHARED ${srcDir}/isotope_info.cpp ${srcDir}/rootUtil.cpp ${srcDir}/edepmath.cpp ${srcDir}/weisskopf.cpp ${srcDir}/lindhard.cpp ${srcDir}/cascadeProd.cpp)

link_libraries(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libncap.so ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/weisskopf.o ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lindhard.o ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cascadeProd.o ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/edepmath.o ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/rootUtil.o)
#link_libraries(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libncap.so) #link libncap to all later executables.
add_executable(realizeCascades ${binDir}/realizeCascades.cpp)
add_executable(fetchYieldModel ${testBin}/fetchYieldModel.cpp)
add_executable(printIsotopeInfo ${testBin}/printIsotopeInfo.cpp)
add_executable(readLevelfile ${testBin}/readLevelfile.cpp)
add_executable(realizeAndSave ${testBin}/realizeAndSave.cpp ${incDir}/cascadeProd.h)

#[[ #comment out all this stuff
all: realizeCascades regexPlayground $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/rootUtil.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/edepmath.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/cascadeProd.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/isotope_info.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/weisskopf.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/lindhard.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/libncap.so

$(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/isotope_info.o: $(srcDir)/isotope_info.cpp $(incDir)/isotope_info.h 
	$(CXX) -fPIC -c $(CFLAGS) $(INCFLAG) $(srcDir)/isotope_info.cpp  `root-config --cflags` $(LIBFLAG) -fprofile-arcs -ftest-coverage
	mv isotope_info.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/

$(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/rootUtil.o: $(srcDir)/rootUtil.cpp $(incDir)/rootUtil.h 
	$(CXX) -fPIC -c $(CFLAGS) $(INCFLAG) $(srcDir)/rootUtil.cpp `root-config --cflags`  $(LIBFLAG) -fprofile-arcs -ftest-coverage
	mv rootUtil.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/

$(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/edepmath.o: $(srcDir)/edepmath.cpp $(incDir)/edepmath.h 
	$(CXX) -fPIC -c $(CFLAGS) $(INCFLAG) $(srcDir)/edepmath.cpp `root-config --cflags` $(LIBFLAG) -fprofile-arcs -ftest-coverage
	mv edepmath.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/

$(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/weisskopf.o: $(srcDir)/weisskopf.cpp $(incDir)/weisskopf.h 
	$(CXX) -fPIC -c $(CFLAGS) $(INCFLAG) $(srcDir)/weisskopf.cpp  `root-config --cflags` $(LIBFLAG) -fprofile-arcs -ftest-coverage
	mv weisskopf.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/

$(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/lindhard.o: $(srcDir)/lindhard.cpp $(incDir)/lindhard.h 
	$(CXX) -fPIC -c $(CFLAGS) $(INCFLAG) $(srcDir)/lindhard.cpp  `root-config --cflags` $(LIBFLAG) -fprofile-arcs -ftest-coverage
	mv lindhard.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/

$(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/cascadeProd.o: $(srcDir)/cascadeProd.cpp $(incDir)/cascadeProd.h 
	$(CXX) -fPIC -c $(CFLAGS) $(INCFLAG) $(srcDir)/cascadeProd.cpp  `root-config --cflags` $(LIBFLAG) -fprofile-arcs -ftest-coverage
	mv cascadeProd.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/

$(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/libncap.so: $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/isotope_info.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/weisskopf.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/lindhard.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/cascadeProd.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/edepmath.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/rootUtil.o
	$(CXX) -fPIC -shared $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/lindhard.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/weisskopf.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/isotope_info.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/cascadeProd.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/edepmath.o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/rootUtil.o `root-config --cflags` -L`root-config --libdir` -lCore -lRIO -lTree  -o $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/libncap.so -fprofile-arcs -ftest-coverage

realizeCascades: $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/libncap.so realizeCascades.cpp
	$(CXX) -fPIC -Wl,-rpath,`root-config --libdir`,-rpath $(CMAKE_LIBRARY_OUTPUT_DIRECTORY) $(CFLAGS) $(INCFLAG) $(LIBFLAG) realizeCascades.cpp `root-config --cflags` -L`root-config --libdir` -lCore -lRIO -lTree -lncap -o realizeCascades -coverage -fprofile-arcs -ftest-coverage

regexPlayground: regexPlayground.cpp
	$(CXX) -fPIC -Wl,-rpath $(CMAKE_LIBRARY_OUTPUT_DIRECTORY) $(CFLAGS) $(INCFLAG) $(LIBFLAG) regexPlayground.cpp -o regexPlayground -coverage -fprofile-arcs -ftest-coverage

$(testBin)/fetchYieldModel: $(testBin)/fetchYieldModel.cpp $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/libncap.so
	$(CXX) -fPIC -Wl,-rpath,`root-config --libdir`,-rpath $(CMAKE_LIBRARY_OUTPUT_DIRECTORY) $(CFLAGS) $(INCFLAG) $(LIBFLAG) $(testBin)/fetchYieldModel.cpp `root-config --cflags` -L`root-config --libdir` -lCore -lRIO -lTree -lncap -o $(testBin)/fetchYieldModel -coverage

$(testBin)/printIsotopeInfo: $(testBin)/printIsotopeInfo.cpp $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/libncap.so
	$(CXX) -fPIC -Wl,-rpath,`root-config --libdir`,-rpath $(CMAKE_LIBRARY_OUTPUT_DIRECTORY) $(CFLAGS) $(INCFLAG) $(LIBFLAG) $(testBin)/printIsotopeInfo.cpp `root-config --cflags` -L`root-config --libdir` -lCore -lRIO -lTree -lncap -o $(testBin)/printIsotopeInfo -coverage

$(testBin)/readLevelfile: $(testBin)/readLevelfile.cpp $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/libncap.so
	$(CXX) -fPIC -Wl,-rpath,`root-config --libdir`,-rpath $(CMAKE_LIBRARY_OUTPUT_DIRECTORY) $(CFLAGS) $(INCFLAG) $(LIBFLAG) $(testBin)/readLevelfile.cpp `root-config --cflags` -L`root-config --libdir` -lCore -lRIO -lTree -lncap -o $(testBin)/readLevelfile -coverage

$(testBin)/realizeAndSave: $(testBin)/realizeAndSave.cpp $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/libncap.so
	$(CXX) -fPIC -Wl,-rpath,`root-config --libdir`,-rpath $(CMAKE_LIBRARY_OUTPUT_DIRECTORY) $(CFLAGS) $(INCFLAG) $(LIBFLAG) $(testBin)/realizeAndSave.cpp `root-config --cflags` -L`root-config --libdir` -lCore -lRIO -lTree -lncap -o $(testBin)/realizeAndSave -coverage

install: realizeCascades
	cp realizeCascades /usr/local/bin/
	cp regexPlayground /usr/local/bin/

tests: $(testBin)/fetchYieldModel $(testBin)/printIsotopeInfo $(testBin)/readLevelfile $(testBin)/realizeAndSave

clean:
	rm -f $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/*.o
	rm -f $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)/*.so
	rm -f realizeCascades
	rm -f regexPlayground
	rm -f $(testBin)/fetchYieldModel
	rm -f $(testBin)/printIsotopeInfo
	rm -f $(testBin)/readLevelfile
	rm -f $(testBin)/realizeAndSave
	rm -f /usr/local/bin/realizeCascades
	rm -f /usr/local/bin/regexPlayground
	rm -f *.o
	rm -f *.so
	rm -rf $(CMAKE_LIBRARY_OUTPUT_DIRECTORY)

#]]