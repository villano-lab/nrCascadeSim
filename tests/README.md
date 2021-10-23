# Tests

This directory houses the testing suite for the code. Each script in this directory is designed to
be run and return 0 on success. When a new test is added it should be added to the Travis-CI
execution list by modifying the file `.travis.yml` in the top-level of this repository.

## `realizeCascades`

The main executable for this code is `realizeCascades` and is simply tested by providing identical
input and comparing the `md5` sum of the `ROOT` output file against the expected value. This
happens inside the script `test_realizeCascades` in this directory.    

# Creating Tests

There are two types of tests that can be created: tests based off of a shell script, and tests
composed of Python scripts. The Python tests use the environment in the top directory
`nrCascadeSim_env.yml`. 

## Script Tests

Place script tests in the `nrCascadeSim/tests` (this) folder. Insert the execution of
the script in the `script:` section of the file `nrCascadeSim/.travis.yml`
after the command `cd tests`.

## Python Tests

Python tests are allowed in the `nrCascadeSim/example-usecase` directory. In
that directory modify the script `test_local.py` to include testing of all of
the Python functions/Jupyter notebooks that are necessary to test the
functionality.  
