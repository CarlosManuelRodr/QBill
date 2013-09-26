#!/bin/bash

if [ -d out ]
then
	if [ -d out/color ]
	then
		rm -r out/color
	fi
	mkdir out/color

	if [ -d out/csv ]
	then
		rm -r out/csv
	fi
	mkdir out/csv

	if [ -d out/plots ]
	then
		rm -r out/plots
	fi
	mkdir out/plots

else
	echo 'Created out/'
	echo 'Created out/color'
	echo 'Created out/csv'
	echo 'Created out/plots'

	mkdir out
	mkdir out/color
	mkdir out/csv
	mkdir out/plots
fi