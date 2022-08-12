#!/bin/bash

#Check that resalizeCascades runs.
ROOT_LIB=`root-config --libdir`
DYLD_LIBRARY_PATH="${ROOT_LIB}" realizeCascades -n 100000 -o "output.root?reproducible=fixedname" -d 1 ../levelfiles/Si28_ngam_all_cascades_rfmt_sorted.txt 
# Check that file is uncorrupted and has data
python3 ./isempty.py
error_code=$?
if [ $error_code != 0 ]; 
then
        exit $error_code
fi

# Check md5sum for all tested inputfiles.

#Set up to generate filenames. Need to remove `.` and `/` special characters by changing them to `-`.
echo TRAVIS_OS_NAME: $TRAVIS_OS_NAME
if [ "$TRAVIS_OS_NAME" == "osx" ]; then
        #echo went into if
        export TRAVIS_DIST=$(echo $TRAVIS_OSX_IMAGE | sed 's/\./-/g' | sed 's/\//-/g')
        #echo finished sed
fi
echo TRAVIS_DIST: $TRAVIS_DIST
export ROOT_VERSION=$(echo $ROOTDIST | cut -c 7-13 | sed 's/\./-/g')
echo ROOT_VERSION: $ROOT_VERSION
for input in Si28_ngam_all_cascades_rfmt_sorted 20ne_ngam allge_ngam_WFast allge_ngam_WSlow 40ar_ngam v3_natSi; do
        DYLD_LIBRARY_PATH="${ROOT_LIB}" realizeCascades -sn 100000 -o "${input}_${ROOT_VERSION}_${TRAVIS_DIST}.root?reproducible=fixedname" -d 1 ../levelfiles/$input.txt
	md5sum "${input}_${ROOT_VERSION}_${TRAVIS_DIST}.root"
done

md5sum --check "../.github/checksums" --ignore-missing
