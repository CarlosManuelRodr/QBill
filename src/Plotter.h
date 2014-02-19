/**
* @file Plotter.h
* @brief Functions for plotting data.
*
* @copyright GNU Public License v3
* @author Carlos Manuel Rodriguez y Martinez
* @date 8/13/2013
*
* This file is part of CMRMLib: https://github.com/cmrm/CMRMLib
*/

#ifndef _PLOTTER
#define _PLOTTER

#include "BmpWriter.h"
#include "Matrix.h"
#include "Types.h"
#include <vector>
#include <string>

typedef class BMPPixel Color;

// Auxiliar.
Color CalcColor(unsigned int colorNum);

// Functions.
void BMPPlot(const std::vector<double> data, const std::string filename);
void BMPPlot(const std::vector<Coord> data, const std::string filename);
void BMPPlot(Matrix<Color> &data, const std::string filename);

#endif
