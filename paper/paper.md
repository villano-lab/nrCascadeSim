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

NR Cascade Sim is a straightforward tool for generating simulation data to represent energy deposits resulting from nuclear cascade events in silicon, germanium, neon, or argon. While the software was developed for solid state detector calibration, it can be used for any application which requires simulated nuclear recoil data.

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

% *I know discussed whether to cite Nudat2 directly before - but this is what I have for now.*

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