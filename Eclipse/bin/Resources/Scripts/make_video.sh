#!/bin/bash

avconv -y -f image2 -i out/plots/img%d.png -r 25 -b 65536k output.mp4