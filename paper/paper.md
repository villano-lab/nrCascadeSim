---
title: '`nrCascadeSim` - A simulation tool for nuclear recoil cascades resulting from neutron capture'
bibliography: references.bib
tags:
  - C++
  - Simulation
  - Nuclear Physics
authors:
  - name: A.N. Villano
    affiliation: 1
    orcid: 0000-0002-3893-7259
  - name: Kitty Harris
    affiliation: 1
    orcid: 0000-0001-5406-8367
  - name: Staci Brown
    affiliation: 2
affiliations:
 - name: Department of Physics, University of Colorado Denver, Denver CO 80217, USA
   index: 1
 - name: Department of Applied Mathematics & Statistics, University of New Mexico, Albuquerque NM 87131, USA
   index: 2
date: 16 October 2021
#nocite: '@*'
---

# Summary

Neutron capture-induced nuclear recoils have emerged as an important tool for detector
calibrations in direct dark matter detection and coherent elastic neutrino-nucleus scattering
(CE$\mathrm{\nu}$NS).

`nrCascadeSim` is a C++ command-line tool for generating simulation data for energy deposits
resulting from neutron capture on pure materials. Presently, capture events within silicon,
germanium, neon, and argon are supported. While the software was developed for solid state
detector calibration, it can be used for any application which requires simulated neutron
capture-induced nuclear recoil data.

A "cascade" occurs when a neutron becomes part of a nucleus.  The neutron can be captured to one
of many discrete energy levels, or states; if the energy level is nonzero (not the ground state),
then the state will eventually change so that it is zero.  This can happen either all at once or
in multiple steps &mdash; that is, the captured neutron may go from its state to the ground state,
or it may go to another state with lower energy that is not the ground state (provided that one
exists).  The cascade refers to the particular "path" of energy levels that a captured neutron
takes to get to the ground state from the neutron separation energy. Currently the code assumes
that the neutrons that enter the nuclear system have zero kinetic energy; this is a good
approximation for thermal neutrons because 0.0254\ eV (the average kinetic energy of a thermal
neutron) is small compared to most nuclear recoil energy scales, making it negligible.

`nrCascadeSim` models many of these cascades at once and saves the energies along with other
useful data to a single file. The output file is a `ROOT` file [@ROOT]. 



# Models Used

When modeling deposits from neutron capture events, we want to look at the recoil of the atom as a
result of these cascades.  To determine how much energy is deposited, we must track how much the
atom slows down between steps of the cascade as well as how each nuclear state change affects the
atom's kinetic energy.  `nrCascadeSim` assumes a constant deceleration that results from the atom
colliding with other nearby electrons and nuclei. This means that it must simulate, along with the
steps of the cascade, the time between each state &mdash; to calculate how much the atom slows
down. And it must also simulate the angle between the atom's momentum before a decay and the
momentum boost (gamma ray) resulting from the decay &mdash; to calculate the resulting momenta.
The time between steps is simulated as an exponential random variable based on the state's
half-life, and the angle is simulated as having an isotropic distribution.  Cascade selection is
weighted by isotope abundance [@abundances;@nudat2] and cross-section as well as the probability
of the energy level.  In existing levelfiles, energy levels are derived from [@Ge] for germanium
and from [@Si] for silicon.

The above process models the recoil energies, and the output gives both the total recoil energy
for a cascade as well as the energy per step.  For some applications, this may be the desired
output, or the user may already have a particular process they will use for converting this energy
to what they wish to measure.  However, we also include, for convenience, the ionization yield and
ionization energy of these recoils. Ionization yield is a fraction that, when multiplied by the
energy, gives the ionization energy, and ionization energy is the amount of energy that would be
read out if an otherwise equivalent electron recoil were to occur. This calculation is useful
because many solid-state detectors read out the ionization energy for nuclear recoils. This
ionization yield assumes the Lindhard model [@lindhard].

Figure \ref{LindvSor_fig} compares the normalized frequencies of ionization energies from the
Lindhard [@lindhard] model with the Sorensen [@sorensen] yield model, which is applied after the
simulation using Python, and applies detector resolution models to both. This figure demonstrates
one example of user-applied analysis utilizing the energy deposits at each step instead of the
ionization energy.

![An overlaid histogram showing an example use case in which points are generated and then multiple yield models and resolutions are applied.  The "Small Res (1/5)" histograms have Gaussians with 1/5 of the width of their counterparts. \label{LindvSor_fig}](SorVsLin_fig.pdf)

# Statement of Need

The goal of this software is to simplify the computation of the nuclear recoil spectrum following
neutron capture for a variety of applications.  These include nuclear recoil calibrations for dark
matter direct detection and coherent neutrino detection (CE$\mathrm{\nu}$NS). In these cases as the
particle detection has become more sensitive (detectors having a lower energy threshold) it is now
possible to use the capture-induced nuclear recoil events for detector calibrations. Additionally, 
thermalized neutrons will provide large backgrounds that have heretofore not been modeled. The key
roadblock to studying these scenarios is the complexity of calculating the nuclear recoil
spectrum. 

`nrCascadeSim` addresses this need by allowing users to generate nuclear recoil simulations that
reflect a variety of single-element detector setups. The energy levels that the recoiling nuclei
may pass between and their respective lifetimes are customizable, and multiple isotopes of the
same element can be present within the same simulation. Pre-defined energy level files exist for
silicon and germanium, which take into account the natural abundance data of each isotope in
[@abundances] and [@nudat2].  Output values include energy deposits at each step along each
individual cascade, total kinetic energy deposits, and ionization energy deposits. 


# State of the Field

While there are tools, such as the GEANT4 [@Geant4] framework, that allow users to simulate
neutron capture, existing tools are not built specifically for neutron capture-based nuclear
recoils as `nrCascadeSim` is and therefore use some underlying assumptions that `nrCascadeSim`
does not. The main approximation often used in GEANT4 that we avoid in `nrCascadeSim` is that all
recoils decay directly to the ground state. While this works for some applications, it is
necessary to be more precise when an accurate spectrum of neutron capture-based recoils is needed
for analyses such as calibration or background subtraction. Figure \ref{G4comp} shows a
comparison for the energy deposits produced by Geant4 for natural silicon compared with those
produced by `nrCascadeSim`. The figure does not include any instrumentation resolution and shows a
highly prominent peak around 1.25 keV recoil energy (coming from capture on $^{28}$Si directly to
the ground state) whereas the `nrCascadeSim` shows another direct-to-ground contribution (from
another isotope) at around 1.0 keV recoil energy and generally far more "spread out" recoils
coming from two- or more step cascades. 

![An overlaid histogram showing how the Geant4 `v10.7.3` energy deposits compare with those from
`nrCascadeSim` for natural silicon. \label{G4comp}](Silicon_comparison.pdf)

Recently, the power of the neutron capture-induced events has been acknowledged in the
CE$\mathrm{\nu}$NS field [@crab]. That initial study, however, used the FIFRELIN code
[@PhysRevC.82.054616], which was originally developed for modeling fission fragments and has been
updated to use statistical models of gamma emission for the purpose of modeling fission-fragment
deexcitation [@FIFRELIN].  `nrCascadeSim` takes the complementary approach of beginning with small
to medium-sized nuclei and modeling the cascades in more exact detail.  The goal is for the code
to be extended to heavier nuclei but still using this detailed approach.     

# Acknowledgments

This material is based upon work supported by the U.S. Department of Energy, Office of Science,
Office of High Energy Physics (HEP) under Award Number DE-SC0021364.

# References
