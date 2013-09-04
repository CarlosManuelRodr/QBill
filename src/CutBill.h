#include <vector>
#include <CMRMLib/CMRM.h>

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
	std::vector<cmrm::Coord> intersections;
};

simres sim_billiard(BillParams param);
simres simular(bool log,bool cerrado, bool silent);

#endif
