#!/bin/bash

avconv -y -f image2 -i W=$1/out/plots/img%d.png -r 25 -b 65536k W=$1/output.mp4