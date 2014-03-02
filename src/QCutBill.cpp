#include "QCutBill.h"
#include "CutBill.h"
#include <cmath>
#include <iostream>
#include <limits>
#include "StringFuncs.h"
using namespace std;

// GridElement.
GridElement::GridElement()
{
	m_amplitude = 0;
	m_time = 0;
	m_x = 0;
	m_y = 0;
	m_amprecord.push_back(0);
	m_timerecord.push_back(0);
}
void GridElement::AddDisturbance(const double value, const double time)
{
	m_amprecord.push_back(value);
	m_timerecord.push_back(time);
	m_amplitude += value;
	m_time = time;
}
void GridElement::SetCoordinates(const double x, const double y)
{
	m_x = x;
	m_y = y;
}


// Grid.
Grid::Grid(const int size)
{
	m_size = size;
	m_epsilon = 2.0/(double)m_size;

	m_minx = -1.0;
	m_maxx = 1.0;
	m_miny = -1.0;
	m_maxy = 1.0;
	m_xfactor = (m_maxx-m_minx)/(m_size-1);
	m_yfactor = (m_maxy-m_miny)/(m_size-1);

	m_grid = new GridElement*[m_size];
	for(int i=0; i<m_size; i++)
	{
		m_grid[i] = new GridElement[m_size];
	}

	for(int i=0; i<m_size; i++)
	{
		for(int j=0; j<m_size; j++)
		{
			m_grid[i][j].SetCoordinates(m_minx + i*m_xfactor, m_maxy - j*m_yfactor);
		}
	}
}
Grid::~Grid()
{
	for(int i=0; i<m_size; i++)
	{
		if(m_grid[i] != NULL)
			delete[] m_grid[i];
	}
	if(m_grid != NULL)
		delete[] m_grid;
}
GridElement** Grid::GetGrid()
{
    return m_grid;
}
GridElement& Grid::GetGridElement(const double x, const double y)
{
    int h = (x-m_minx)/m_xfactor;
    int v = (m_maxy-y)/m_yfactor;
    if(h < m_size && v < m_size)
    {
        return m_grid[h][v];
    }
    else
    {
        static GridElement gNULL;
        return gNULL;
    }
}
Coord Grid::GetCoord(const double x, const double y)
{
    return Coord((int)((x-m_minx)/m_xfactor), (int)((m_maxy-y)/m_yfactor));
}
int Grid::GetSize()
{
    return m_size;
}
Grid Grid::Normalize(double max_value)
{
	double maxAmplitude = 0;
	double minAmplitude = 0;

	for(int i=0; i<m_size; i++)
	{
		for(int j=0; j<m_size; j++)
		{
			if(m_grid[i][j].m_amplitude > maxAmplitude)
				maxAmplitude = m_grid[i][j].m_amplitude;
		}
	}

	Grid out(m_size);
	double maxValue = maxAmplitude;
	double minValue = minAmplitude;
	double diff = maxValue-minValue;

	for(int i=0; i<m_size; i++)
	{
		for(int j=0; j<m_size; j++)
		{
			out[i][j].m_amplitude = max_value*(m_grid[i][j].m_amplitude-minValue)/diff;
		}
	}
	return out;
}
Grid Grid::Absolute()
{
	// Take the absolute value of all it's elements.
	Grid out(m_size);
	for(int i=0; i<m_size; i++)
	{
		for(int j=0; j<m_size; j++)
		{
			out[i][j].m_amplitude = abs(m_grid[i][j].m_amplitude);
		}
	}
	return out;
}
GridElement*& Grid::operator[](const unsigned int pos)
{
    return m_grid[pos];
}
Grid Grid::operator+(Grid &in)
{
	unsigned int size = in.GetSize();
	Grid out(size);
	for(unsigned int i=0; i<size; i++)
	{
		for(unsigned int j=0; j<size; j++)
		{
			out[i][j].m_amplitude = m_grid[i][j].m_amplitude + in[i][j].m_amplitude;
		}
	}
	return out;
}
Grid& Grid::operator+=(Grid &in)
{
	// First we determine the temporal limit to the sum.
	double highest_time1 = 0;
	double highest_time2 = 0;
	bool empty1 = true;
	bool empty2 = true;

	unsigned int size = in.GetSize();
	for(unsigned int i=0; i<size; i++)
	{
		for(unsigned int j=0; j<size; j++)
		{
			if(m_grid[i][j].m_time > highest_time1) highest_time1 = m_grid[i][j].m_time;
			if(in[i][j].m_time > highest_time2) highest_time2 = in[i][j].m_time;
			if(m_grid[i][j].m_time != 0.0) empty1 = false;
			if(in[i][j].m_time != 0.0) empty2 = false;
		}
	}

	double time_limit;
	if(highest_time1 > highest_time2) time_limit = highest_time2;
	else time_limit = highest_time1;

	if(empty1) time_limit = highest_time2;
	if(empty2) time_limit = highest_time1;

	// Clear grid for unrequired times.
	for(unsigned int i=0; i<size; i++)
	{
		for(unsigned int j=0; j<size; j++)
		{
			if(in[i][j].m_time > time_limit)
			{
				in[i][j].m_amplitude = 0.0;
				in[i][j].m_time = 0.0;
			}
			if(m_grid[i][j].m_time > time_limit)
			{
				m_grid[i][j].m_amplitude = 0.0;
				m_grid[i][j].m_time = 0.0;
			}
		}
	}

	// Sum amplitudes.
	for(unsigned int i=0; i<size; i++)
	{
		for(unsigned int j=0; j<size; j++)
		{
			m_grid[i][j].m_amplitude += in[i][j].m_amplitude;
			if(in[i][j].m_time > m_grid[i][j].m_time) { m_grid[i][j].m_time = in[i][j].m_time; }
		}
	}
	return *this;
}


// Sim.
Grid Quantum_Bill(unsigned int grid_size, Simres tray, double (*disturbance)(double, double), bool real_collision,
				 bool log, ostream *out)
{
    double m,b;
    double d;
    double prevD = 0;
    double phase = 0;
    double x1,x2,y1,y2;
    double stepX, stepY;
    double tempX, tempY;
    double grid_element_size = 2.0/(double)grid_size;
    double delta = grid_element_size/10.0;
    double infty = std::numeric_limits<double>::infinity();
    Grid bGrid(grid_size);
    Coord prevCoord = Coord(infty, infty);

    unsigned int maxIter = tray.intersections.size();
    for(unsigned int i=1; i<maxIter; i++)
    {
        x1 = tray.intersections[i-1].m_x;
        x2 = tray.intersections[i].m_x;
        y1 = tray.intersections[i-1].m_y;
        y2 = tray.intersections[i].m_y;
        m = (y2-y1)/(x2-x1);
        b = y1 - m*x1;
        stepX = abs(x2-x1)*delta;
        stepY = abs(y2-y1)*delta;
        d = -1;

        if(abs(x2-x1) > 10e-6 || abs(y2-y1) > 10e-6)
        {
			if(x2 != x1)
			{
				double cuad = atan(abs(m));
				while((cuad - 1.57079) > 0) cuad -= 1.57079;

				// Slope < 45 degrees
				if(cuad < 0.78539)
				{
					if(x2 > x1)
					{
						for(double xPos = x1; xPos < x2; xPos += stepX)
						{
							tempY = m*xPos + b;
							if(bGrid.GetCoord(xPos, tempY) != prevCoord)
							{
								d = sqrt(pow(xPos-x1, 2.0) + pow(tempY-y1, 2.0));
								prevCoord = bGrid.GetCoord(xPos, tempY);
								bGrid.GetGridElement(xPos, tempY).AddDisturbance(disturbance(d + prevD, phase), d + prevD);
							}
						}
					}
					else
					{
						for(double xPos = x1; xPos > x2; xPos -= stepX)
						{
							tempY = m*xPos + b;
							if(bGrid.GetCoord(xPos, tempY) != prevCoord)
							{
								d = sqrt(pow(xPos-x1, 2.0) + pow(tempY-y1, 2.0));
								prevCoord = bGrid.GetCoord(xPos, tempY);
								bGrid.GetGridElement(xPos, tempY).AddDisturbance(disturbance(d + prevD, phase), d + prevD);
							}
						}
					}
				}
				// Slope > 45 degrees
				else
				{
					if(y2 > y1)
					{
						for(double yPos = y1; yPos < y2; yPos += stepY)
						{
							tempX = (yPos-b)/m;
							if(bGrid.GetCoord(tempX, yPos) != prevCoord)
							{
								d = sqrt(pow(tempX-x1, 2.0) + pow(yPos-y1, 2.0));
								prevCoord = bGrid.GetCoord(tempX, yPos);
								bGrid.GetGridElement(tempX, yPos).AddDisturbance(disturbance(d + prevD, phase), d + prevD);
							}
						}
					}
					else
					{
						for(double yPos = y1; yPos > y2; yPos -= stepY)
						{
							tempX = (yPos-b)/m;
							if(bGrid.GetCoord(tempX, yPos) != prevCoord)
							{
								d = sqrt(pow(tempX-x1, 2.0) + pow(yPos-y1, 2.0));
								prevCoord = bGrid.GetCoord(tempX, yPos);
								bGrid.GetGridElement(tempX, yPos).AddDisturbance(disturbance(d + prevD, phase), d + prevD);
							}
						}
					}
				}
				prevD += sqrt(pow(x2-x1, 2.0) + pow(y2-y1, 2.0));
			}
			// Slope -> Infinity. Vertical path.
			else
			{
				if(y1 != y2)
				{
					if(y2 > y1)
					{
						for(double yPos = y1; yPos < y2; yPos += stepY)
						{
							if(bGrid.GetCoord(x1, yPos) != prevCoord)
							{
								d = abs(yPos-y1);
								prevCoord = bGrid.GetCoord(x1, yPos);
								bGrid.GetGridElement(x1, yPos).AddDisturbance(disturbance(d + prevD, phase), d + prevD);
							}
						}
					}
					else
					{
						for(double yPos = y1; yPos > y2; yPos -= stepY)
						{
							if(bGrid.GetCoord(x1, yPos) != prevCoord)
							{
								d = abs(yPos-y1);
								prevCoord = bGrid.GetCoord(x1, yPos);
								bGrid.GetGridElement(x1, yPos).AddDisturbance(disturbance(d + prevD, phase), d + prevD);
							}
						}
					}
					prevD += abs(y2-y1);
				}
			}

			if(log)
			{
				if(d < 0)
				{
					*out << "[i=" << num_to_string(i) << ", x1=" << num_to_string(x1)
						 << ", x2=" << num_to_string(x2) << ", y1=" << num_to_string(y1)
						 << ", y2=" << num_to_string(y2) << "]  Negative d: " << num_to_string(d)
						 << endl;

				}
			}

			// Phase change on realistic collision.
			if(real_collision) phase += pi;
        }
        else
        {
        	if(log)
        	{
        		*out << "[i=" << num_to_string(i) << ", x1=" << num_to_string(x1)
					 << ", x2=" << num_to_string(x2) << ", y1=" << num_to_string(y1)
					 << ", y2=" << num_to_string(y2) << "]  Very small collision"
					 << endl;

        	}
        }
    }
    return bGrid;
}
