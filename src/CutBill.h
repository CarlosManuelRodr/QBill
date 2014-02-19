/**
* @file CutBill.h
* @brief Implementation of the dynamics of the cut billiard.
*
* @copyright GNU Public License v3
* @author José Ramón Palacios Barreda
* @date 10/10/2013
*
* This file is part of QBill: https://github.com/cmrm/QBill
*/

#include <vector>
#include "Types.h"

#ifndef _CUTBILL
#define _CUTBILL

struct BillParams
{
    double W, phi, delta, gridSize;
    bool closed;
    int iter;
};

class simres
{
public:
	int colisiones;
	int salida;
	std::vector<Coord> intersections;
};

simres sim_billiard(BillParams param);
simres simular(bool log,bool cerrado, bool silent);

#endif
