#!/bin/bash

for P in 0.5 0.6 0.7 0.8 0.7 1 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2
do
   ./QBill Resources/params.ini W=$P --silent&
done