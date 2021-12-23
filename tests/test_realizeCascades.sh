#!/bin/bash

#for some systems like OSX
#alias md5sum='md5'

realizeCascades -n 100000 -o "output.root?reproducible=fixedname" -d 1 -v2 ../levelfiles/Si28_ngam_all_cascades_rfmt_sorted.txt 
# Check that file is uncorrupted and has data
python3 ./isempty.py
# Check md5sum
echo $CHECKSUM
md5sum output.root
md5sum output.root | awk -v cs=$CHECKSUM '{if ($1==cs){system("rm output.root"); print"Checksum passed!"} else {system("rm output.root"); exit 1}}'
