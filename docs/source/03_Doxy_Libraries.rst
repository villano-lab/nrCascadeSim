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

---------------
`weisskopf.h`
---------------

In this header is contained a prototype for obtaining the Weisskopf decay-time estimate
[Weisskopf1951]_ for a gamma decay of a certain energy (in **MeV**) and certain multipolarity
(like `M1`, `E1`, etc.).


.. doxygenfile:: weisskopf.h
   :project: nrCascadeSim
