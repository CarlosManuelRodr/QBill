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

const double pi = 3.14159265358979323846;

struct BillParams
{
    double W, phi, delta, gridElementSize;
    double injection_x, injection_y;
    bool closed;
    int iter;
};

class Simres
{
public:
	Simres();

	int colisiones;
	int salida;
	std::vector< Coord<double> > intersections;
	double total_distance;
};

Simres Sim_Billiard(BillParams param);
Simres Simulate(bool log,bool cerrado, bool silent);

#endif
