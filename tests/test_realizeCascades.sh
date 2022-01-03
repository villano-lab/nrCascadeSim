#!/bin/bash

#for some systems like OSX
#alias md5sum='md5'

realizeCascades -n 100000 -o "output.root?reproducible=fixedname" -d 1 ../levelfiles/Si28_ngam_all_cascades_rfmt_sorted.txt 
# Check that file is uncorrupted and has data
python3 ./isempty.py
error_code=$?
if [ $error_code != 0 ]; 
then
        exit $error_code
fi
# Check md5sum
export SUMLINE=$BUILDLINE
declare -i SUMLINE
SUMLINE+=32
CHECKSUM=$(sed -n "$SUMLINE"p ../.travis/checksums | sed 's/^ *//g') #There may be an elegant solution here, but I don't know it.
echo Checksum from file: $CHECKSUM, found on line $SUMLINE
md5sum output.root
md5sum output.root | awk -v cs=$CHECKSUM '{if ($1==cs){system("rm output.root"); print"Checksum passed!"} else {system("rm output.root"); exit 1}}'
