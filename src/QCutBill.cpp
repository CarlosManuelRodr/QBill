#include "QCutBill.h"
#include "CutBill.h"
#include <cmath>
#include <iostream>
#include <limits>
using namespace cmrm;
using namespace std;

// GridElement.
GridElement::GridElement()
{
	m_amplitude = 0;
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
}
void GridElement::SetCoordinates(const double x, const double y)
{
	m_x = x;
	m_y = y;
}


// Grid.
Grid::Grid(const double epsilon)
{
	m_epsilon = epsilon;
	m_size = 2.0/m_epsilon;

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
GridElement** Grid::GetGrid()
{
    return m_grid;
}
GridElement*& Grid::operator[](const unsigned int pos)
{
    return m_grid[pos];
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

// Sim.
Grid QuantumBill(BillParams param, double (*disturbance)(double, double, double), bool log, ostream *out)
{
    Grid bGrid(param.gridSize);
    double m,b;
    double d;
    double prevD = 0;
    double x1,x2,y1,y2;
    double stepX, stepY;
    double tempX, tempY;
    double delta = param.gridSize/10.0;
    double infty = std::numeric_limits<double>::infinity();

    simres result = sim_billiard(param);
    unsigned int maxIter = result.intersections.size();
    for(unsigned int i=1; i<maxIter; i++)
    {
        Coord prevCoord = Coord(infty, infty);

        x1 = result.intersections[i-1].m_x;
        x2 = result.intersections[i].m_x;
        y1 = result.intersections[i-1].m_y;
        y2 = result.intersections[i].m_y;
        m = (y2-y1)/(x2-x1);
        b = y1 - m*x1;
        stepX = abs((x2-x1)*delta);
        stepY = abs((y2-y1)*delta);
        d = -1;

        if(abs(x2-x1) > 10e-6 && abs(y2-y1) > 10e-6)
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
								d = sqrt(pow(xPos-x1, 2) + pow(tempY-y1, 2));
								prevCoord = bGrid.GetCoord(xPos, tempY);
								bGrid.GetGridElement(xPos, tempY).AddDisturbance(disturbance(d + prevD, (double)i, (double)maxIter), d + prevD);
							}
						}
						prevD += d;
					}
					else
					{
						for(double xPos = x1; xPos > x2; xPos -= stepX)
						{
							tempY = m*xPos + b;
							if(bGrid.GetCoord(xPos, tempY) != prevCoord)
							{
								d = sqrt(pow(xPos-x1, 2) + pow(tempY-y1, 2));
								prevCoord = bGrid.GetCoord(xPos, tempY);
								bGrid.GetGridElement(xPos, tempY).AddDisturbance(disturbance(d + prevD, (double)i, (double)maxIter), d + prevD);
							}
						}
						prevD += d;
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
								d = sqrt(pow(tempX-x1, 2) + pow(yPos-y1, 2));
								prevCoord = bGrid.GetCoord(tempX, yPos);
								bGrid.GetGridElement(tempX, yPos).AddDisturbance(disturbance(d + prevD, (double)i, (double)maxIter), d + prevD);
							}
						}
						prevD += d;
					}
					else
					{
						for(double yPos = y1; yPos > y2; yPos -= stepY)
						{
							tempX = (yPos-b)/m;
							if(bGrid.GetCoord(tempX, yPos) != prevCoord)
							{
								d = sqrt(pow(tempX-x1, 2) + pow(yPos-y1, 2));
								prevCoord = bGrid.GetCoord(tempX, yPos);
								bGrid.GetGridElement(tempX, yPos).AddDisturbance(disturbance(d + prevD, (double)i, (double)maxIter), d + prevD);
							}
						}
						prevD += d;
					}
				}
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
								bGrid.GetGridElement(x1, yPos).AddDisturbance(disturbance(d + prevD, (double)i, (double)maxIter), d + prevD);
							}
						}
						prevD += d;
					}
					else
					{
						for(double yPos = y1; yPos > y2; yPos -= stepY)
						{
							if(bGrid.GetCoord(x1, yPos) != prevCoord)
							{
								d = abs(yPos-y1);
								prevCoord = bGrid.GetCoord(x1, yPos);
								bGrid.GetGridElement(x1, yPos).AddDisturbance(disturbance(d + prevD, (double)i, (double)maxIter), d + prevD);
							}
						}
						prevD += d;
					}
				}
			}

			if(log)
			{
				if(d < 0)
				{
					*out << "[phi=" << num_to_string(param.phi) << ", i=" << num_to_string(i) << ", x1=" << num_to_string(x1)
						 << ", x2=" << num_to_string(x2) << ", y1=" << num_to_string(y1)
						 << ", y2=" << num_to_string(y2) << "]  Negative d: " << num_to_string(d)
						 << endl;

				}
			}
        }
        else
        {
        	if(log)
        	{
        		*out << "[phi=" << num_to_string(param.phi) << ", i=" << num_to_string(i) << ", x1=" << num_to_string(x1)
					 << ", x2=" << num_to_string(x2) << ", y1=" << num_to_string(y1)
					 << ", y2=" << num_to_string(y2) << "]  Very small collision"
					 << endl;

        	}
        }
    }

    return bGrid;
}
