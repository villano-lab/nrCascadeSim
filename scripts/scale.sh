#!/bin/sh

FILE=$1
FACTOR=$2

cat ${FILE} |awk -v scale=${FACTOR} '{printf("%f",$1*scale); for(i=2;i<=NF;i++){printf("\t%s",$i);} printf("\n");}'
