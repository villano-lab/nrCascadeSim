[![Build Status](https://app.travis-ci.com/villano-lab/nrCascadeSim.svg?branch=master)](https://app.travis-ci.com/villano-lab/nrCascadeSim)  
[![Documentation Status](https://readthedocs.org/projects/nrcascadesim/badge/?version=latest)](https://nrcascadesim.readthedocs.io/en/latest/?badge=latest)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)


# nrCascadeSim - a tool for generating nuclear recoil spectra resulting from neutron capture

[![status](https://joss.theoj.org/papers/fd8076268036956d3bf08193c4fc2db9/status.svg)](https://joss.theoj.org/papers/fd8076268036956d3bf08193c4fc2db9)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.5579857.svg)](https://doi.org/10.5281/zenodo.5579857) <br/>


<img src="paper/SorVsLin_fig.png" width="500">

## Ionization

Ionization assumes the [Lindhard](https://www.osti.gov/biblio/4701226) model:

  Y = k*g(&epsilon;)/(1+kg(&epsilon;))  
  g(&epsilon;) = a\*&epsilon;<sup>&gamma;</sup> + b\*&epsilon;<sup>&omega;</sup> + &epsilon;  
  &epsilon;(E<sub>r</sub>) = 11.5E<sub>r</sub>\[keV\]Z<sup>-7/3</sup>

Using the accepted value for Silicon (*k = 0.143*) or Germanium (*k = 0.159*), whichever is
appropriate; *a = 3*; *b = 0.7*; *&gamma; = 0.15*; and *&omega; = 0.6*.
