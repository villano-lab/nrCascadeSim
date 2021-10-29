#!/bin/sh
STARTDIR=$PWD
cd $0/../../
echo $PWD
bin/realizeCascades -n 10000 -o test-example/data/file2.root levelfiles/Si28_ngam_all_cascades.txt
cd $STARDIR