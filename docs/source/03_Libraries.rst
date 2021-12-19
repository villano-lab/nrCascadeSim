========================================
3. The libraries of *nrCascadeSim*
========================================

The functionality of the executables of *nrCascadeSim* such as `realizeCascades` is constructed
from a set of library functions whose code is stored in the `libncap.so` file and whose prototypes
are stored in several header files. Below we list the different header files with the internal
functions, data structures, and their uses. 

---------------
`cascadeProd.h`
---------------

This header has the prototypes for the functions that generate the statistical realizations of
each cascade by Monte-Carlo simulation. The structures and functions related to our data
structures are prototyped in the table below:

.. role:: C(code)
   :language: C
   :class: highlight

+-------------------------------------------------------------+---------------------------------------------------+
| prototypes for structs and functions                        |   purpose                                         |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`struct cli`                                            |  the "Cascade Level Info" struct                  |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`bool cli.sucess`                                       |  has the structure been written correctly         |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`int cli.n`                                             |  number of levels in this cascade                 |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`int cli.cid`                                           |  cascade id                                       |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`double cli.frac`                                       |  probability of observing this particular cascade |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`double cli.Sn`                                         |  neutron separation energy                        |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`int cli.A`                                             |  mass number of capturing isotope                 |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`double* cli.Elev`                                      |  array of energy levels in **keV**                |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`double* cli.taus`                                      |  half-lives of energy level in **as**             |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`struct cri`                                            |  the "Cascade Recoil Info" struct                 |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`int cri.n`                                             |  n levels in the cascade that gave this recoil    |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`int cri.cid`                                           |  cascade id for cascade that gave this recoil     |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`double* cri.E`                                         |  recoil energy at beginning of recoil step **eV** |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`double* cri.delE`                                      |  energy deposited in step **eV**                  |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`int* cri.I`                                            |  ionization created in each step in n e/h pairs   |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`double* cri.Ei`                                        |  electron-equivalent ionization energy **eV**     |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`double* cri.time`                                      |  time since capture **fs**                        |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`double* cri.Eg`                                        |  gamma energy emitted in each step **MeV**        |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`void freecli(cli *cascade_levels)`                     |  function to free memory in cli struct            |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`void freecliarray(int n,cli *cascade_levels)`          |  function to free memory in array of cli structs  |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`void freecri(cri *cascade_data)`                       |  function  to free memory in cri struct           |
+-------------------------------------------------------------+---------------------------------------------------+
|  :C:`void freecriarray(int n,cli *cascade_levels)`          |  function to free memory in array of cri structs  |
+-------------------------------------------------------------+---------------------------------------------------+

There are also some utility functions that are used for reading the cascade input files, and they
are prototyped in the following table. 

.. role:: C(code)
   :language: C
   :class: highlight

+----------------------------------------------------------------------------------------+---------------------------------------------------+
| prototypes for structs and functions                                                   |   purpose                                         |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`cli *readCascadeDistributionFile(int &n,string file,bool &success)`               |  function to read in the cascade file with n lines|
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double interpretDbl(string in,bool &success)`                                     |   function to read doubles from input correctly   |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double interpretSn(string in,bool &success)`                                      |   function to read Sn from input correctly        |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double interpretWeisskopf(string in,double Egam,double A,bool &success)`          |   function to convert Weisskopf abbreviations     |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double *interpretElevVector(int &n,string in,bool &success)`                      |   function to read E levels from input correctly  |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double *interpretTauVector(int n,string in,double A,double *Elev,bool &success)`  |   function to read lifetimes from input correctly |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`vector<string> vsplit(string in)`                                                 |   function for splitting strings (C++)            |
+----------------------------------------------------------------------------------------+---------------------------------------------------+

The functions in the next table provide the functionality to calculate various details of the
atom/ion trajectories for the supported elements: germanium, silicon, argon, neon. At this time
there are separate functions for each of the supported elements; this is meant to be unified in
the future in order to support a wider range of elements. For now we always use
constant-acceleration `S2` stopping. `S2` refers to the parameter from the Lindhard paper [Lindhard1963]_. 

.. role:: C(code)
   :language: C
   :class: highlight

+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
| prototypes for structs and functions                                                                             |   purpose                                         |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`cri *Cascade(int n,int cid,double Sn,int nlev,double *Elev, double *taus, double A, mt19937 *mtrand)`  |  func. for realizing cascades (Si,Ge only for now)|
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`cri *geCascade(int n,int cid,double Sn,int nlev,double *Elev, double *taus, double A, mt19937 *mtrand)`|  germanium func. for realizing cascades           |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double geDecay(double v, double M, double Egam, mt19937 *rand)`                                              |  germanium func. energy after mid-stop decay      |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double *geStop(double E, double M, double tau, mt19937 *rand)`                                               |  germanium func. velocity at random stopping time |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double rgeS2(double E, double M, double t)`                                                                 |  germanium func. returning distance after time t  |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double vgeS2(double E, double M, double t)`                                                                 |  germanium func. returning velocity after time t  |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double vgeS2func(double *x,double *par)`                                                                    |  germanium func. velocity as function of time (x) |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`cri *siCascade(int n,int cid,double Sn,int nlev,double *Elev, double *taus, double A, mt19937 *mtrand)`|  silicon func. for realizing cascades             |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double siDecay(double v, double M, double Egam, mt19937 *rand)`                                        |  silicon func. energy after mid-stop decay        |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double *siStop(double E, double M, double tau, mt19937 *rand)`                                         |  silicon func. velocity at random stopping time   |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double rsiS2(double E, double M, double t)`                                                                 |  silicon func. returning distance after time t    |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double vsiS2(double E, double M, double t)`                                                                 |  silicon func. returning velocity after time t    |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double vsiS2func(double *x,double *par)`                                                                    |  silicon func. velocity as function of time (x)   |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`cri *arCascade(int n,int cid,double Sn,int nlev,double *Elev, double *taus, double A, mt19937 *mtrand)`|  argon func. for realizing cascades               |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double arDecay(double v, double M, double Egam, mt19937 *rand)`                                        |  argon func. energy after mid-stop decay          |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double *arStop(double E, double M, double tau, mt19937 *rand)`                                         |  argon func. velocity at random stopping time     |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double rarS2(double E, double M, double t)`                                                                 |  argon func. returning distance after time t      |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double varS2(double E, double M, double t)`                                                                 |  argon func. returning velocity after time t      |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double varS2func(double *x,double *par)`                                                                    |  argon func. velocity as function of time (x)     |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`cri *neCascade(int n,int cid,double Sn, int nlev, double *Elev, double *taus, double A, std::19937 *mtrand)`|  neon func. for realizing cascades                |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double neDecay(double v, double M, double Egam, std::19937 *rand)`                                          |  neon func. energy after mid-stop decay           |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double *neStop(double E, double M, double tau, std:19937 *rand)`                                            |  neon func. velocity at random stopping time      |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double rneS2(double E, double M, double t)`                                                                 |  neon func. returning distance after time t       |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double vneS2(double E, double M, double t)`                                                                 |  neon func. returning velocity after time t       |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double vneS2func(double *x,double *par)`                                                                    |  neon func. velocity as function of time (x)      |
+------------------------------------------------------------------------------------------------------------------+---------------------------------------------------+


---------------
`lindhard.h`
---------------

In this header is contained prototypes for functions to furnish simple representations of the
Lindhard ionization model [Lindhard1963]_. They generally help return the ionization yield fraction given at
a particular starting energy (in **eV**). There are also specified functions to return the
ionization for an atom slowing down from one starting energy to another (as would happen in one
step of the cascade). Again, as in `cascadeProd.h` there are separate functions for each isotope
currently and this is intended to be unified in the future.   

.. role:: C(code)
   :language: C
   :class: highlight

+----------------------------------------------------------------------------------------+---------------------------------------------------+
| prototypes for structs and functions                                                   |   purpose                                         |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double lindhard(double *x, double *par)`                                          |  generic lindhard function (**do not use**)       |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double lindhard_ge_k(double *x, double *par)`                                     |  germanium Lindhard yield for energy x[0]         |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double *geIonizationInRange_k(double E0,double E1,double k,std::19937 *rand)`     |  germanium Lindhard ionization in an energy range |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double lindhard_si_k(double *x, double *par)`                                     |  silicon Lindhard yield for energy x[0]           |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double *siIonizationInRange_k(double E0,double E1,double k,mt19937 *rand)`   |  silicon Lindhard ionization in an energy range   |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double lindhard_ar_k(double *x, double *par)`                                     |  argon Lindhard yield for energy x[0]             |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double *arIonizationInRange_k(double E0,double E1,double k,mt19937 *rand)`   |  argon Lindhard ionization in an energy range     |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double lindhard_ne_k(double *x, double *par)`                                     |  neon Lindhard yield for energy x[0]              |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double *neIonizationInRange_k(double E0,double E1,double k,mt19937 *rand)`   |  neon Lindhard ionization in an energy range      |
+----------------------------------------------------------------------------------------+---------------------------------------------------+

---------------
`weisskopf.h`
---------------

In this header is contained a prototype for obtaining the Weisskopf decay-time estimate [Weisskopf1951]_ for
a gamma decay of a certain energy (in **MeV**) and certain multipolarity (like `M1`, `E1`, etc.).

.. role:: C(code)
   :language: C
   :class: highlight

+----------------------------------------------------------------------------------------+---------------------------------------------------+
| prototypes for structs and functions                                                   |   purpose                                         |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double we(double Egam,double A,std::string transition="E1")`                      |  return the Weisskopf estimated lifetime          |
+----------------------------------------------------------------------------------------+---------------------------------------------------+

----------------
`isotope_info.h`
----------------

In this header is contained prototypes for getting various isotope information. In the future this
should be replaced with a more robust API to a database to get all of this information. For now,
the information needed is hard-coded in the library. 

.. role:: C(code)
   :language: C
   :class: highlight

+----------------------------------------------------------------------------------------+---------------------------------------------------+
| prototypes for structs and functions                                                   |   purpose                                         |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double getRecoilEnergy(std::string isotope="70Ge")`                               |  get the recoil energy for isotope (default 70Ge) |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double getMass(std::string isotope="70Ge")`                                       |  get the mass for isotope (default 70Ge)          |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double getDelta(std::string isotope="70Ge")`                                      |  get the mass deficit for isotope (default 70Ge)  |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double getN(std::string isotope="70Ge")`                                          |  get the neutron number for isotope (default 70Ge)|
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double getZ(std::string isotope="70Ge")`                                          |  get the protron number for isotope (default 70Ge)|
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`double getSn(std::string isotope="70Ge")`                                         |  get neutron separation for isotope (default 70Ge)|
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`void listStuff()`                                                                 |  print all available information                  |
+----------------------------------------------------------------------------------------+---------------------------------------------------+

------------------------
Mersenne Twister License
------------------------

Copyright (c) 2002, Rick Wagner, All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    All advertising materials mentioning features or use of this software must display the following acknowledgement: This product includes software developed by the <copyright holder>.
    Neither the name of the <copyright holder> nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> AS IS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

-------------------
`rootUtil.h`
-------------------

In this header is contained prototypes for interfacing with the `ROOT` [ROOT1997]_ system. This is only
for the  writing of the output file.  

.. role:: C(code)
   :language: C
   :class: highlight

+----------------------------------------------------------------------------------------+---------------------------------------------------+
| prototypes for structs and functions                                                   |   purpose                                         |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`bool addToNRTTree(TTree *t,int nr,cri *recoil_lists,cli cascade_spec)`            |  Add data to the output TTree (`ROOT` data class) |
+----------------------------------------------------------------------------------------+---------------------------------------------------+
