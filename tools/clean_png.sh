#!/bin/bash

for i in ../bin/data/*.png 
do
	echo "Cleaning up $i"
	convert "$i" -strip "$i"
done