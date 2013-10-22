#!/bin/bash

# Arg1: Omega parameter. Arg2: Legacy color plot, 1 true, 0 false.

if [ -d W=$1 ]; then
	if [ $2 -eq 1 ]; then
		if [ -d W=$1/out/rawColor ]
		then
			rm -r W=$1/out/rawColor
		fi
		mkdir W=$1/out/rawColor
	fi

	if [ -d W=$1/out/colorPlots ]; then
		rm -r W=$1/out/colorPlots
	fi
	mkdir W=$1/out/colorPlots

	if [ -d W=$1/out/csv ]; then
		rm -r W=$1/out/csv
	fi
	mkdir W=$1/out/csv

	if [ -d W=$1/out/plots ]; then
		rm -r W=$1/out/plots
	fi
	mkdir W=$1/out/plots

else
	echo "Created W=$1/out/"
	echo "Created W=$1/out/colorPlots"
	echo "Created W=$1/out/csv"
	echo "Created W=$1/out/plots"

	mkdir W=$1
	mkdir W=$1/out
	mkdir W=$1/out/colorPlots
	mkdir W=$1/out/csv
	mkdir W=$1/out/plots

	if [ $2 -eq 1 ]; then
		mkdir W=$1/out/rawColor
		echo "Created W=$1/out/rawColor"
	fi
fi