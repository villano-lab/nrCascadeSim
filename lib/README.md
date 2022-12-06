# nrCascadeSim - a tool for generating nuclear recoil spectra resulting from neutron capture

This directory contains the object files for the `nrCascadeSim` library. 

## Separate Files

* `libcap.so` -- shared object for the whole `nrCascadeSim` libraries.
* `cascadeProd.o` -- cascade simulating functions
* `edepmath.o` -- mathematical functions
* `isotope_info.o` -- information like neutron separation energies for specific isotopes
* `lindhard.o` -- functions to implement ionization yield
* `rootUtil.o` --  `ROOT` file writing
* `weisskopf.o` -- Weisskopf lifetime estimate functions
