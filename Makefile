###############################################################
#
#
# Makefile for nrCascadeSim
###############################################################

#everybody here needs root
ROOTFLAGS=`root-config --cflags`

#we also need # not cdmsbats
BUILDDIR=bin
INCDIROUT=$(BUILDDIR)/include
LIBDIROUT=$(BUILDDIR)/lib
INCDIR=inc/
SRCDIR=src/
INCFLAG= -IMersenne -Iinc
LIBFLAG= -L. -L$(LIBDIROUT)

#trick for getting the git version in the code
GIT_VERSION = $(shell sh -c 'git describe --abbrev=4 --always')

CFLAGS += -D__GIT_VERSION=\"$(GIT_VERSION)\"

RUN_SCRIPT := $(shell mkdir -p 'bin/lib')

all: $(BUILDDIR)/realizeCascades $(LIBDIROUT)/rootUtil.o $(LIBDIROUT)/edepmath.o $(LIBDIROUT)/cascadeProd.o $(LIBDIROUT)/isotope_info.o $(LIBDIROUT)/weisskopf.o $(LIBDIROUT)/lindhard.o $(LIBDIROUT)/libncap.so

$(LIBDIROUT)/isotope_info.o: $(SRCDIR)/isotope_info.c $(INCDIR)/isotope_info.h 
	g++ -fPIC -c $(CFLAGS) $(INCFLAG) $(SRCDIR)/isotope_info.c `root-config --cflags --glibs` $(LIBFLAG) 
	mv isotope_info.o $(LIBDIROUT)/

$(LIBDIROUT)/rootUtil.o: $(SRCDIR)/rootUtil.c $(INCDIR)/rootUtil.h 
	g++ -fPIC -c $(CFLAGS) $(INCFLAG) $(SRCDIR)/rootUtil.c `root-config --cflags --glibs` $(LIBFLAG) 
	mv rootUtil.o $(LIBDIROUT)/

$(LIBDIROUT)/edepmath.o: $(SRCDIR)/edepmath.c $(INCDIR)/edepmath.h 
	g++ -fPIC -c $(CFLAGS) $(INCFLAG) $(SRCDIR)/edepmath.c `root-config --cflags --glibs` $(LIBFLAG) 
	mv edepmath.o $(LIBDIROUT)/

$(LIBDIROUT)/weisskopf.o: $(SRCDIR)/weisskopf.c $(INCDIR)/weisskopf.h 
	g++ -fPIC -c $(CFLAGS) $(INCFLAG) $(SRCDIR)/weisskopf.c `root-config --cflags --glibs` $(LIBFLAG) 
	mv weisskopf.o $(LIBDIROUT)/

$(LIBDIROUT)/lindhard.o: $(SRCDIR)/lindhard.c $(INCDIR)/lindhard.h 
	g++ -fPIC -c $(CFLAGS) $(INCFLAG) $(SRCDIR)/lindhard.c `root-config --cflags --glibs` $(LIBFLAG) 
	mv lindhard.o $(LIBDIROUT)/

$(LIBDIROUT)/cascadeProd.o: $(SRCDIR)/cascadeProd.c $(INCDIR)/cascadeProd.h 
	g++ -fPIC -c $(CFLAGS) $(INCFLAG) $(SRCDIR)/cascadeProd.c `root-config --cflags --glibs` $(LIBFLAG) 
	mv cascadeProd.o $(LIBDIROUT)/

$(LIBDIROUT)/libncap.so: $(LIBDIROUT)/isotope_info.o $(LIBDIROUT)/weisskopf.o $(LIBDIROUT)/lindhard.o $(LIBDIROUT)/cascadeProd.o $(LIBDIROUT)/edepmath.o $(LIBDIROUT)/rootUtil.o
	g++ -fPIC -shared $(LIBDIROUT)/lindhard.o $(LIBDIROUT)/weisskopf.o $(LIBDIROUT)/isotope_info.o $(LIBDIROUT)/cascadeProd.o $(LIBDIROUT)/edepmath.o $(LIBDIROUT)/rootUtil.o `root-config --cflags --glibs` -o $(LIBDIROUT)/libncap.so 

$(BUILDDIR)/realizeCascades: $(LIBDIROUT)/libncap.so $(BUILDDIR)/realizeCascades.cpp
	g++ -fPIC -Wl,-rpath=$(LIBDIROUT) $(CFLAGS) $(INCFLAG) $(LIBFLAG) $(BUILDDIR)/realizeCascades.cpp `root-config --cflags --glibs` -lncap -o $(BUILDDIR)/realizeCascades 

clean:
	rm -f $(LIBDIROUT)/*.o
	rm -f $(LIBDIROUT)/*.so
	rm -f $(BUILDDIR)/realizeCascades
	rm -f *.o
	rm -f *.so
	rm -rf $(LIBDIROUT)
