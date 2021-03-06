#include "QCutBill.h"
#include "CutBill.h"
#include <cmath>
#include <iostream>
#include <limits>
#include "StringFuncs.h"
#include "System.h"
using namespace std;

// GridElement.
GridElement::GridElement()
{
    m_amplitude = 0;
    m_time = 0;
    m_x = 0;
    m_y = 0;
    m_assignations = 0;
    m_amprecord.push_back(0);
    m_timerecord.push_back(0);
}
void GridElement::AddDisturbance(const double value, const double time)
{
    m_amprecord.push_back(value);
    m_timerecord.push_back(time);
    m_amplitude += value;
    m_time = time;
    m_assignations++;
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
    m_xfactor = (m_maxx-m_minx)/(m_size-1.0);
    m_yfactor = (m_maxy-m_miny)/(m_size-1.0);

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
void Grid::AssignNorm()
{
    double norm;
    for(int i=0; i<m_size; i++)
    {
        for(int j=0; j<m_size; j++)
        {
            norm = (double)m_grid[i][j].m_assignations;
            if(m_grid[i][j].m_amplitude != 0) { m_grid[i][j].m_amplitude /= norm; }
        }
    }
}
GridElement** Grid::GetGrid()
{
    return m_grid;
}
GridElement& Grid::GetGridElement(const double x, const double y)
{
    int h = floor((x-m_minx)/m_xfactor);
    int v = floor((m_maxy-y)/m_yfactor);
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
Coord<int> Grid::GetGridCoord(const double x, const double y)
{
    return Coord<int>(floor((x-m_minx)/m_xfactor), floor((m_maxy-y)/m_yfactor));
}
Coord<double> Grid::GetBilliardCoord(const int x, const int y)
{
    return Coord<double>(m_minx + x*m_xfactor, m_maxy - y*m_yfactor);
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
Grid& Grid::operator=(const Grid& other)
{
    if(m_size != other.m_size)
    {
        // Delete grid.
        for(int i=0; i<m_size; i++)
        {
            if(m_grid[i] != NULL)
                delete[] m_grid[i];
        }
        if(m_grid != NULL)
            delete[] m_grid;

        // Generate a new one.
        m_size = other.m_size;
        m_epsilon = other.m_epsilon;

        m_minx = -1.0;
        m_maxx = 1.0;
        m_miny = -1.0;
        m_maxy = 1.0;
        m_xfactor = (m_maxx-m_minx)/(m_size-1.0);
        m_yfactor = (m_maxy-m_miny)/(m_size-1.0);

        m_grid = new GridElement*[m_size];
        for(int i=0; i<m_size; i++)
        {
            m_grid[i] = new GridElement[m_size];
        }
    }

    // Assign.
    for(unsigned int i=0; i<m_size; i++)
    {
        for(unsigned int j=0; j<m_size; j++)
        {
            m_grid[i][j].m_x = other.m_grid[i][j].m_x;
            m_grid[i][j].m_y = other.m_grid[i][j].m_y;
            m_grid[i][j].m_amplitude = other.m_grid[i][j].m_amplitude;
            m_grid[i][j].m_time = other.m_grid[i][j].m_time;
            m_grid[i][j].m_assignations = other.m_grid[i][j].m_assignations;
            m_grid[i][j].m_amprecord = other.m_grid[i][j].m_amprecord;
            m_grid[i][j].m_timerecord = other.m_grid[i][j].m_timerecord;
        }
    }
    return *this;
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

    // Sum amplitudes.
    for(unsigned int i=0; i<size; i++)
    {
        for(unsigned int j=0; j<size; j++)
        {
            m_grid[i][j].m_amplitude += in[i][j].m_amplitude;
            m_grid[i][j].m_assignations += in[i][j].m_assignations;
        }
    }
    return *this;
}


// Sim.
Grid Quantum_Bill(Simres tray, QBillParams q_params, std::ostream *out)
{
    double m,b;
    double d;
    double prevD = 0;
    double phase = 0;
    double x1,x2,y1,y2;
    double stepX, stepY;
    double tempX, tempY;
    double grid_element_size = 2.0/(double)q_params.grid_size;
    double delta = grid_element_size/10.0;
    double infty = std::numeric_limits<int>::infinity();
    Grid bGrid(q_params.grid_size);
    Coord<int> prevCoord = Coord<int>(infty, infty);

    unsigned int maxIter = tray.intersections.size();
    for(unsigned int i=1; i<maxIter; i++)
    {
        x1 = tray.intersections[i-1].m_x;
        x2 = tray.intersections[i].m_x;
        y1 = tray.intersections[i-1].m_y;
        y2 = tray.intersections[i].m_y;

        if(q_params.bresenham)
        {
            Coord<int> grid_coord1, grid_coord2;
            Coord<double> billiard_coord1, billiard_coord2;
            int grid_x1, grid_x2, grid_y1, grid_y2;
            grid_coord1 = bGrid.GetGridCoord(x1, y1);
            grid_coord2 = bGrid.GetGridCoord(x2, y2);
            grid_x1 = grid_coord1.m_x;
            grid_y1 = grid_coord1.m_y;
            grid_x2 = grid_coord2.m_x;
            grid_y2 = grid_coord2.m_y;

            int sx, sy;
            int x = grid_x1, y = grid_y1;
            int dx = abs(grid_x2-grid_x1);
            int dy = abs(grid_y2-grid_y1);

            if (grid_x1 < grid_x2) { sx = 1; }
            else { sx = -1; }

            if (grid_y1 < grid_y2) { sy = 1; }
            else { sy = -1; }

            int err = dx-dy;

            while(x != grid_x2 || y != grid_y2)
            {
                billiard_coord1 = bGrid.GetBilliardCoord(grid_x1, grid_y1);        // Initial coordinate.
                billiard_coord2 = bGrid.GetBilliardCoord(x, y);                    // Current coordinate.
                d = sqrt(pow(billiard_coord2.m_x - billiard_coord1.m_x, 2.0) + pow(billiard_coord2.m_y - billiard_coord1.m_y, 2.0));
                if(x != grid_x1 || i == 1) { bGrid[x][y].AddDisturbance(q_params.disturbance(d + prevD, phase), d + prevD); }

                int e2 = 2*err;

                if (e2 > -dy)
                {
                    err = err - dy;
                    x = x + sx;
                }
                if (e2 < dx)
                {
                    err = err + dx;
                    y = y + sy;
                }
            }
            prevD += sqrt(pow(x2-x1, 2.0) + pow(y2-y1, 2.0));

            // Phase change on realistic collision.
            if(q_params.real_collision) phase += pi;
        }
        else
        {
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
                    if(cuad <= 0.78539)
                    {
                        if(x2 > x1)
                        {
                            for(double xPos = x1; xPos <= x2; xPos += stepX)
                            {
                                tempY = m*xPos + b;

                                if(q_params.skip_same)
                                {
                                    if(bGrid.GetGridCoord(xPos, tempY) != prevCoord)
                                    {
                                        d = sqrt(pow(xPos-x1, 2.0) + pow(tempY-y1, 2.0));
                                        prevCoord = bGrid.GetGridCoord(xPos, tempY);
                                        bGrid.GetGridElement(xPos, tempY).AddDisturbance(q_params.disturbance(d + prevD, phase), d + prevD);
                                    }
                                }
                                else
                                {
                                    d = sqrt(pow(xPos-x1, 2.0) + pow(tempY-y1, 2.0));
                                    bGrid.GetGridElement(xPos, tempY).AddDisturbance(q_params.disturbance(d + prevD, phase), d + prevD);
                                }
                            }
                        }
                        else
                        {
                            for(double xPos = x1; xPos >= x2; xPos -= stepX)
                            {
                                tempY = m*xPos + b;

                                if(q_params.skip_same)
                                {
                                    if(bGrid.GetGridCoord(xPos, tempY) != prevCoord)
                                    {
                                        d = sqrt(pow(xPos-x1, 2.0) + pow(tempY-y1, 2.0));
                                        prevCoord = bGrid.GetGridCoord(xPos, tempY);
                                        bGrid.GetGridElement(xPos, tempY).AddDisturbance(q_params.disturbance(d + prevD, phase), d + prevD);
                                    }
                                }
                                else
                                {
                                    d = sqrt(pow(xPos-x1, 2.0) + pow(tempY-y1, 2.0));
                                    bGrid.GetGridElement(xPos, tempY).AddDisturbance(q_params.disturbance(d + prevD, phase), d + prevD);
                                }
                            }
                        }
                    }
                    // Slope > 45 degrees
                    else
                    {
                        if(y2 > y1)
                        {
                            for(double yPos = y1; yPos <= y2; yPos += stepY)
                            {
                                tempX = (yPos-b)/m;

                                if(q_params.skip_same)
                                {
                                    if(bGrid.GetGridCoord(tempX, yPos) != prevCoord)
                                    {
                                        d = sqrt(pow(tempX-x1, 2.0) + pow(yPos-y1, 2.0));
                                        prevCoord = bGrid.GetGridCoord(tempX, yPos);
                                        bGrid.GetGridElement(tempX, yPos).AddDisturbance(q_params.disturbance(d + prevD, phase), d + prevD);
                                    }
                                }
                                else
                                {
                                    d = sqrt(pow(tempX-x1, 2.0) + pow(yPos-y1, 2.0));
                                    bGrid.GetGridElement(tempX, yPos).AddDisturbance(q_params.disturbance(d + prevD, phase), d + prevD);
                                }
                            }
                        }
                        else
                        {
                            for(double yPos = y1; yPos >= y2; yPos -= stepY)
                            {
                                tempX = (yPos-b)/m;

                                if(q_params.skip_same)
                                {
                                    if(bGrid.GetGridCoord(tempX, yPos) != prevCoord)
                                    {
                                        d = sqrt(pow(tempX-x1, 2.0) + pow(yPos-y1, 2.0));
                                        prevCoord = bGrid.GetGridCoord(tempX, yPos);
                                        bGrid.GetGridElement(tempX, yPos).AddDisturbance(q_params.disturbance(d + prevD, phase), d + prevD);
                                    }
                                }
                                else
                                {
                                    d = sqrt(pow(tempX-x1, 2.0) + pow(yPos-y1, 2.0));
                                    bGrid.GetGridElement(tempX, yPos).AddDisturbance(q_params.disturbance(d + prevD, phase), d + prevD);
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
                            for(double yPos = y1; yPos <= y2; yPos += stepY)
                            {
                                if(q_params.skip_same)
                                {
                                    if(bGrid.GetGridCoord(x1, yPos) != prevCoord)
                                    {
                                        d = abs(yPos-y1);
                                        prevCoord = bGrid.GetGridCoord(x1, yPos);
                                        bGrid.GetGridElement(x1, yPos).AddDisturbance(q_params.disturbance(d + prevD, phase), d + prevD);
                                    }
                                }
                                else
                                {
                                    d = abs(yPos-y1);
                                    bGrid.GetGridElement(x1, yPos).AddDisturbance(q_params.disturbance(d + prevD, phase), d + prevD);
                                }
                            }
                        }
                        else
                        {
                            for(double yPos = y1; yPos >= y2; yPos -= stepY)
                            {
                                if(q_params.skip_same)
                                {
                                    if(bGrid.GetGridCoord(x1, yPos) != prevCoord)
                                    {
                                        d = abs(yPos-y1);
                                        prevCoord = bGrid.GetGridCoord(x1, yPos);
                                        bGrid.GetGridElement(x1, yPos).AddDisturbance(q_params.disturbance(d + prevD, phase), d + prevD);
                                    }
                                }
                                else
                                {
                                    d = abs(yPos-y1);
                                    bGrid.GetGridElement(x1, yPos).AddDisturbance(q_params.disturbance(d + prevD, phase), d + prevD);
                                }
                            }
                        }
                        prevD += abs(y2-y1);
                    }
                }

                // Phase change on realistic collision.
                if(q_params.real_collision) phase += pi;
            }
            else
            {
                if(q_params.log)
                {
                    *out << "[i=" << num_to_string(i) << ", x1=" << num_to_string(x1)
                         << ", x2=" << num_to_string(x2) << ", y1=" << num_to_string(y1)
                         << ", y2=" << num_to_string(y2) << "]  Very small collision"
                         << endl;

                }
            }
        }
    }

    if(q_params.normalize) { bGrid.AssignNorm(); }
    return bGrid;
}
