========================================
2. Executables of *nrCascadeSim*
========================================

Using the library functions defined in *nrCascadeSim* through the shared-object library
`libncap.so` users can define their own executables for a task or use the pre-defined executables.


----------------------------------------------
Using *realizeCascades* command-line executable 
----------------------------------------------

To get a quick list of the expected inputs and flags one can always run `realizeCascades --help`.
The result will be:

.. code-block:: bash 

  Usage:  (null) options [ inputfile(s) ]
    -d, --seed          <integer>      seed for random numbers 
    -h, --help                         print usage 
    -n, --numgen        <number>       number of traces to generate 
    -o, --outfile       <filename>     name the output file 
    -s, --silent                       silent, no standard out 
    -v, --verbose       <level>        Print verbose messages at level <level>
    -V, --version                      print version and exit
    -l, --log           <filename>     Log additional output to the specified file. If this option is not used, no logging will occur.


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
* ``-v, --verbose`` verbosity level. Default to 1 where only the random seed is printed. Max level is currently 2 where a lot of level/simulation information is printed.
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

.. The three most important abstract base classes of *obscura* are

.. #. ``DM_Particle``
.. #. ``DM_Distribution``
.. #. ``DM_Detector``

.. We will discuss the interface each of these classes provide in more detail.
.. But first we take a look at the detection targets in direct DM search experiments, namely nuclei, bound electrons in atoms, and bound electrons in crystals.
