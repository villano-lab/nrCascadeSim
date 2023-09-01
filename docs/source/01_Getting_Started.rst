==================
1. Getting started
==================

------------
Installation
------------

*nrCascadeSim* is designed to run in a Unix-based system and is tested via multiple CI systems. 
It is hypothetically compatible with most versions of ROOT. 
The table below describes the combinations of tests run.

.. table:: Version Support
        :widths: auto

        =====================        ============       ==================      ====================
        Operating System             C++ Standard       ROOT Versions           Last Working Version
        =====================        ============       ==================      ====================
        Ubuntu 16.04: Xenial_        `c++11`            v6.20/00, v6.24/06      Current
        Ubuntu 18.04: Bionic_        `c++14`            v6.20/00, v6.24/06      Current
        Ubuntu 20.04: Focal_         `c++17`            v6.24/06                Current
        MacOS 10.15: Catalina        `c++11`            v6.20/00, v6.24/06      Current
        MacOS 11: Big Sur            `c++11`            v6.24/06                v1.5.0
        Mac M1 ??                    `c++??`            v?.??/??                not yet implemented
        =====================        ============       ==================      ===================

Because of a combination of the operating systems tested and the versions of `ROOT` (see below)
the Travis-CI_ tests three C++ compiler standards: `c++11` (Xenial_, Mojave, Catalina, Big Sur);
`c++14` (Bionic_); and `c++17` (Focal_). 

.. _Xenial: https://docs.travis-ci.com/user/reference/xenial/ 
.. _Bionic: https://docs.travis-ci.com/user/reference/bionic/ 
.. _Focal:  https://docs.travis-ci.com/user/reference/focal/

^^^^^^^^^^^^
Dependencies
^^^^^^^^^^^^

""""""""""""""""""""""""""""""""""""
1. `ROOT <https://root.cern/>`_
""""""""""""""""""""""""""""""""""""

Travis-CI_ tests two versions of `ROOT`: 6.24.06 and 6.20.00. It is assumed (though not explicitly
tested) that the versions in between those two versions will also work. 
See Table 1 above for more information.

To install `ROOT` please follow the instructions on the `CERN  <https://root.cern/install/>`_
website.

It is intended that *nrCascadeSim* is compatible with all versions; known compatibility with
version 6. 

On Linux machines, you can also install `ROOT` via a `pre-packaged binary
<https://root.cern/install/#download-a-pre-compiled-binary-distribution>`_ run::

	wget https://root.cern/download/root_v6.24.02.Linux-ubuntu20-x86_64-gcc9.3.tar.gz
	tar -xzvf root_v6.24.02.Linux-ubuntu20-x86_64-gcc9.3.tar.gz
	source root/bin/thisroot.sh # also available: thisroot.{csh,fish,bat}

**Note:** You will need to find the appropriate ROOT link for your Linux distribution. 
The one provided above is used for Focal. Bionic uses ``root_v6.24.06.Linux-ubuntu18-x86_64-gcc7.5.tar.gz``
and Xenial uses ``root_v6.24.06.Linux-ubuntu20-x86_64-gcc9.3.tar.gz``.

**If you are using WSL,** you will need to `install the conda package <https://root.cern/install#conda>`_ (recommended) 
or `build ROOT from source <https://root.cern/install#build-from-source>`_. 
Using a pre-compiled binary or installing from most package managers will not work for WSL users.

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
2. `gcc <https://gcc.gnu.org/>`_
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

.. To install *gcc* on a Mac, we can use `homebrew <https://brew.sh/>`_ ::

..	brew install libconfig

You almost certainly have a version of `gcc` already and *nrCascadeSim* is compatible with version
4.4.7 or newer.

On Linux machines, you can build `gcc` via the `apt-get` manager::

	sudo apt-get install gcc

If, for some reason, you need to use a C++ standard older than 11, please use nrCascadeSim v1.2.2 or older.


^^^^^^^^^^^^^^^^
Download & Build
^^^^^^^^^^^^^^^^

The `nrCascadeSim` source code can be downloaded by cloning this `git repository
<https://github.com/villano-lab/nrCascadeSim>`_: ::

   git clone https://github.com/villano-lab/nrCascadeSim.git
   cd nrCascadeSim 

The code is compiled and the executable and library is built by `CMake <https://cmake.org/>`_. To build, choose a build directory `/path/to/build/` and do.::

        mkdir -p /path/to/build
        cd /path/to/build
	cmake -DCMAKE_INSTALL_PREFIX=/path/to/install /path/to/source	
	make

If everything worked well, the executable and library file are created in the build directory as::

	realizeCascades
	regexPlayground
	lib/libncap.so	

And various `.o` files. If you would like to build the testing suite, use::

        make tests

To clean the installation use::

	make clean

To install to the directory `/path/to/install` use::

        make install

From the build directory. Then, if you want to uninstall use::

        make uninstall
       



---------------------------------------
Using *nrCascadeSim* command-line tools
---------------------------------------

*nrCascadeSim* contains pre-built executables built on the library functions that can be used as
command-line tools. See Section 2 for list of the executables and inputs and outputs. 

.. .. warning::

..	The import of these parameters via libconfig is very case-sensitive. A float parameter has to be set to e.g. *1.0*, and **not** just *1*.

..  .. raw:: html

..	<details>
.. 	<summary><a>The full configuration file</a></summary>
 
 
.. .. raw:: html

..	</details>

----------------------------------
Using *nrCascadeSim* as a library
----------------------------------

If we want to use *nrCascadeSim* functions in an external code, we can do so and import it as a library.
We recommend to do this inside your Make build, where the *nrCascadeSim* libraries can be linked
with the `-fPIC` and `-lncap` flags.

Use the following headers when including the library functions:


.. code-block:: c++

  //library commands
  #include "cascadeProd.h"
  #include "lindhard.h"
  #include "weisskopf.h"
  #include "isotope_info.h"
  #include "MersenneTwister.h"
  
  //ROOT stuff
  #include "rootUtil.h"


Using the `ROOT` utilities  will also require having `ROOT` installed and adding `root-config
--cflags --glibs` to the compilation line in your Make file. 


.. As an instructional example `this repository <https://github.com/temken/template_cpp_cmake_obscura>`_ contains a C++ project template built with CMake that imports and uses the *obscura* library.
