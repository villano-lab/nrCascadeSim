.. .. image:: https://github.com/temken/obscura/actions/workflows/main.yml/badge.svg?branch=master
..   :target: https://github.com/temken/obscura/actions/workflows/main.yml
..   :alt: Build Status
.. .. image:: https://codecov.io/gh/temken/obscura/branch/master/graph/badge.svg?token=1Pe1QMcngr
..   :target: https://codecov.io/gh/temken/obscura
..   :alt: Code Coverage 
.. image:: https://app.travis-ci.com/villano-lab/nrCascadeSim.svg?branch=master 
   :target: https://app.travis-ci.com/villano-lab/nrCascadeSim
   :alt: Build Status 
.. image:: https://readthedocs.org/projects/nrcascadesim/badge/?version=latest
   :target: https://nrcascadesim.readthedocs.io/en/latest/?badge=latest
   :alt: Documentation Status
.. image:: https://img.shields.io/badge/License-MIT-blue.svg
   :target: https://opensource.org/licenses/MIT
   :alt: License: MIT
.. image:: https://img.shields.io/badge/arXiv-2104.02742-orange.svg?style=flat
   :target: https://arxiv.org/abs/2104.02742
   :alt: arXiv: 2104.02742
.. image:: https://codecov.io/gh/villano-lab/nrCascadeSim/branch/develop/graph/badge.svg?token=Q6XPU6LPPL
   :target: https://codecov.io/gh/villano-lab/nrCascadeSim
    

============================================================================================
*nrCascadeSim* - a tool for generating nuclear recoil spectra resulting from neutron capture
============================================================================================

.. image:: https://zenodo.org/badge/DOI/10.5281/zenodo.5579857.svg
   :target: https://doi.org/10.5281/zenodo.5579857
   :alt: DOI
.. image:: https://joss.theoj.org/papers/d69ced49c5c17fdbf637e0747d815deb/status.svg
   :target: https://joss.theoj.org/papers/d69ced49c5c17fdbf637e0747d815deb
   :alt: JOSS paper

The purpose of this code is to simulate energy deposits due to cascading of energy levels
following neutron capture.  This code was written for use in nuclear recoil calibration for dark
matter detectors, but may be useful in other particle physics applications as well, including
coherent elastic neutrino nucleus scattering (CEνNS).  Currently, we use a constant
acceleration model for the atom slowing down and calculation of the ionization energy.  We also
use the Lindhard model for calculating the ionization, but the output is complete enough to allow
the user to choose their ionization yield model after simulation.  The code currently supports
Neon, Argon, Silicon, and Germanium cascades slowing down in a lattice of like material.

The documentation does not contain a review of the physics implemented in the library.

If you want to contribute to `nrCascadeSim`, please check out the `contribution guidelines
<https://github.com/villano-lab/nrCascadeSim/blob/master/CONTRIBUTING.md>`_.

.. image:: https://raw.githubusercontent.com/villano-lab/nrCascadeSim/master/paper/SorVsLin_fig.png 
   :width: 500

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   01_Getting_Started
   02_Executables
   03_Doxy_Libraries
   04_Examples
   05_Documentation
   06_Citations
   07_Release_History
   08_License
   09_Contact
   References

