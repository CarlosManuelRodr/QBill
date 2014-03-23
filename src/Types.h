/**
* @file Types.h
* @brief Data types used on QBill.
*
* @copyright GNU Public License v3
* @author Carlos Manuel Rodriguez Martinez
* @date 2/18/2014
*
* This file is part of QBill: https://github.com/cmrm/QBill
*/

#pragma once
#ifndef _TYPES

typedef char uint8;
typedef short unsigned int uint16;
typedef unsigned int uint32;
typedef int int32;

#ifndef NULL
#define NULL 0
#endif

struct Coord
{
public:
	double m_x, m_y;
	Coord();
	Coord(double x, double y);
	Coord& operator=(const Coord& other);
	bool operator==(const Coord& other);
	bool operator!=(const Coord& other);
};


#endif
