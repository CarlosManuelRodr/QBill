#!/bin/bash

for P in 0.5 0.7
do
   ./QBill Resources/params.ini W=$P --silent&
done