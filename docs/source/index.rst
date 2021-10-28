.. .. image:: https://github.com/temken/obscura/actions/workflows/main.yml/badge.svg?branch=master
..   :target: https://github.com/temken/obscura/actions/workflows/main.yml
..   :alt: Build Status
.. .. image:: https://codecov.io/gh/temken/obscura/branch/master/graph/badge.svg?token=1Pe1QMcngr
..   :target: https://codecov.io/gh/temken/obscura
..   :alt: Code Coverage 
.. image:: https://readthedocs.org/projects/nrcascadesim/badge/?version=latest
   :target: https://nrcascadesim.readthedocs.io/en/latest/?badge=latest
   :alt: Documentation Status
.. image:: https://img.shields.io/badge/License-MIT-blue.svg
   :target: https://opensource.org/licenses/MIT
   :alt: License

============================================================================================
*nrCascadeSim* - a tool for generating nuclear recoil spectra resulting from neutron capture
============================================================================================

.. image:: https://zenodo.org/badge/5579857.svg
   :target: https://zenodo.org/badge/latestdoi/5579857
   :alt: DOI

The purpose of this code is to simulate energy deposits due to cascading of energy levels
following neutron capture.  This code was written for use in nuclear recoil calibration for dark
matter detectors, but may be useful in other particle physics applications as well, including
coherent elastic neutrino nucleus scattering (CE:math:`{\nu}` NS).  Currently, we use a constant
acceleration model for the atom slowing down and calculation of the ionization energy.  We also
use the Lindhard model for calculating the ionization, but the output is complete enough to allow
the user to choose their ionization yield model after simulation.  The code currently supports
Neon, Argon, Silicon, and Germanium cascades slowing down in a lattice of like material.

.. .. image:: https://raw.githubusercontent.com/villano-lab/nrCascadeSim/master/paper/SorVsLin_fig.pdf 
   :width: 500

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   01_Getting_Started
..   02_Main_Modules
..   03_Targets
..   04_DM_Particle
..   05_DM_Distribution
..   06_DM_Detector
..   07_Examples
..   08_Experiments
..   09_Citations
..   10_Release_History
..   11_License
..   12_Contact
..   References

