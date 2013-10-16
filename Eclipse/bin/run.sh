#!/bin/bash

for P in 0.5 0.6 0.7 0.8
do
   ./QBill Resources/params.ini W=$P --silent&
done