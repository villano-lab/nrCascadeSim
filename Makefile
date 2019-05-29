###############################################################
#
#
# Makefile for cdmsRPF
###############################################################

#everybody here needs root
ROOTFLAGS=`root-config --cflags`

#we also need cdmsbats
CDMSBATSDIR=/data/chocula/villaa/cdmsbats_release/cdmsbats
BUILDDIR=$(CDMSBATSDIR)/BUILD
INCDIR=$(BUILDDIR)/include
LIBDIR=$(BUILDDIR)/lib
LINALGINCDIR=$(BUILDDIR)/linalgebra
CDMSBATSINC= -IMersenne -I$(INCDIR) -I$(LINALGINCDIR)/linalgebra -I$(LINALGINCDIR)/boost-numeric/include
CDMSBATSLIB= -L. -L$(LIBDIR)

NOISELIB= ../noiselib

#trick for getting the git version in the code
GIT_VERSION = $(shell sh -c 'git describe --abbrev=4 --always')

CFLAGS += -D__GIT_VERSION=\"$(GIT_VERSION)\"

#CPPFLAGS+= $(ROOTFLAGS)
#CPPFLAGS+= $(CDMSBATSINC)

all: evalCascades realizeCascades cascadeTest rootUtil.o edepmath.o cascadeProd.o isotope_info.o weisskopf.o lindhard.o libncap.so

isotope_info.o: isotope_info.c isotope_info.h 
	g++ -fPIC -c $(CFLAGS) $(RQINC) $(CDMSBATSINC) isotope_info.c `root-config --cflags --glibs` $(CDMSBATSLIB) -lCdmsbats

rootUtil.o: rootUtil.c rootUtil.h 
	g++ -fPIC -c $(CFLAGS) $(RQINC) $(CDMSBATSINC) rootUtil.c `root-config --cflags --glibs` $(CDMSBATSLIB) -lCdmsbats

edepmath.o: edepmath.c edepmath.h 
	g++ -fPIC -c $(CFLAGS) $(RQINC) $(CDMSBATSINC) edepmath.c `root-config --cflags --glibs` $(CDMSBATSLIB) -lCdmsbats

weisskopf.o: weisskopf.c weisskopf.h 
	g++ -fPIC -c $(CFLAGS) $(RQINC) $(CDMSBATSINC) weisskopf.c `root-config --cflags --glibs` $(CDMSBATSLIB) -lCdmsbats

lindhard.o: lindhard.c lindhard.h 
	g++ -fPIC -c $(CFLAGS) $(RQINC) $(CDMSBATSINC) lindhard.c `root-config --cflags --glibs` $(CDMSBATSLIB) -lCdmsbats

cascadeProd.o: cascadeProd.c cascadeProd.h 
	g++ -fPIC -c $(CFLAGS) $(RQINC) $(CDMSBATSINC) cascadeProd.c `root-config --cflags --glibs` $(CDMSBATSLIB) -lCdmsbats

libncap.so: isotope_info.o weisskopf.o lindhard.o cascadeProd.o edepmath.o rootUtil.o
	g++ -fPIC -shared lindhard.o weisskopf.o isotope_info.o cascadeProd.o edepmath.o rootUtil.o `root-config --cflags --glibs` -o libncap.so

cascadeTest: libncap.so cascadeTest.cpp
	g++ -fPIC -Wl,-rpath=./ $(CFLAGS) $(CDMSBATSINC) $(CDMSBATSLIB) cascadeTest.cpp `root-config --cflags --glibs` -lncap -o cascadeTest 

realizeCascades: libncap.so realizeCascades.cpp
	g++ -fPIC -Wl,-rpath=./ $(CFLAGS) $(CDMSBATSINC) $(CDMSBATSLIB) realizeCascades.cpp `root-config --cflags --glibs` -lncap -o realizeCascades 

evalCascades: libncap.so evalCascades.cpp
	g++ -fPIC -Wl,-rpath=./ $(CFLAGS) $(CDMSBATSINC) $(CDMSBATSLIB) evalCascades.cpp `root-config --cflags --glibs` -lncap -o evalCascades 

clean:
	rm -f *.o
	rm -f *.so
