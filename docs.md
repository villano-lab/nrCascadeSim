# Summary

The purpose of this code is to simulate energy deposits due to cascading of energy levels following neutron capture. 
Currently, the Lindhard model is assumed when determining the ionization energy [check that this is actually true...],
but as the energies before this conversion are also output [awk],
any model can be used.
The code currently works best for Silicon and Germanium cascades.


# Installation Instructions

This program is designed to run in a Unix-based system and has been tested on Ubuntu and MacOS.

## Dependencies

Required:
* CERN's ROOT, which has its own installation instructions here: [https://root.cern/install/](https://root.cern.install/)
* gcc, which can be installed via the command line (ex: `sudo apt-get install gcc`).

Optional:
* Anaconda or conda, as optionally used in the ROOT installation process: [https://www.anaconda.com/products/individual](https://www.anaconda.com/products/individual)
* A package for reading *.root files (for python, one example of this is [uproot](https://pypi.org/project/uproot/) `pip install uproot`)

## Steps

1) [Download](https://github.com/villano-lab/nrCascadeSim/archive/master.zip) or clone (`git clone https://github.com/villano-lab/nrCascadeSim.git`) the master branch of this repository
    * If downloaded as a zip, unzip the file (`unzip "nrCascadeSim-master.zip"`)
    * Download/clone to the location you want the final installation to be
2) Install dependencies
3) Enter the new directory (`cd nrCascadeSim`)
4) Either add ROOT to the current enviornment or switch to an environment in which it is active (see [ROOT instalation instructions](https://root.cern/install/))
5) Run the command `make`

# Quick Start

1) If ROOT is not part of the current environment, switch to an appropriate environment (ex: `conda activate name_of_root_env`)
2) Switch to the appropriate directory (`cd /path/to/directory/nrCascadeSim`)
3) ./realizeCascades -n desired-number-of-events -o /path/to/output/file levelfiles/name_of_levelfile.txt

# Instructions for Use

## realizeCascades

The `./realizeCascades` command will run the simulation a specified number of times for a given input file.

### Arguments

All three of these arguments are required:
* `-n` specfies the total number of cascade events to be simulated. (example: `-n 100000`)
* `-o` specifies the location of the output file. (example: `-o ~/output.root`)
* The main argument (no prefix) specifies the input file. (example: `levelfiles/Si28_ngam_all_cascades_rfmt_sorted.txt`)
Making the full example: `./realizeCascades -n 100000 -o ~/output.root levelfiles/Si28_ngam_all_cascades_rfmt_sorted.txt`

----------------------------------------
An issue we may run into:
"single-function libraries are not acceptable." (https://joss.readthedocs.io/en/latest/submitting.html)
However, I believe this is the only function of the package...

# Levelfile (Input) Format

The levelfile is a text file read by the program using regular expressions.
Each row in a levelfile corresponds to one cascade.
While it is helfpul to create columns that are easy for the user to read,
columns can be delineated by any number of spaces.

The first column is the probability of a cascade occuring. 
This can be in scientific notation or a "standard" decimal (0.000671 or 6.71e-04).
This should be weighted by the relative abundance of the material to other materials present in the same levelfile.
It should also be weighted by the cross-section for interactions and the probability of the particular energy levels being reached.
The sum of the probabilities must be less than or equal to one in order for the simulation to work properly.
If the sum is less than one, the simulation may skip generating some points in the output
-- for example, when requesting 100 entries, if the total probability is 0.95, one would expect 95 entries on average -- 
but the input cascades will still be at teh correct proportions with respect to one another.
However, if the sum is greater than one, the simulation may not reach certain cascades at all -- 
for instance, if a file has 12 cascades, and the probabilities of the first 10 add up to 1, 
then the last two will never be generated.

The second column is a label - this exists solely for the user's benefit. [verify this!]
Typically, it specifies an isotope of an element, following the standard format ##Xx (ex: 28Si). 
Some levelfiles may have only one isotope, one element with multiple isotopes, or even multiple elements. 

The third column is the number of particles in the nucleus.
For example, for 72Ge, the third column would be 72.

The fourth column contains a series of energy levels. 
The neutron is assumed to be captured, giving it an energy level of at least the separation energy;
this energy is *not* listed.
All other energy levels of the cascade, including the ground state, are listed in keV within brackets, separated by spaces, in descending order.
(The energy levels are written relative to the ground state; they are positive, and the ground state's energy is zero.)
So, an entry of \[0\] indicates the straight-to-ground case,
while an entry of [8104.8	2235.3	0] will capture to the 8104.8 keV energy level, decay to 2235.3 keV, and then finally go to the ground state.

The final column is formatted much as the forth one.
It contains the lifetimes of the energy levels in attoseconds \[as\],
or thousandths of femtoseconds.
Each entry should match up with an energy level;
that is, the first lifetime will correspond to the first energy level,
and the last lifetime will correspond to the lifetime of the ground state,
which is approximated as 1e+14 \[as\], or 1 \[ms\].
The levels are again all contained within brackets and separated with spaces:
[24000	215000	1e+14]

# Root file (output) format

The output files are *.root files and therefore cannot be read as text.
Instead, they need to be imported to a program to be read out.
One straightforward way of reading these files is with python and the [uproot](https://pypi.org/project/uproot/) package.

The *.root files store information in a tree-like structure. The top-most key in the output files will be `cascade` (there are no other top-level keys). Beneath this, the following keys exist:
* `n` - *1D Array* - Array denoting the number of energy levels in a given cascade. This includes intermediate levels and the 
* `cid` - *1D Array* - Array of cascade IDs. The cascade ID is the number of the row in the levelfile which contains the cascade used. These count starting from zero.
* `Elev` - *Jagged Array* - Array of energy level inputs. Each entry is an array, and the returned array varies in size because the number of energy levels varies.
* `taus` - *Jagged Array* - Array of lifetime inputs. Each entry is an array.
* `delE` - *Jagged Array* - Array of energy deposits between energy levels. Each entry is an array of the individual energy deposits, not the total energy deposit. If using a custom nonlinear ionization model, these are the best to operate on.
* `I` - *Jagged Array* - 
* `Ei` - *Jagged Array* - Array of calculated ionization energy per step. These are conversions of delE. Each entry is an array of the individual ionization energies.
* `time` - *Jagged Array* - Array of the time spent at each energy level. Each entry is an array of individual times.
* `Eg` - *Jagged Array* - 

The ordering of values in the arrays are consistent; that is, the nth entry of `n` corresponds to the nth entry of `cid`, the nth entry of `Elev`, and so on.

I have NO idea what `I` or `Eg` correspond to.