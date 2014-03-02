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
#include <fstream>
#include "CutBill.h"
#include "Types.h"


/**
* @class GridElement
* @brief Element of the grid. Stores the actual data retrieved from the simulation.
*/

class GridElement
{
public:
    double m_amplitude;
    double m_time;
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
    unsigned int m_size;		///< Size of the grid. Number of elements in a row or column.

public:
    Grid(const int size);
    ~Grid();

    GridElement** GetGrid();
    GridElement& GetGridElement(const double x, const double y);
    Coord GetCoord(const double x, const double y);
    int GetSize();

    Grid Normalize(double max_value = 1);		///< Normalize all elements of the grid to values ranging from 0 to max_value.
    Grid Absolute();		///< Return a grid with no negatives.

    GridElement*& operator[](const unsigned int pos);
    Grid operator+(Grid &in);
    Grid& operator+=(Grid &in);
};

Grid Quantum_Bill(unsigned int grid_size, Simres tray, double (*disturbance)(double, double), bool real_collision = false,
				 bool log = false, std::ostream *out = NULL);

#endif
