#!/bin/bash
#This just needs to be run once on each build to get the intial values

for f in levelfiles/*.txt; do
	realizeCascades -sn 100000 -o "output.root?reproducible=fixedname" -d 1 "$f"
	echo $f
	md5sum output.root
done
for f in levelfiles/Geant4/*.txt; do
	realizeCascades -sn 100000 -o "output.root?reproducible=fixedname" -d 1 "$f"
        echo $f
        md5sum output.root
done
