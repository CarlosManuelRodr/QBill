/**
* @file QCutBill.h
* @brief Quantum cut billiard simulation classes and functions.
*
* @copyright GNU Public License v3
* @author Carlos Manuel Rodriguez y Martinez
* @date 10/10/2013
*
* This file is part of QBill: https://github.com/cmrm/QBill
*/

#pragma once
#ifndef _QCUTBILL
#define _QCUTBILL

#include <vector>
#include <CMRMLib/CMRM.h>
#include "Palette.h"
#include "CutBill.h"

/**
* @class GridElement
* @brief Element of the grid. Stores the actual data retrieved from the simulation.
*/

class GridElement
{
public:
    double m_amplitude;
    std::vector<double> m_amprecord, m_timerecord;

    double m_x, m_y;

    GridElement();
    void AddDisturbance(const double value, const double time);
    void SetCoordinates(const double x, const double y);
};


/**
* @class Grid
* @brief A grid that is used to collect data from the path of the particle.
*/

class Grid
{
    GridElement** m_grid;
    double m_epsilon;		///< Size of the grid element.
    double m_minx, m_maxx, m_miny, m_maxy;		///< Coordinates of the grid.
    double m_xfactor, m_yfactor;		///< Conversion factors: Coordinates <==> Grid element.
    int m_size;		///< Size of the grid. Number of elements in a row or column.

public:
    Grid(const double epsilon);
    GridElement** GetGrid();
    GridElement*& operator[](const unsigned int pos);
    GridElement& GetGridElement(const double x, const double y);
    cmrm::Coord GetCoord(const double x, const double y);
    int GetSize();
};

simres sim_billiard(BillParams param);		///< Launch the simulation of the trayectories on the Cut-Billiard.
Grid QuantumBill(BillParams param, double (*disturbance)(double, double, double), bool log = true, std::ostream *out = NULL);

#endif
