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
	bool operator==(const Coord& other);
	bool operator!=(const Coord& other);
};


#endif
