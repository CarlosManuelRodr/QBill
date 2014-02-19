#include "Types.h"

// Coord.
Coord::Coord()
{
	m_x = 0;
	m_y = 0;
}
Coord::Coord(double x, double y)
{
	m_x = x;
	m_y = y;
}
bool Coord::operator==(const Coord& other)
{
    if(other.m_x == m_x && other.m_y == m_y) return true;
    else return false;
}
bool Coord::operator!=(const Coord& other)
{
    if(other.m_x == m_x && other.m_y == m_y) return false;
    else return true;
}