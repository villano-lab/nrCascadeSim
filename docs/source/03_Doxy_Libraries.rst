========================================
3. The libraries of *nrCascadeSim*
========================================

The functionality of the executables of *nrCascadeSim* such as `realizeCascades` is constructed
from a set of library functions whose code is stored in the `libncap.so` file and whose prototypes
are stored in several header files. Below we list the different header files with the internal
functions, data structures, and their uses. 

---------------
`cascadeProd.h`
---------------

This header has the prototypes for the functions that generate the statistical realizations of
each cascade by Monte-Carlo simulation. The structures and functions related to our data
structures are prototyped below:

.. doxygenstruct:: cli
   :members:

.. doxygenstruct:: cri
   :members:

There are then the functions related to allocating or freeing the memory associated with those
structures (several of them are pointer-based):

.. doxygenfunction:: freecli 
   :project: nrCascadeSim

.. doxygenfunction:: freecliarray
   :project: nrCascadeSim

.. doxygenfunction:: freecri 
   :project: nrCascadeSim

.. doxygenfunction:: freecriarray 
   :project: nrCascadeSim

There are also some utility functions that are used for reading the cascade input files, and they
are prototyped below. 

.. doxygenfunction:: readCascadeDistributionFile 
   :project: nrCascadeSim

.. doxygenfunction:: interpretDbl 
   :project: nrCascadeSim

.. doxygenfunction:: interpretSn 
   :project: nrCascadeSim

.. doxygenfunction:: interpretWeisskopf 
   :project: nrCascadeSim

.. doxygenfunction:: interpretElevVector 
   :project: nrCascadeSim

.. doxygenfunction:: interpretTauVector 
   :project: nrCascadeSim

.. doxygenfunction:: vsplit 
   :project: nrCascadeSim

The functions below provide the functionality to calculate various details of the
atom/ion trajectories for the supported elements: germanium, silicon, argon, neon. At this time
there are separate functions for each of the supported elements; this is meant to be unified in
the future in order to support a wider range of elements. For now we always use
constant-acceleration `S2` stopping. `S2` refers to the parameter from the Lindhard paper [Lindhard1963]_. 

---------------
`lindhard.h`
---------------

In this header is contained prototypes for functions to furnish simple representations of the
Lindhard ionization model [Lindhard1963]_. They generally help return the ionization yield fraction given at
a particular starting energy (in **eV**). There are also specified functions to return the
ionization for an atom slowing down from one starting energy to another (as would happen in one
step of the cascade). Again, as in `cascadeProd.h` there are separate functions for each isotope
currently and this is intended to be unified in the future.   


.. doxygenfile:: lindhard.h
   :project: nrCascadeSim

---------------
`weisskopf.h`
---------------

In this header is contained a prototype for obtaining the Weisskopf decay-time estimate
[Weisskopf1951]_ for a gamma decay of a certain energy (in **MeV**) and certain multipolarity
(like `M1`, `E1`, etc.).


.. doxygenfile:: weisskopf.h
   :project: nrCascadeSim

----------------
`isotope_info.h`
----------------

In this header is contained prototypes for getting various isotope information. In the future this
should be replaced with a more robust API to a database to get all of this information. For now,
the information needed is hard-coded in the library. 

.. doxygenfile:: isotope_info.h
   :project: nrCascadeSim


-------------------
`rootUtil.h`
-------------------

In this header is contained prototypes for interfacing with the `ROOT` [ROOT1997]_ system. This is
only for the  writing of the output file.  

.. doxygenfile:: rootUtil.h
   :project: nrCascadeSim
