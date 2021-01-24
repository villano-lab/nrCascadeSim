---
title: '`NR Cascade Sim` - A simulation tool for nuclear recoil cascades resulting from neutron capture'
bibliography: references.bib
tags:
  - C++
  - Simulation
  - Nuclear Physics
authors:
  - name: Anthony Villano
    affiliation: 1
  - name: Kitty Harris
    affiliation: 1
affiliations:
 - name: University of Colorado Denver, United States
   index: 1
date: 12 December 2020
nocite: '@*'
---

# Summary

NR Cascade Sim is a straightforward tool for generating simulation data to represent energy deposits resulting from nuclear energy deposits in silicon, germanium, neon, or argon. While the software was developed for solid state detector calibration, it can be used for any application which requires simulated nuclear recoil data.

% *it says the audience should be "non-specialist", so I'm trying to explain as much as possible (but still concisely) below.*
% *having trouble with organization*
% *^ I know this isn't a comment format but there is none in markdown so here we are*

A "cascade" occurs when a neutron becomes part of a nucleus. 
The neutron can be captured to one of many discrete energy levels, or states; 
if the energy level is nonzero (not the ground state), then the state will eventually change so that it is.
This can happen either all at once or in multiple steps &mdash; 
that is, the captured neutron may go from its state to the ground state,
or it may go to another state with lower energy that is not the ground state (provided that one exists).
The cascade refers to the particular "path" of energy levels that a captured neutron takes to get to the ground state.

% *I know discussed whether to cite Nudat2 directly before - but this is what I have for now.*
% *References to Lindhard Model*
% *References to two papers on Si and Ge cascade papers.*
% *Generate output graphic to represent cascade data*

# Models Used

When modeling deposits from neutron capture events, we want to look at the recoil of the nucleus as a result of these cascades.
To determine how much energy is deposited, we must also track how much the nucleus slows down between steps of the cascade,
as well as *how* each state change affects the nucleus' travel.
NR Cascade Sim assumes a constant deceleration that results from the nucleus colliding with other nearby nuclei. 
This means that it must simulate, along with the steps of the cascade, the time between each state 
&mdash; to calculate how much the nucleus slows down &mdash;
and the angle between the nucleus' momentum before a decay and the momentum boost resulting from the decay 
&mdash; to calculate the resulting momentum.
The time between steps is simulated as an exponential function based on the state's half-life,
and the angle is simulated as having a uniform distribution on the surface of a sphere.
Cascade selection is weighted by isotope abundance and cross-section as well as the probability of the energy level.

The above process models the recoil energies, 
and the output gives both the total recoil energy for a cascade as well as the energy per step. 
For some applications, this may be the desired outcome, 
or the user may already have a particular process they will use for converting this energy to what they wish to measure.
However, we include, for convenience, the ionization yield and ionization energy of these recoils.
This ionization yield assumes the Lindhard model\cite{Lindhard}:
$$
\array[rcl]
  Y & = & \frac{kg_{(\eps)}}{1+kg_{(\eps)}} \\
  g_{(\eps)} & = & a\eps^\gamma + b\eps^w + \eps \\
  \eps_{(E_r)} & = & 11.5E_r\[keV\]Z^{-7/3}
$$
Using the accepted value for Silicon ($k=0.143$) or Germanium ($k=0.159$), whichever is appropriate; $a=3$; and $b=0.7$.

% still need to figure out where to \cite{Si} and \cite{Ge}

# Statement of Need

NR Cascade Sim is a C++ package for generating a specified number of energy deposits resulting from nuetron capture-induced nuclear recoils.
The energy levels and their lifetimes are customizable,
and multiple isotopes of the same element can be present within the simulation.
Pre-defined energy level files exist for silicon and germanium, which are constructed from the data in \cite{abundances} and \cite{nudat2}. 
Outputs include energy deposits at each step, total kinetic energy deposits, and ionization energy deposits,
making them useful for a variety of applications.

% # Acknowledgements

% *(Not sure what to put here - if anything is needed?)*

# References