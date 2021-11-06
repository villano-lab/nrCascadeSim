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

.. code-block:: c++

  Usage:  (null) options [ inputfile(s) ]
    -d, --seed          <integer>      seed for random numbers 
    -n, --numgen        <number>       number of traces to generate 
    -o, --outfile       <filename>     name the output file 
    -s, --silent                       silent, no standard out 
    -v, --verbose       <level>        Print verbose messages at level <level>
    -V, --version                      print version and exit
    -l, --log           <filename>     Log additional output to the specified file. If this option is not used, no logging will occur.

.. The three most important abstract base classes of *obscura* are

.. #. ``DM_Particle``
.. #. ``DM_Distribution``
.. #. ``DM_Detector``

.. We will discuss the interface each of these classes provide in more detail.
.. But first we take a look at the detection targets in direct DM search experiments, namely nuclei, bound electrons in atoms, and bound electrons in crystals.
