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

+------------------------------------------------------------------------------------+---------------------------------------------------+
| prototypes for structs and functions                                               |   purpose                                         |
+------------------------------------------------------------------------------------+---------------------------------------------------+
|  :C:`cli *readCascadeDistributionFile(int &n,string file,bool &success)`           |  function to read in the cascade file with n lines|
+------------------------------------------------------------------------------------+---------------------------------------------------+

---------------
`lindhard.h`
---------------


---------------
`weisskopf.h`
---------------


---------------
`isotope_info.h`
---------------


-------------------
`MersenneTwister.h`
-------------------


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


