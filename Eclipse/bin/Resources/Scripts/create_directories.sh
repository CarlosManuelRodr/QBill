#!/bin/bash

if [ -d W=$1 ]
then
	if [ -d W=$1/out/color ]
	then
		rm -r W=$1/out/color
	fi
	mkdir W=$1/out/color

	if [ -d W=$1/out/csv ]
	then
		rm -r W=$1/out/csv
	fi
	mkdir W=$1/out/csv

	if [ -d W=$1/out/plots ]
	then
		rm -r W=$1/out/plots
	fi
	mkdir W=$1/out/plots

else
	echo "Created W=$1/out/"
	echo "Created W=$1/out/color"
	echo "Created W=$1/out/csv"
	echo "Created W=$1/out/plots"

	mkdir W=$1
	mkdir W=$1/out
	mkdir W=$1/out/color
	mkdir W=$1/out/csv
	mkdir W=$1/out/plots
fi