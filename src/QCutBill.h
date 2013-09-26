#include <vector>
#include <CMRMLib/CMRM.h>
#include "Palette.h"
#include "CutBill.h"

struct GridElement
{
    double m_amplitude;
    double m_x, m_y;
};

class Grid
{
    GridElement** m_grid;
    double m_epsilon;
    double m_minx, m_maxx, m_miny, m_maxy;
    double m_xfactor, m_yfactor;
    int m_size;

public:
    Grid(double epsilon);
    GridElement** GetGrid();
    GridElement*& operator[](const unsigned int pos);
    GridElement& GetGridElement(double x, double y);
    cmrm::Coord GetCoord(double x, double y);
    int GetSize();
};

simres sim_billiard(BillParams param);
Grid QuantumBill(BillParams param, double (*disturbance)(double), bool log = true, std::ostream *out = NULL);
