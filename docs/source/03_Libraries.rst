========================================
3. The libraries of *nrCascadeSim*
========================================



This high level of modularity in this type of calculation needs to be reflected in the code's polymorphic structure.
The goal of *obscura* is to provide for each of the different inputs one generic interface or abstract base class, that comprises the general required functionalities, without specifying the detailed implementations further.
These depend on a multitude of assumptions which can change in different projects, for different users, etc.

If the base classes are defined properly, it is also possible and straight-forward to 

#. extend *obscura* by implementing further derived classes overriding the virtual functions of the base class.
#. design research software that is agnostic to the detailed implementation and thereby very generally applicable to a variety of scenarios. As long as our scientific functions are formulated in terms of these base functions, they will be able to handle any new implementation that comes in the form of derived classes.

The three most important abstract base classes of *obscura* are

#. ``DM_Particle``
#. ``DM_Distribution``
#. ``DM_Detector``

We will discuss the interface each of these classes provide in more detail.
But first we take a look at the detection targets in direct DM search experiments, namely nuclei, bound electrons in atoms, and bound electrons in crystals.
