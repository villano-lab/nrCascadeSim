==================
1. Getting started
==================

------------
Installation
------------

*nrCascadeSim* is designed to run in a Unix-based system and is tested via Travis-CI_ using the
Xenial_ distribution, Ubuntu 16.04 on x86_64 archetecture.

.. _Travis-CI: https://app.travis-ci.com/github/villano-lab/nrCascadeSim
.. _Xenial: https://docs.travis-ci.com/user/reference/xenial/ 

^^^^^^^^^^^^
Dependencies
^^^^^^^^^^^^

""""""""""""""""""""""""""""""""""""
1. `ROOT <https://root.cern/>`_
""""""""""""""""""""""""""""""""""""

To install `ROOT` please follow the instructions on the `CERN  <https://root.cern/install/>`_
website.

It is intended that *nrCascadeSim* is compatible with all versions; known compatibility with
version 6. 

On Linux machines, you can also install `ROOT` via a `pre-packaged binary
<https://root.cern/install/#download-a-pre-compiled-binary-distribution>`_ run::

	wget https://root.cern/download/root_v6.24.02.Linux-ubuntu20-x86_64-gcc9.3.tar.gz
	tar -xzvf root_v6.24.02.Linux-ubuntu20-x86_64-gcc9.3.tar.gz
	source root/bin/thisroot.sh # also available: thisroot.{csh,fish,bat}


""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
2. `gcc <https://gcc.gnu.org/>`_
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

.. To install *gcc* on a Mac, we can use `homebrew <https://brew.sh/>`_ ::

..	brew install libconfig
You almost certainly have a version of `gcc` already and *nrCascadeSim* is compatible with version
4.4.7 or newer.

On Linux machines, you can build `gcc` via the `apt-get` manager::

	sudo apt-get install gcc


^^^^^^^^^^^^^^^^
Download & Build
^^^^^^^^^^^^^^^^

The `nrCascadeSim` source code can be downloaded by cloning this `git repository
<https://github.com/villano-lab/nrCascadeSim>`_: ::

   git clone https://github.com/villano-lab/nrCascadeSim.git
   cd nrCascadeSim 

The code is compiled and the executable and library is built by `Make <https://www.gnu.org/software/make/>`_. To build run the following commands from the repository's root folder.::

	make	
	sudo cmake install

If everything worked well, the executable and library file are created as::

	bin/realizeCascades
	bin/lib/libncap.so	

To clean the installation use::

	make clean


---------------------------------
Using *realizeCascades* as a tool
---------------------------------

*Obscura* can be used as a tool and builds an executable which can be run from */bin/* via::

./obscura config.cfg

As can be seen in the `/src/main.cpp <https://github.com/temken/obscura/blob/master/src/main.cpp>`_ file, this script computes direct detection limits and saves them in the */results/* folder.
The specifications of the exclusion limits (DM physics and halo model, statistics, experiment, mass range,...) are defined in a configuration file, in this case *config.cfg*.
For the handling of configuration files, *obscura* relies on `libconfig <https://hyperrealm.github.io/libconfig/>`_. 

^^^^^^^^^^^^^^^^^^^^^^
The configuration file
^^^^^^^^^^^^^^^^^^^^^^

The configuration file contains all input parameters necessary to define the various *obscura* models.

.. warning::

	The import of these parameters via libconfig is very case-sensitive. A float parameter has to be set to e.g. *1.0*, and **not** just *1*.

.. raw:: html

	<details>
	<summary><a>The full configuration file</a></summary>
 
.. code-block:: c++

   //obscura - Configuration File

   //ID
   	ID		=	"test";

   //Dark matter particle
   	DM_mass		  	=	0.1;		// in GeV
   	DM_spin		  	=	0.5;
   	DM_fraction		=	1.0;		// the DM particle's fractional abundance (set to 1.0 for 100%)
   	DM_light		=	false;		// Options: true or false. low mass mode

   	DM_interaction		=	"SI";		// Options: "SI" or "SD"

   	DM_isospin_conserved		=	true; 		// only relevant for SI and SD
   	DM_relative_couplings		=	(1.0, 0.0); //relation between proton (left) and neutron (right) couplings.
   												//only relevant if 'DM_isospin_conserved' is false.
   	DM_cross_section_nucleon	=	1.0e-36;	//in cm^2
   	DM_cross_section_electron	=	1.0e-36;	//in cm^2 (only relevant for SI and SD)
   	DM_form_factor		=	"Contact";	// Options: "Contact", "Electric-Dipole", "Long-Range", "General"
   												//(only relevant for SI)
   	DM_mediator_mass	=	0.0;		// in MeV (only relevant if 'DM_form_factor' is "General")

   //Dark matter distribution
   	DM_distribution 	=	"SHM";		//Options: "SHM", "SHM++", "File"
   	DM_local_density	=	0.4;		//in GeV / cm^3
   	
   	//Options for "SHM" and "SHM++"
   		SHM_v0		=	220.0;				//in km/sec
   		SHM_vObserver	=	(0.0, 232.0, 0.0);	//in km/sec
   		SHM_vEscape	=	544.0;				//in km/sec
   	//Options for "SHM++"
   		SHMpp_eta	=	0.2;
   		SHMpp_beta	=	0.9;
   	//Options for "File" (The file has to be a 2-column table of format v[km/sec] :: f(v) [sec/km])
   		file_path  = "DM_Speed_PDF.txt";

   //Dark matter detection experiment
   	DD_experiment	=	"Electron recoil";	//Options for nuclear recoils: "Nuclear recoil", "DAMIC_N_2011", "XENON1T_N_2017", "CRESST-II","CRESST-III", "CRESST-surface"
							//Options for electron recoils: "Semiconductor","protoSENSEI@MINOS","protoSENSEI@surface", "SENSEI@MINOS", "CDMS-HVeV_2018", "CDMS-HVeV_2020", "Electron recoil", "XENON10_S2", "XENON100_S2", "XENON1T_S2", "DarkSide-50_S2"

   	//Options for user-defined experiments ("Nuclear recoil", "Electron recoil", and "Semiconductor")
	  //General
	  DD_exposure 		=	1.0;	//in kg years
	  DD_efficiency 		=	1.0;	//flat efficiency
	  DD_observed_events 	=	0;		//observed signal events
	  DD_expected_background 	=	0.0;	//expected background events

	  //Specific options for "Nuclear recoil"
	  DD_targets_nuclear	=	(
	  				(4.0, 8),
	  				(1.0, 20),
	  				(1.0, 74)
	  			);				// Nuclear targets defined by atom ratio/abundances and Z
	  DD_threshold_nuclear    =	4.0;    //in keV
	  DD_Emax_nuclear         =	40.0;	//in keV
	  DD_energy_resolution    =	0.0;    //in keV

	  //Specific options for "Electron recoil" and "Semiconductor:
	  DD_target_electron	=	"Xe";	//Options for "Electron recoil": 	"Xe", "Ar"
	  								//Options for "Semiconductor":	"Si", "Ge"
	  DD_threshold_electron	=	4;		//In number of electrons or electron hole pairs.

   //Computation of exclusion limits
   	constraints_certainty	=	0.95;	//Certainty level
   	constraints_mass_min	=	0.02;	//in GeV										
   	constraints_mass_max	=	1.0;	//in GeV
   	constraints_masses	=	10;										
 
.. raw:: html

	</details>

----------------------------
Using *obscura* as a library
----------------------------

If we want to use *obscura* functions in an external code, we can do so and import it as a library.
We recommend to do this inside your CMake build, where *obscura* can be downloaded, built, included, and linked automatically during the build of your code.


As an instructional example `this repository <https://github.com/temken/template_cpp_cmake_obscura>`_ contains a C++ project template built with CMake that imports and uses the *obscura* library.
