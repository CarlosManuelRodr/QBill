/**
* @file Tests.h
* @brief Tests for the Quantum billiard.
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
#include "QCutBill.h"

void Test_Grid();        ///< Test Grid operators.
void Test_Quantum(QBillParams q_params, BillParams params);    ///< Test Quantum billiard error.
void Plot_Quantum_Error(QBillParams q_params, BillParams params);

#endif
