/**
* @file Tests.h
* @brief Quantum cut billiard simulation classes and functions.
*
* @copyright GNU Public License v3
* @author Carlos Manuel Rodriguez y Martinez
* @date 01/03/2014
*
* This file is part of QBill: https://github.com/cmrm/QBill
*/

#pragma once
#ifndef _QBILLTESTS
#define _QBILLTESTS

void Test_Grid();
void Test_Quantum();
void Plot_Quantum_Error(unsigned int iter = 500, unsigned int grid_size = 200);

#endif
