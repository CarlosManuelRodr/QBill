#!/bin/bash

for P in 1.6 1.7 1.8 1.9
do
   ./QBill Resources/params.ini W=$P --silent&
done