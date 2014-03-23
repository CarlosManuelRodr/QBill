#include "Tests.h"
#include "QCutBill.h"
#include "StringFuncs.h"
#include "CsvParser.h"
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

// Testing aux.
double diag_const = pi/sqrt(2.0);

double Square_Test_Sin(double x, double unused1)
{
	return sin(pi*x);
}
double Square_Diag_Test_Sin(double x, double unused1)
{
	return sin(diag_const*x);
}
double Square_Test_Linear(double x, double unused1)
{
	while((x - 4.0) > 0.0) { x -= 4.0; }
	if(x < 2.0) return 1.0;
	else return -1.0;
}
vector<Coord> Generate_Test_Coords(Coord first, Coord last, int iterations)
{
	vector<Coord> out;
	if(iterations % 2 == 0) iterations++;

	for(unsigned i=0; i<iterations; i++)
	{
		if(i % 2 == 0)
			out.push_back(first);
		else
			out.push_back(last);
	}
	return out;
}
bool Test_Quantum_Grid(Grid &gd)
{
	bool test_ok = true;
	double max_error = 0.01;
	for(unsigned int i=0; i<gd.GetSize(); i++)
	{
		for(unsigned j=0; j<gd.GetSize(); j++)
		{
			if(abs(gd[i][j].m_amplitude) > max_error)
			{
				test_ok = false;
			}
		}
	}
	return test_ok;
}
double Test_Quantum_Grid_Error(Grid &gd)
{
	// Calculate mean error.
	double active_elements = 0.0;
	double values = 0.0;
	for(unsigned int i=0; i<gd.GetSize(); i++)
	{
		for(unsigned j=0; j<gd.GetSize(); j++)
		{
			if(gd[i][j].m_amplitude != 0.0)
			{
				active_elements++;
				values += abs(gd[i][j].m_amplitude);
			}
		}
	}

	if(active_elements == 0.0) { active_elements++; }
	return values/active_elements;
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

	// Assignment Operator.
	bool assign_ok = true;
	Grid target(300);
	target = first;
	if(target.GetSize() == first.GetSize())
	{
		for(unsigned int i=0; i<target.GetSize(); i++)
		{
			for(unsigned j=0; j<target.GetSize(); j++)
			{
				if(target[i][j].m_amplitude != first[i][j].m_amplitude)
				{
					assign_ok = false;
				}
			}
		}
	}
	else
	{
		assign_ok = false;
	}
	target = last;
	if(target.GetSize() == last.GetSize())
	{
		for(unsigned int i=0; i<target.GetSize(); i++)
		{
			for(unsigned j=0; j<target.GetSize(); j++)
			{
				if(target[i][j].m_amplitude != last[i][j].m_amplitude)
				{
					assign_ok = false;
				}
			}
		}
	}
	else
	{
		assign_ok = false;
	}
	if(assign_ok)
		cout << "Test Grid: Assignment operator OK" << endl;
	else
		cout << "Test Grid: Assignment operator FAILED" << endl;

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

	// Test grid coordinates.
	Grid gd = Grid(400);
	double grid_element_size = 2.0/400.0;
	double delta = grid_element_size/10.0;
	double x1 = -1.0;
	double x2 = 1.0;
	double stepX = abs(x2-x1)*delta;
	Coord cd, prev;
	int repeat = 0;
	vector<int> coord_repeat;

	for(double xPos = x1; xPos <= x2; xPos += stepX)
	{
		cd = gd.GetCoord(xPos, 0.0);
		if(cd == prev || xPos == x1) { repeat++; }
		else
		{
			coord_repeat.push_back(repeat);
			repeat = 1;
		}
		prev = cd;
	}
	int min = *min_element(coord_repeat.begin(), coord_repeat.end());
	int max = *max_element(coord_repeat.begin(), coord_repeat.end());
	cout << "Test Grid: Coordinates repeated " << max-min << endl;
}
void Test_Quantum(QBillParams q_params, BillParams params)
{
	Simres test;

	// Horizontal test.
	test.intersections = Generate_Test_Coords(Coord(-1.0, 0.0), Coord(1.0, 0.0), params.iter);
	q_params.disturbance = &Square_Test_Sin;
	q_params.real_collision = false;
	q_params.log = false;
	Grid gd = Quantum_Bill(test, q_params, NULL);
	bool h_test = Test_Quantum_Grid(gd);
	double h_test_error = Test_Quantum_Grid_Error(gd);

	if(h_test)
		cout << "Test Quantum: Horizontal test OK. Mean error: " << num_to_string(h_test_error) << endl;
	else
		cout << "Test Quantum: Horizontal test FAILED with mean error: " << num_to_string(h_test_error) << endl;

	// Vertical test.
	test.intersections = Generate_Test_Coords(Coord(0.0, -1.0), Coord(0.0, 1.0), params.iter);
	gd = Quantum_Bill(test, q_params, NULL);
	bool v_test = Test_Quantum_Grid(gd);
	double v_test_error = Test_Quantum_Grid_Error(gd);

	if(v_test)
		cout << "Test Quantum: Vertical test OK. Mean error: " << num_to_string(v_test_error) << endl;
	else
		cout << "Test Quantum: Vertical test FAILED with mean error: " << num_to_string(v_test_error) << endl;

	// Diagonal north-east test.
	test.intersections = Generate_Test_Coords(Coord(0.0, 0.0), Coord(1.0, 1.0), params.iter);
	q_params.disturbance = &Square_Diag_Test_Sin;
	gd = Quantum_Bill(test, q_params, NULL);
	bool dne_test = Test_Quantum_Grid(gd);
	double dne_test_error = Test_Quantum_Grid_Error(gd);

	if(dne_test)
		cout << "Test Quantum: Diagonal north-east test OK. Mean error: " << num_to_string(dne_test_error) << endl;
	else
		cout << "Test Quantum: Diagonal north-east test FAILED with mean error: " << num_to_string(dne_test_error) << endl;

	// Diagonal south-east test.
	test.intersections = Generate_Test_Coords(Coord(0.0, 1.0), Coord(1.0, 0.0), params.iter);
	q_params.disturbance = &Square_Diag_Test_Sin;
	gd = Quantum_Bill(test, q_params, NULL);
	bool dse_test = Test_Quantum_Grid(gd);
	double dse_test_error = Test_Quantum_Grid_Error(gd);

	if(dse_test)
		cout << "Test Quantum: Diagonal south-east test OK. Mean error: " << num_to_string(dse_test_error) << endl;
	else
		cout << "Test Quantum: Diagonal south-east test FAILED with mean error: " << num_to_string(dse_test_error) << endl;

	// High slope.
	q_params.disturbance = &Square_Test_Sin;
	test.intersections = Generate_Test_Coords(Coord(0.0, -1.0), Coord(0.0000001, 1.0), params.iter);
	gd = Quantum_Bill(test, q_params, NULL);
	bool hs_test = Test_Quantum_Grid(gd);
	double hs_test_error = Test_Quantum_Grid_Error(gd);

	if(hs_test)
		cout << "Test Quantum: High slope test OK. Mean error: " << num_to_string(hs_test_error) << endl;
	else
		cout << "Test Quantum: High slope test FAILED with mean error: " << num_to_string(hs_test_error) << endl;

	// Linear test.
	q_params.disturbance = &Square_Test_Linear;
	test.intersections = Generate_Test_Coords(Coord(-1.0, 0.0), Coord(1.0, 0.0), params.iter);
	gd = Quantum_Bill(test, q_params, NULL);
	bool l_test = Test_Quantum_Grid(gd);
	double l_test_error = Test_Quantum_Grid_Error(gd);

	if(l_test)
		cout << "Test Quantum: Linear test OK. Mean error: " << num_to_string(l_test_error) << endl;
	else
		cout << "Test Quantum: Linear test FAILED with mean error: " << num_to_string(l_test_error) << endl;
}
void Plot_Quantum_Error(QBillParams q_params, BillParams params)
{
	Simres test;
	q_params.disturbance = &Square_Test_Sin;
	q_params.real_collision = false;
	q_params.skip_same = false;
	q_params.log = false;

	// Horizontal test.
	vector<Coord> h_error_list;
	for(unsigned int i=0; i<=params.iter; i++)
	{
		// Check if testable.
		if(i % 2 != 0)
		{
			test.intersections = Generate_Test_Coords(Coord(-1.0, 0.0), Coord(1.0, 0.0), i);
			Grid gd = Quantum_Bill(test, q_params, NULL);
			h_error_list.push_back(Coord(i, Test_Quantum_Grid_Error(gd)));
		}
	}

	CsvWriter cw("Log/error_horizontal.csv");
	for(unsigned int i=0; i<h_error_list.size(); i++)
	{
		cw.Push(h_error_list[i].m_x, h_error_list[i].m_y);
	}
	cw.Close();
	cout << "Horizontal test error saved to: Log/error_horizontal.csv" << endl;

	// Vertical test.
	vector<Coord> v_error_list;
	for(unsigned int i=0; i<=params.iter; i++)
	{
		if(i % 2 != 0)
		{
			test.intersections = Generate_Test_Coords(Coord(0.0, -1.0), Coord(0.0, 1.0), i);
			Grid gd = Quantum_Bill(test, q_params, NULL);
			v_error_list.push_back(Coord(i, Test_Quantum_Grid_Error(gd)));
		}
	}

	cw.Open("Log/error_vertical.csv");
	for(unsigned int i=0; i<v_error_list.size(); i++)
	{
		cw.Push(v_error_list[i].m_x, v_error_list[i].m_y);
	}
	cw.Close();
	cout << "Vertical test error saved to: Log/error_vertical.csv" << endl;

	// Diagonal north-east test.
	q_params.disturbance = &Square_Diag_Test_Sin;
	vector<Coord> dne_error_list;
	for(unsigned int i=0; i<=params.iter; i++)
	{
		if(i % 2 != 0)
		{
			test.intersections = Generate_Test_Coords(Coord(0.0, 0.0), Coord(1.0, 1.0), i);
			Grid gd = Quantum_Bill(test, q_params, NULL);
			dne_error_list.push_back(Coord(i, Test_Quantum_Grid_Error(gd)));
		}
	}

	cw.Open("Log/error_diag_ne.csv");
	for(unsigned int i=0; i<dne_error_list.size(); i++)
	{
		cw.Push(dne_error_list[i].m_x, dne_error_list[i].m_y);
	}
	cw.Close();
	cout << "Diagonal NE test error saved to: Log/error_diag_ne.csv" << endl;

	// Diagonal south-east test.
	q_params.disturbance = &Square_Diag_Test_Sin;
	vector<Coord> dse_error_list;
	for(unsigned int i=0; i<=params.iter; i++)
	{
		if(i % 2 != 0)
		{
			test.intersections = Generate_Test_Coords(Coord(0.0, 1.0), Coord(1.0, 0.0), i);
			Grid gd = Quantum_Bill(test, q_params, NULL);
			dse_error_list.push_back(Coord(i, Test_Quantum_Grid_Error(gd)));
		}
	}

	cw.Open("Log/error_diag_se.csv");
	for(unsigned int i=0; i<dse_error_list.size(); i++)
	{
		cw.Push(dse_error_list[i].m_x, dse_error_list[i].m_y);
	}
	cw.Close();
	cout << "Diagonal SE test error saved to: Log/error_diag_se.csv" << endl;

	// Linear test.
	q_params.disturbance = &Square_Test_Linear;
	vector<Coord> l_error_list;
	for(unsigned int i=0; i<=params.iter; i++)
	{
		if(i % 2 != 0)
		{
			test.intersections = Generate_Test_Coords(Coord(-1.0, 0.0), Coord(1.0, 0.0), i);
			Grid gd = Quantum_Bill(test, q_params, NULL);
			l_error_list.push_back(Coord(i, Test_Quantum_Grid_Error(gd)));
		}
	}

	cw.Open("Log/error_diag_l.csv");
	for(unsigned int i=0; i<l_error_list.size(); i++)
	{
		cw.Push(l_error_list[i].m_x, l_error_list[i].m_y);
	}
	cw.Close();
	cout << "Linear test error saved to: Log/error_diag_l.csv" << endl;
}
