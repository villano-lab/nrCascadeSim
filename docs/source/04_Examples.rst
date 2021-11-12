====================================
4. Examples: Putting it all together
====================================


---------------------------
Reading a Level Input File
---------------------------


1. First we define the DM particle objects that describe SI and SD interactions

.. code-block:: c++

   // 1. DM particle (SI and Sd)
   obscura::DM_Particle_SI dm_SI(10.0 * GeV);
   dm_SI.Set_Sigma_Proton(1.0e-40 * cm * cm);
   dm_SI.Print_Summary();

   obscura::DM_Particle_SD dm_SD(10.0 * GeV);
   dm_SD.Set_Sigma_Proton(1.0e-40 * cm * cm);
   dm_SD.Print_Summary();    

----------------------------
Printing Isotope Information
----------------------------

----------------------------------
Fetching an Ionization Yield Model
----------------------------------

--------------------------------------------------
Generating a Single Cascade Realization and Saving
--------------------------------------------------
