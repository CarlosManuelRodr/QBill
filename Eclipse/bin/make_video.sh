#!/bin/bash

avconv -f image2 -i out/img/img%d.png -r 25 -b 65536k output.mp4