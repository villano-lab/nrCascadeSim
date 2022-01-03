#!/bin/bash
#This just needs to be run once on each build to get the intial values

for f in levelfiles/*cross_sections*.txt; do
	realizeCascades -sn 100000 -o "output.root?reproducible=fixedname" -d 1 "$f"
	echo $f
	md5sum output.root
done
