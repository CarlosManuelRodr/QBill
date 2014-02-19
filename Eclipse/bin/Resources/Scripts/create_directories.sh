#!/bin/bash

# Arg1: Omega parameter.

if [ -d W=$1 ]; then
	if [ -d W=$1/out/colorPlots ]; then
		rm -r W=$1/out/colorPlots
	fi
	mkdir W=$1/out/colorPlots

	if [ -d W=$1/out/nModesPlots ]; then
		rm -r W=$1/out/nModesPlots
	fi
	mkdir W=$1/out/nModesPlots

	if [ -d W=$1/out/csv ]; then
		rm -r W=$1/out/csv
	fi
	mkdir W=$1/out/csv

	if [ -d W=$1/out/plots3d ]; then
		rm -r W=$1/out/plots3d
	fi
	mkdir W=$1/out/plots3d

else
	echo "Created W=$1/out/"
	echo "Created W=$1/out/colorPlots"
	echo "Created W=$1/out/nModesPlots"
	echo "Created W=$1/out/csv"
	echo "Created W=$1/out/plots3d"

	mkdir W=$1
	mkdir W=$1/out
	mkdir W=$1/out/colorPlots
	mkdir W=$1/out/nModesPlots
	mkdir W=$1/out/csv
	mkdir W=$1/out/plots3d
fi