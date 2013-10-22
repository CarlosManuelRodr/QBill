#!/bin/bash

avconv -y -f image2 -i W=$1/out/colorPlots/img%d.png -r 25 -b 65536k W=$1/colorOutput.mp4