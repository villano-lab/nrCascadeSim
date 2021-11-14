# Example Usecase

The primary purpose of this directory is to house components of `Yields_and_Resolutions.ipynb`, 
which serves as an example of how one might use the outputs of `nrCascadeSim`. 
In this example, the yield of the simulated data is taken, 
a mathematical representation of the detector's resolution is applied,
and the resulting spectra are plotted.

## Instructions

*These instructions are provided in the notebook as well if you would prefer to see them in context.*

The notebook calls a pre-generated file `data/file.root`, which will re-generate the provided image exactly if left unchanged.
If you would like to see the full functionality of nrCascadeSim, however, you can replace this file or generate a new one and alter the path in the library.
You can replace the file by running the script `generate.sh` or by the command below from the repository's top level:
    `bin/realizeCascades -n 10000 -o test-example/data/file.root levelfiles/Si28_ngam_all_cascades.txt`

## Files

* `Yields_and_Resolutions.ipynb` - the notebook serving as the example described above.
* `environment.yml` - a conda environment yml file including everything needed to run this example.
* `generate.sh` - a short shell script for running a new simulation using nrCascadeSim. Note that this will overwrite the provided file.
* `requirements.txt` - a pip requirements text file including all pip packages needed to run this example, as an alternative to the yml file.
* `SorVsLin.svg` - a static image of one particular run of the simulation and analysis.
* `standard.mplstyle` - formatting parameters read by the plotting library.
* `test_local.py` - script for CI to ensure that the notebook is running without errors.
* `data` directory - contains some precompiled data for the notebook for those who do not wish to generate their own.
* `python` directory - contains libraries used for the data processing.