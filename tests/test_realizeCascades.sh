#!/bin/bash

#for some systems like OSX
#alias md5sum='md5'

realizeCascades -n 100000 -o "output.root?reproducible=fixedname" -d 1 ../levelfiles/Si28_ngam_all_cascades_rfmt_sorted.txt 
#> /dev/null
md5sum output.root
md5sum output.root | awk '{if ($1=="2029e8b402b89d3a05d745e48f546370"){system("rm output.root"); print"Checksum passed!"} else {system("rm output.root"); exit 1}}'
# Check that file is uncorrupted and has data
python ./isempty.py