#include "Tests.h"
#include "QCutBill.h"
#include "StringFuncs.h"
#include "CsvParser.h"
#include <iostream>
#include <cmath>
using namespace std;

// Testing aux.
double Square_Test_Sin(double x, double unused1)
{
	return sin(pi*x);
}

// Test.
void Test_Grid()
{
	Grid first = Grid(400);
	Grid last = Grid(400);

	for(unsigned int i=0; i<first.GetSize(); i++)
	{
		for(unsigned j=0; j<first.GetSize(); j++)
		{
			first[i][j].m_amplitude = 1.0;
			last[i][j].m_amplitude = 1.0;
		}
	}

	// Test sum operator.
	bool sum_ok = true;
	Grid sum_out = first + last;
	if(sum_out.GetSize() == first.GetSize())
	{
		for(unsigned int i=0; i<sum_out.GetSize(); i++)
		{
			for(unsigned j=0; j<sum_out.GetSize(); j++)
			{
				if(sum_out[i][j].m_amplitude != 2.0)
				{
					sum_ok = false;
				}
			}
		}
	}
	if(sum_ok)
		cout << "Test Grid: Sum operator OK" << endl;
	else
		cout << "Test Grid: Sum operator FAILED" << endl;

	// Compound Assignment Operator +=.
	first += last;
	sum_ok = true;
	if(sum_out.GetSize() == first.GetSize())
	{
		for(unsigned int i=0; i<sum_out.GetSize(); i++)
		{
			for(unsigned j=0; j<sum_out.GetSize(); j++)
			{
				if(first[i][j].m_amplitude != 2.0) sum_ok = false;
			}
		}
	}
	if(sum_ok)
		cout << "Test Grid: Compound assignment operator += OK" << endl;
	else
		cout << "Test Grid: Compound assignment operator += FAILED" << endl;
}
void Test_Quantum()
{
	Simres test;

	// Horizontal test.
	test.colisiones = 3;
	test.intersections.push_back(Coord(-1.0, 0.0));
	test.intersections.push_back(Coord(1.0, 0.0));
	test.intersections.push_back(Coord(-1.0, 0.0));
	Grid gd = Quantum_Bill(200, test, &Square_Test_Sin, false, false, NULL);

	bool test_ok = true;
	double max_error = 0.01;
	for(unsigned int i=0; i<gd.GetSize(); i++)
	{
		for(unsigned j=0; j<gd.GetSize(); j++)
		{
			if(abs(gd[i][j].m_amplitude) > max_error)
			{
				test_ok = false;
				//cout << gd[i][j].m_amplitude << endl;
			}
		}
	}

	if(test_ok)
		cout << "Test Quantum: Horizontal test OK" << endl;
	else
	{
		// Calculate error.
		double active_elements = 0.0;
		double values = 0.0;

		for(unsigned int i=0; i<gd.GetSize(); i++)
		{
			for(unsigned j=0; j<gd.GetSize(); j++)
			{
				if(gd[i][j].m_amplitude != 0)
				{
					active_elements++;
					values += abs(gd[i][j].m_amplitude);
				}
			}
		}

		cout << "Test Quantum: Horizontal test FAILED with mean error: " << num_to_string(values/active_elements) << endl;;
	}

	// Vertical test.
	test.colisiones = 3;
	test.intersections.clear();
	test.intersections.push_back(Coord(0.0, -1.0));
	test.intersections.push_back(Coord(0.0, 1.0));
	test.intersections.push_back(Coord(0.0, -1.0));
	//gd = Quantum_Bill(200, test, &Square_Test_Sin, false, false, NULL);
}
void Plot_Quantum_Error(unsigned int iter, unsigned int grid_size)
{
	vector<Coord> error_list;
	Simres test;
	for(unsigned int i=0; i<=iter; i++)
	{
		// Check if testeable.
		if(i % 2 == 0)
		{
			test.intersections.clear();
			for(unsigned j=0; j<=i; j++)
			{
				if(j % 2 == 0)
					test.intersections.push_back(Coord(-1.0, 0.0));
				else
					test.intersections.push_back(Coord(1.0, 0.0));
			}
			Grid gd = Quantum_Bill(grid_size, test, &Square_Test_Sin, false, false, NULL);

			// Calculate error.
			double active_elements = 0.0;
			double values = 0.0;

			for(unsigned int i=0; i<gd.GetSize(); i++)
			{
				for(unsigned j=0; j<gd.GetSize(); j++)
				{
					if(gd[i][j].m_amplitude != 0)
					{
						active_elements++;
						values += abs(gd[i][j].m_amplitude);
					}
				}
			}
			error_list.push_back(Coord(i, values/active_elements));
		}
	}

	CsvWriter cw("Log/error_horizontal.csv");
	for(unsigned int i=0; i<error_list.size(); i++)
	{
		cw.Push(error_list[i].m_x, error_list[i].m_y);
	}
	cw.Close();
}
