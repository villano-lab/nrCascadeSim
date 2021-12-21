========================================
2. Executables of *nrCascadeSim*
========================================

Using the library functions defined in *nrCascadeSim* through the shared-object library
`libncap.so` users can define their own executables for a task or use the pre-defined executables.


------------------------------------------------
Using *realizeCascades* command-line executable 
------------------------------------------------

To get a quick list of the expected inputs and flags one can always run `realizeCascades --help`.
The result will be:

.. code-block:: bash 

  Usage:  (null) options [ inputfile(s) ]
    -d, --seed          <integer>      Seed for random numbers
    -h, --help                         Print usage 
    -n, --numgen        <number>       Number of traces to generate \n"
    -o, --outfile       <filename>     Name the output file \n"
    -s, --silent                       Silent, no standard out \n"
    -v, --verbose       <level>        Print verbose messages at level <level>. \n"
                                       Currently must use `--verbose=<level>` or `-v<level>` - no spaces.\n"
    -V, --version                      Print version and exit\n"
    -l, --log           <filename>     Log additional output to the specified file.\n"
                                       If this option is not used, no logging will occur.


The `realizeCascades` command will run the simulation a specified number of times for a given
input file.  

Note that `realizeCascades` must be run from the `nrCascadeSim/bin` directory unless the user has
added it to the path like by doing `sudo make install`.  Also note that `ROOT` must be present in
the current environment for this command to work.

^^^^^^^^^
Arguments
^^^^^^^^^

""""""""""""""""""
Required Arguments
""""""""""""""""""

All three of these arguments are required:
* ``-n, --numgen`` specifies the total number of cascade events to be simulated. (example: `-n 100000` to simulate one hundred thousand events.)
* ``-o, --outfile`` specifies the location of the output file. (example: `-o ~/output.root` to output to a file `output.root` in the home directory.)
* The main argument (no prefix) specifies the input file. (example: `levelfiles/Si28_ngam_all_cascades_rfmt_sorted.txt` to call a levelfile with all cascades for 28Si available.)

This makes the full example:

.. code-block:: bash 

  realizeCascades -n 100000 -o ~/output.root levelfiles/Si28_ngam_all_cascades_rfmt_sorted.txt


to simulate 100000 events for 28Si and output them to a file in the home directory.

""""""""""""""""""
Optional Arguments
""""""""""""""""""

* ``-h, --help`` display the help menu. This overrides other options. Help will be displayed and program will exit. 
* ``-s, --silent`` silent. Nothing will be printed to stdout.
* ``-v, --verbose`` verbosity level. Default to 1 where only the random seed is printed. Max level is currently 2 where a lot of level/simulation information is printed. Currently must use ``--verbose=<level>`` or ``-v<level>`` - no spaces.
* ``-V, --version`` version. Print the version tag of the code and exit.  
* ``-l, --log`` log file. Specify a file to print the output to.  


^^^^^^^^^^^^^^^^^^
Reproducible Files
^^^^^^^^^^^^^^^^^^

The ``-d, --seed`` flag for the seed will result in files with consistent data. 

However, additional binary data may result in checksums being different despite the data being the
same.  If you want a reproducible file that can be compared to another by an md5 checksum, append
to the output file's name:  `?reproducible=fixedname`  (you will either need to put the filename
in quotes or escape the `?` character).  Example: `-o "output.root?reproducible=fixedname"`  This
surpresses various forms of metadata that result in changes to the binary even for the same data
(see `ROOT` page on reproducible-files_ ).

.. _reproducible-files: https://root.cern.ch/doc/master/classTFile.html#ad0377adf2f3d88da1a1f77256a140d60 

Unfortunately, the binary still seems to be influenced by the environment it is generated in,
so at present there is no md5sum to compare to that will work across all devices.

-------------------------------------
*realizeCascades* cascade input file 
-------------------------------------

^^^^^^^^^^^^^^^^^^^^^^^^
Levelfile (Input) Format
^^^^^^^^^^^^^^^^^^^^^^^^

The levelfile is a singular text file read by the program using regular expressions.  Each row in
a levelfile corresponds to one possible cascade, which should include a relative weight for the
probability of the cascade's occurrence.  While it is helpful to create columns that are easy for
the user to read, columns can be delineated by any number of spaces.

The general format of one row of an input file is:

.. code-block:: bash
   
   weight isotope A [..,E2,E1,E0] [..,tau2,tau1,inf]

Each portion of this row is described in the table below.

.. list-table:: Input Row 
   :widths: 25 25 50
   :header-rows: 1

   * - Name
     - Format
     - Description
   * - `weight`
     - `#.##` or `#e+/-##`
     - The probability of this cascade occuring, normalized to unity with all other cascades. This variable includes a weight for the isotope's abundance compared to other isotopes listed within the same levelfile. (If only one isotope is present within the levelfile, the abundance weight is not needed.) Weights can be given in decimal form or scientific notation (e.g. 0.000671 or 6.71e-04). 
   * - `isotope`
     - `##Xx`
     - The isotope of the nucleus *after* capture. (For example, if 28Si is present, it will become 29Si, so 29Si should be listed.) This should be formatted as two numbers, one capital letter, and one lower-case letter (e.g. 29Si, 74Ge).
   * - `A`
     - `##`
     - The number of particles in the nucleus after capture. This should match the first two digits of `isotope`. For example, if `isotope` is 72Ge, `A` should be 72.
   * - `energies` 
     - `[... E2 E1 0]`
     - An ordered list of the energy levels traversed (keV), including the ground state (0 keV), separated by spaces. These should be in the decreasing order, the order in which the nucleus will go through the states. Do not include the separation energy to account for the initial unbound state before capture; this is already assumed.
   * - `lifetimes`
     - `[... tau2 tau1 inf]`
     - An ordered list of the lifetimes of the energy levels traversed (fs), separated by spaces. It must be the same length as the list of energies, and the lifetimes should be in the same order as the energies. The last entry is `100000000000000.0` (1e+14 fs, or 100 ms), which is effectively infinite on the timescale of the simulation, to indicate that the state is stable at the ground state.

Note: for the lifetimes one can also use strings representing multipolarity (like `M1`, `E1`,
etc.) to instruct the program to use the corresponding Weisskopf estimate [Weisskopf1951]_. These estimates
are not very accurate and are known to be systematically low. 

^^^^^^^^^^
On Weights
^^^^^^^^^^

The sum of the probabilities must be less than or equal to one in order for the simulation to 
work properly. If the sum is less than one, the simulation may skip generating some points in 
the output &mdash; for example, when requesting 100 entries, if the total probability is 0.95, 
one would expect 95 entries on average &mdash; but the input cascades will still be at the 
correct proportions with respect to one another. If the sum is greater than one, the simulation 
may not reach certain cascades at all--for instance, if a file has 12 cascades, and the 
probabilities of the first 10 add up to 1, then the last two will never be generated.

"""""""""""""""""""""""""""""""""""
An example for calculating weights:
"""""""""""""""""""""""""""""""""""

A silicon detector has three isotopes, which become 29Si, 30Si, and 31Si after capture.  The
abundances within the detector are 60%, 30%, and 10%, respectively.  Each has three possible
cascades we want to model, which we list below in our (incomplete) draft of the levelfile:

.. code-block:: bash

   weight? 29Si 29 [0]         [100000000000000.0]
   weight? 29Si 29 [5000 0]    [0.84   100000000000000.0]
   weight? 29Si 29 [3000 0]    [0.5    100000000000000.0]
   weight? 30Si 30 [0]         [100000000000000.0]
   weight? 30Si 30 [4000 0]    [1      100000000000000.0]
   weight? 30Si 30 [2000 0]    [0.15   100000000000000.0]
   weight? 31Si 31 [0]         [100000000000000.0]
   weight? 31Si 31 [4999 0]    [0.15   100000000000000.0]
   weight? 31Si 31 [540  0]    [.954   100000000000000.0]


Let's say the probabilities of the cascade occurring **within the respective isotopes** are as below:


+-----------+-------------+-------+---------------+---------------+
| **29Si**  | Cascade     | `[0]` | `[5000    0]` | `[3000    0]` | 
+-----------+-------------+-------+---------------+---------------+
|           | Probability | 0.35  |    0.5        |    0.15       |
+-----------+-------------+-------+---------------+---------------+

+-----------+-------------+-------+---------------+---------------+
| **30Si**  | Cascade     | `[0]` | `[4000    0]` | `[2000    0]` | 
+-----------+-------------+-------+---------------+---------------+
|           | Probability | 0.8   |    0.1        |    0.1        |
+-----------+-------------+-------+---------------+---------------+

+-----------+-------------+-------+---------------+---------------+
| **31Si**  | Cascade     | `[0]` | `[4999    0]` | `[540    0]`  | 
+-----------+-------------+-------+---------------+---------------+
|           | Probability | 0.2   |    0.3        |    0.5        |
+-----------+-------------+-------+---------------+---------------+


Then the relative probabilities **within the simulation** are:

+-----------+-------------+-------+---------------+---------------+
| **29Si**  | Cascade     | `[0]` | `[5000    0]` | `[3000    0]` | 
+-----------+-------------+-------+---------------+---------------+
|           | Probability | 0.21  |    0.3        |    0.09       |
+-----------+-------------+-------+---------------+---------------+

+-----------+-------------+-------+---------------+---------------+
| **30Si**  | Cascade     | `[0]` | `[4000    0]` | `[2000    0]` | 
+-----------+-------------+-------+---------------+---------------+
|           | Probability | 0.24  |    0.03       |    0.03       |
+-----------+-------------+-------+---------------+---------------+

+-----------+-------------+-------+---------------+---------------+
| **31Si**  | Cascade     | `[0]` | `[4999    0]` | `[540    0]`  | 
+-----------+-------------+-------+---------------+---------------+
|           | Probability | 0.0   |    0.03       |    0.05       |
+-----------+-------------+-------+---------------+---------------+


Making our completed levelfile:

.. code-block:: bash

   0.21    29Si 29 [0]         [100000000000000.0]
   0.30    29Si 29 [5000 0]    [0.84   100000000000000.0]
   0.09    29Si 29 [3000 0]    [0.5    100000000000000.0]
   0.24    30Si 30 [0]         [100000000000000.0]
   0.03    30Si 30 [4000 0]    [1      100000000000000.0]
   0.03    30Si 30 [2000 0]    [0.15   100000000000000.0]
   0.02    31Si 31 [0]         [100000000000000.0]
   0.03    31Si 31 [4999 0]    [0.15   100000000000000.0]
   0.05    31Si 31 [540  0]    [.954   100000000000000.0]

^^^^^^^^^^^^^^^^^^^^^^^^^
On Energies and Lifetimes
^^^^^^^^^^^^^^^^^^^^^^^^^

In the following levelfile row, the nth lifetime entry corresponds to the nth energy level entry.

.. code-block:: bash

   0.30    29Si 29 [5000 4000 3000 2000 1000 0]    [0.84 0.95 1.35 0.03 0.11 100000000000000.0]

Therefore, the program reads this as:

+---------------+-----------+-----------+-----------+-----------+-----------+
| Energy level: | 5000 keV  | 4000 keV  | 3000 keV  | 2000 keV  | 1000 keV  |
+---------------+-----------+-----------+-----------+-----------+-----------+
| **Lifetime:** | 0.84 fs   | 0.95 fs   | 1.35 fs   | 0.03 fs   | 0.11 fs   |
+---------------+-----------+-----------+-----------+-----------+-----------+


-------------------------------------
*realizeCascades* cascade output file 
-------------------------------------

Note: ROOT_ is needed to open these files.

.. _ROOT: https://root.cern/install/

A file that contains the separate NR deposits, along with their Ionization deposits.  In addition
all of the exiting gamma energies and times should be listed.

The output files are `*.root` files and therefore cannot be read as text.
Instead, they need to be imported to a program to be read out.
One straightforward way of reading these files is with python and the
`uproot <https://pypi.org/project/uproot/>`_ package.

The `*.root` files store information in a tree-like structure. The top-most key in the output
files will be `cascade` (there are no other top-level keys). Beneath this, the following keys
exist:  

.. list-table:: Output Structure 
   :widths: 25 25 25 50
   :header-rows: 1
   
   * -  `Name`  
     -  `Shape`       
     -  **Units** 
     -  Description 
   * -  `n`  
     -  `1D Array`    
     -   N/A       
     -  Array denoting the number of energy levels in a given cascade. This includes intermediate levels and the ground state.
   * -  `cid`    
     -  `1D Array`    
     -   N/A       
     -   Array of cascade IDs. The cascade ID is the number of the row in the levelfile which contains the cascade used. These count starting from zero.
   * -  `Elev` 
     -  `Jagged Array` 
     -   **keV**   
     - Array of energy level inputs. Each entry is an array of size `n`.
   * - `taus`
     - `Jagged Array`
     -  **femto-sec (fs)**  
     -  Array of lifetime inputs. Each entry is an array of size `n`.
   * -  `delE`   
     -  `Jagged Array`
     -   **eV**    
     -   Array of energy deposits between energy levels. Each entry is an array of size `n - 1`. It contains the individual energy deposits, not the total energy deposit. If using a custom nonlinear ionization model, these are the best to operate on.
   * -  `I` 
     - `Jagged Array`
     -  None   
     -  Array containing the ionization calculations for each energy deposit. Each entry is an array of size `n - 1`. This ionization is given in terms of a number of charges.
   * -  `Ei` 
     -  `Jagged Array`
     -   **eV** 
     -  Array of calculated ionization energy per step. These energies are conversions of `delE` to ionization energies. Each entry is an array of size `n - 1` containing the individual ionization energies. The Lindhard model is used here.
   * -  `time`  
     -  `Jagged Array`
     -   **fs**  
     -   Array of the time spent at each energy level. Each entry is an array of size `n` containing individual times.
   * -   `Eg`  
     -   `Jagged Array`
     -    **MeV**   
     -    Array of gamma energies. Each entry is an array of gamma energies, corresponding to an energy deposit.

The ordering of values in the arrays are consistent; that is, the nth entry of `n` corresponds to
the nth entry of `cid`, the nth entry of `Elev`, and so on.  The length of each main array should
be equal to the number of simulations; that is, if running 10000 events, `n` and `cid` will have
lengths of 10000 and the jagged arrays will have first dimensions of length 10000.

.. image:: https://raw.githubusercontent.com/villano-lab/nrCascadeSim/master/output_structure.svg 
   :width: 750 
.. The three most important abstract base classes of *obscura* are

.. #. ``DM_Particle``
.. #. ``DM_Distribution``
.. #. ``DM_Detector``

.. We will discuss the interface each of these classes provide in more detail.
.. But first we take a look at the detection targets in direct DM search experiments, namely nuclei, bound electrons in atoms, and bound electrons in crystals.
