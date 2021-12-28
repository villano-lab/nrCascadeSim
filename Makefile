###############################################################
#
#
# Makefile for nrCascadeSim
###############################################################

#everybody here needs root
ROOTFLAGS=`root-config --cflags`

TOPDIR=$(shell pwd)
BUILDDIR=$(TOPDIR)/bin
INCDIROUT=$(BUILDDIR)/include
LIBDIROUT=$(BUILDDIR)/lib
INCDIR=inc/
SRCDIR=src/
INCFLAG= -IMersenne -Iinc
LIBFLAG= -L. -L$(LIBDIROUT)


#need to use an older version of gcc b/c of Mersenne Twister using the deprecated `register` keyword
#https://github.com/danini/graph-cut-ransac/issues/23
#CXX=clang++
#CXXFLAGS=-g -std=c++11 -Wall -pedantic

#trick for getting the git version in the code
GIT_VERSION = $(shell sh -c 'git describe --abbrev=4 --always')

override CFLAGS += -D__GIT_VERSION=\"$(GIT_VERSION)\"

RUN_SCRIPT := $(shell mkdir -p 'bin/lib')

all: $(BUILDDIR)/realizeCascades $(BUILDDIR)/regexPlayground $(LIBDIROUT)/rootUtil.o $(LIBDIROUT)/edepmath.o $(LIBDIROUT)/cascadeProd.o $(LIBDIROUT)/isotope_info.o $(LIBDIROUT)/weisskopf.o $(LIBDIROUT)/lindhard.o $(LIBDIROUT)/libncap.so

$(LIBDIROUT)/isotope_info.o: $(SRCDIR)/isotope_info.c $(INCDIR)/isotope_info.h 
	$(CXX) -fPIC -c $(CFLAGS) $(INCFLAG) $(SRCDIR)/isotope_info.c  `root-config --cflags` $(LIBFLAG) -fprofile-arcs -ftest-coverage
	mv isotope_info.o $(LIBDIROUT)/

$(LIBDIROUT)/rootUtil.o: $(SRCDIR)/rootUtil.c $(INCDIR)/rootUtil.h 
	$(CXX) -fPIC -c $(CFLAGS) $(INCFLAG) $(SRCDIR)/rootUtil.c `root-config --cflags`  $(LIBFLAG) -fprofile-arcs -ftest-coverage
	mv rootUtil.o $(LIBDIROUT)/

$(LIBDIROUT)/edepmath.o: $(SRCDIR)/edepmath.c $(INCDIR)/edepmath.h 
	$(CXX) -fPIC -c $(CFLAGS) $(INCFLAG) $(SRCDIR)/edepmath.c `root-config --cflags` $(LIBFLAG) -fprofile-arcs -ftest-coverage
	mv edepmath.o $(LIBDIROUT)/

$(LIBDIROUT)/weisskopf.o: $(SRCDIR)/weisskopf.c $(INCDIR)/weisskopf.h 
	$(CXX) -fPIC -c $(CFLAGS) $(INCFLAG) $(SRCDIR)/weisskopf.c  `root-config --cflags` $(LIBFLAG) -fprofile-arcs -ftest-coverage
	mv weisskopf.o $(LIBDIROUT)/

$(LIBDIROUT)/lindhard.o: $(SRCDIR)/lindhard.c $(INCDIR)/lindhard.h 
	$(CXX) -fPIC -c $(CFLAGS) $(INCFLAG) $(SRCDIR)/lindhard.c  `root-config --cflags` $(LIBFLAG) -fprofile-arcs -ftest-coverage
	mv lindhard.o $(LIBDIROUT)/

$(LIBDIROUT)/cascadeProd.o: $(SRCDIR)/cascadeProd.c $(INCDIR)/cascadeProd.h 
	$(CXX) -fPIC -c $(CFLAGS) $(INCFLAG) $(SRCDIR)/cascadeProd.c  `root-config --cflags` $(LIBFLAG) -fprofile-arcs -ftest-coverage
	mv cascadeProd.o $(LIBDIROUT)/

$(LIBDIROUT)/libncap.so: $(LIBDIROUT)/isotope_info.o $(LIBDIROUT)/weisskopf.o $(LIBDIROUT)/lindhard.o $(LIBDIROUT)/cascadeProd.o $(LIBDIROUT)/edepmath.o $(LIBDIROUT)/rootUtil.o
	$(CXX) -fPIC -shared $(LIBDIROUT)/lindhard.o $(LIBDIROUT)/weisskopf.o $(LIBDIROUT)/isotope_info.o $(LIBDIROUT)/cascadeProd.o $(LIBDIROUT)/edepmath.o $(LIBDIROUT)/rootUtil.o `root-config --cflags` -L`root-config --libdir` -lCore -lRIO -lTree  -o $(LIBDIROUT)/libncap.so -fprofile-arcs -ftest-coverage

$(BUILDDIR)/realizeCascades: $(LIBDIROUT)/libncap.so $(BUILDDIR)/realizeCascades.cpp
	$(CXX) -fPIC -Wl,-rpath,`root-config --libdir`,-rpath $(LIBDIROUT) $(CFLAGS) $(INCFLAG) $(LIBFLAG) $(BUILDDIR)/realizeCascades.cpp `root-config --cflags` -L`root-config --libdir` -lCore -lRIO -lTree -lncap -o $(BUILDDIR)/realizeCascades -coverage 

$(BUILDDIR)/regexPlayground: $(BUILDDIR)/regexPlayground.cpp
	$(CXX) -fPIC -Wl,-rpath $(LIBDIROUT) $(CFLAGS) $(INCFLAG) $(LIBFLAG) $(BUILDDIR)/regexPlayground.cpp -o $(BUILDDIR)/regexPlayground -coverage

install: $(BUILDDIR)/realizeCascades
	cp $(BUILDDIR)/realizeCascades /usr/local/bin/
	cp $(BUILDDIR)/regexPlayground /usr/local/bin/

clean:
	rm -f $(LIBDIROUT)/*.o
	rm -f $(LIBDIROUT)/*.so
	rm -f $(BUILDDIR)/realizeCascades
	rm -f $(BUILDDIR)/regexPlayground
	rm -f /usr/local/bin/realizeCascades
	rm -f /usr/local/bin/regexPlayground
	rm -f *.o
	rm -f *.so
	rm -rf $(LIBDIROUT)
