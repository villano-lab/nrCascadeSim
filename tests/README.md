# Tests

This directory houses the testing suite for the code. Each script in this directory is designed to
be run and return 0 on success. When a new test is added it should be added to the Travis-CI
execution list by modifying the file `.travis.yml` in the top-level of this repository.

## `realizeCascades`

The main executable for this code is `realizeCascades` and is simply tested by providing identical
input and comparing the `md5` sum of the `ROOT` output file against the expected value. This
happens inside the script `test_realizeCascades` in this directory.    
