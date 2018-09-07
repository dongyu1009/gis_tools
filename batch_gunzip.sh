#!/bin/bash

filenames=`ls |grep .gz`
for filename in $filenames
do
    echo $filename
    gunzip $filename 
done
