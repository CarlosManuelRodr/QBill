#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include "QCutBill.h"
#include "muParserX/mpParser.h"
#include "ConfigParser.h"
#include "CsvParser.h"
#include "Matrix.h"
#include "Plotter.h"
#include "System.h"
#include "Tests.h"
using namespace std;

// Parser
/*
 * x: Evaluation distance.
 * Iter: Total number of iterations.
 * i: Iteration index.
 */
mup::ParserX parser;
mup::Value xVal, phiVal;
void Parser_Init(string expr)
{
#ifdef UNICODE
    std::wstring wexpr = std::wstring(expr.begin(), expr.end());
#else
	string wexpr = expr;
#endif
    parser.SetExpr(wexpr);
    parser.DefineVar(L"x", mup::Variable(&xVal));
    parser.DefineVar(L"phi", mup::Variable(&phiVal));
}
double Parser_Eval(double x, double phi)
{
    xVal = x;
    phiVal = phi;
    return parser.Eval().GetFloat();
}

// Writing functions.
void Write_Csv(Grid& gd, double omega, string phi, string prefix, bool skip_empty)
{
	string outName = string("W=") + num_to_string(omega) + string("/") + prefix;
	outName += phi;
	outName += ".csv";
	CsvWriter cw;
	cw.Open(outName);

	int gSize = gd.GetSize();
	for(unsigned int i=0; i<gSize; i++)
	{
		for(unsigned int j=0; j<gSize; j++)
		{
			if(skip_empty)
			{
				if(gd[i][j].m_amplitude != 0)
					cw.Push(gd[i][j].m_x, gd[i][j].m_y, gd[i][j].m_amplitude);
			}
			else
				cw.Push(gd[i][j].m_x, gd[i][j].m_y, gd[i][j].m_amplitude);
		}
	}
	cw.Close();
}
void Write_Bitmap(Grid& gd, double omega, string phi, string prefix)
{
	// Draw bitmap.
	int g_size = gd.GetSize();
	Matrix<Color> out(g_size, g_size);
	Grid plot_qb = gd.Normalize(60);

	for(int i=0; i<g_size; i++)
	{
		for(int j=0; j<g_size; j++)
		{
			out[i][j] = CalcColor(plot_qb[j][i].m_amplitude);
		}
	}

	string outName = string("W=") + num_to_string(omega) + string("/") + prefix;
	outName += phi;
	outName += ".bmp";
	BMPPlot(out, outName);
}

void Write_Abs_Bitmap(Grid& gd, double omega, string phi, string prefix)
{
	// Draw bitmap.
	int g_size = gd.GetSize();
	Matrix<Color> out(g_size, g_size);
	Grid plot_qb = gd.Absolute().Normalize(45);

	for(int i=0; i<g_size; i++)
	{
		for(int j=0; j<g_size; j++)
		{
			out[i][j] = CalcColor(plot_qb[j][i].m_amplitude);
		}
	}

	string outName = string("W=") + num_to_string(omega) + string("/") + prefix;
	outName += phi;
	outName += ".bmp";
	BMPPlot(out, outName);
}


int main(int argc, char *argv[])
{
    // Load param file
    string paramFile;
    if(argc == 1)
    {
        paramFile = "Resources/params.ini";
    }
    else
    {
        paramFile = string(argv[1]);
    }
    ConfigParser cp(paramFile);
    if(cp.FileOpened())
    {
    	// Debug mode.
    	bool Make_Tests;
    	cp.BoolArgToVar(Make_Tests, "Make_Tests", false);

        // Get billiard params from file.
        BillParams param;
        double Min_Phi, Max_Phi, Phi_Step;
        string perturbation, sim_mode;
        bool Log, realistic_collision, skip_same, normalize;
        bool default_directories;

        cp.StringArgToVar(sim_mode, "Sim_Mode", "Wave");
        cp.DblArgToVar(param.W, "Omega", 2.0);
        cp.DblArgToVar(Min_Phi, "Min_phi", -1.57);
        cp.DblArgToVar(Max_Phi, "Max_phi", 1.57);
        cp.DblArgToVar(Phi_Step, "Phi_step", 0.3);
        cp.DblArgToVar(param.delta, "Delta", 10.0);
        cp.DblArgToVar(param.gridElementSize, "Grid_Element_Size", 0.005);
        cp.BoolArgToVar(param.closed, "Closed", true);
        cp.IntArgToVar(param.iter, "Iterations", 5000);
        cp.DblArgToVar(param.injection_x, "Injection_X", 1.0);
        cp.DblArgToVar(param.injection_y, "Injection_Y", 0.0);
        cp.StringArgToVar(perturbation, "Perturbation", "sin(x)^2");
        cp.BoolArgToVar(realistic_collision, "Realistic_Collision", false);
        cp.BoolArgToVar(skip_same, "Skip_Same", false);
        cp.BoolArgToVar(normalize, "Normalize", false);
        cp.BoolArgToVar(Log, "Log", false);
        cp.BoolArgToVar(default_directories, "Default_Directories", true);

        // Color output.
		bool colorOutput, makeColorVideo, normalizeColor;
		string colorPrefix, colorPlotter;
		cp.BoolArgToVar(colorOutput, "Color_Output", false);
		cp.BoolArgToVar(makeColorVideo, "Make_Color_Video", false);
		cp.BoolArgToVar(normalizeColor, "Normalize_Color", false);
		cp.StringArgToVar(colorPrefix, "Color_Prefix", "img_");
		cp.StringArgToVar(colorPlotter, "Color_Plotter", "Internal");

		// Absolute output.
		bool absoluteOutput;
		string absColorPlotter, absColorPrefix;
		cp.BoolArgToVar(absoluteOutput, "Absolute_Output", false);
		cp.StringArgToVar(absColorPlotter, "Abs_Color_Plotter", "Internal");
		cp.StringArgToVar(absColorPrefix, "Abs_Color_Prefix", "img_");

		// Csv output.
		bool csvOutput, skipEmpty;
		string csvPath, csvPrefix;
		cp.BoolArgToVar(csvOutput, "Csv_Output", false);
		cp.StringArgToVar(csvPrefix, "Csv_Prefix", "out_");
		cp.BoolArgToVar(skipEmpty, "Skip_Empty", true);

		// Plotting.
		bool plotCsv, makeVideo;
		cp.BoolArgToVar(plotCsv, "Plot_Csv", false);
		cp.BoolArgToVar(makeVideo,"Make_Video", false);

		// Check options and overwrite parameter with argument if there is any.
		if(argc > 2)
		{
			string arg;
			for(int i= 2; i < argc; i++)
			{
				arg = argv[i];
				arg = rem_whitespaces(arg);

				// Param omega.
				if(is_there_substr(arg, string("W=")))
				{
					param.W = string_to_num<double>(arg.substr(arg.find_first_of("=") + 1));
				}

				// Param perturbation.
				if(is_there_substr(arg, string("Perturbation=")))
				{
					perturbation = arg.substr(arg.find_first_of("=") + 1);
				}

				// Param iteration.
				if(is_there_substr(arg, string("Iterations=")))
				{
					param.iter = string_to_num<int>(arg.substr(arg.find_first_of("=") + 1));
				}

				if(is_there_substr(arg, string("Abs_Color_Prefix")))
				{
					absColorPrefix = arg.substr(arg.find_first_of("=") + 1);
				}

				if(is_there_substr(arg, string("--silent")) || is_there_substr(arg, string("-s")))
				{
					silent = true;
				}
			}
		}
		if(colorOutput || absoluteOutput || plotCsv) csvOutput = true;

		// Tests.
		if(Make_Tests)
		{
			Test_Grid();
			Test_Quantum();
			Plot_Quantum_Error(param.iter, 2.0/param.gridElementSize);
			return 0;
		}

		// Format output directories.
		if(default_directories) { Setup_Directories(param.W); }

		// Create log.
		ofstream file;
		if(Log)
		{
			file.open("Log/log.txt");
			file << "QBill log - ";
			file << GetCurrentDateTime() << endl;
			file << "-------------------------------" << endl;
		}

        // Start simulation.
		unsigned int grid_size = 2.0/param.gridElementSize;
		QBillParams q_params;
		q_params.min_phi = Min_Phi;
		q_params.max_phi = Max_Phi;
		q_params.phi_step = Phi_Step;
		q_params.grid_size = grid_size;
		q_params.disturbance = &Parser_Eval;
		q_params.real_collision = realistic_collision;
		q_params.skip_same = skip_same;
		q_params.normalize = normalize;
		q_params.log = Log;
        Parser_Init(perturbation);
        int i = 0, steps = (int)((Max_Phi-Min_Phi)/Phi_Step);
        Print(param.W, "Simulating...");

        if(sim_mode == "Linear")
        {
			for(double phi = Min_Phi; phi <= Max_Phi; phi += Phi_Step)
			{
				if( (Min_Phi != Max_Phi) && !silent) { LoadBar(i, steps, steps, 30, phi); }
				param.phi = phi;

				Simres result = Sim_Billiard(param);	// Classical trayectories.
				Grid qb = Quantum_Bill(result, q_params, &file);	// Quantum grid.

				string str_phi = num_to_string(phi);

				// Standard output.
				if(csvOutput) { Write_Csv(qb, param.W, str_phi, csvPrefix, skipEmpty); }

				// Internal color plot.
				if(colorOutput && colorPlotter == "Internal") { Write_Bitmap(qb, param.W, str_phi, colorPrefix); }

				// Internal abs plot.
				if(absoluteOutput && absColorPlotter == "Internal") { Write_Abs_Bitmap(qb, param.W, str_phi, absColorPrefix); }

				i++;
			}

			// Mathematica color plot.
			string command;
			if(colorOutput && colorPlotter == "Mathematica")
			{
				Print(param.W, "Plotting color plots.. ");
				command = "MathematicaScript -script Resources/Scripts/colorPlotter.m ";
				command += num_to_string(param.W);
				if(normalizeColor)
					command += " True";
				else
					command += " False";

				Print(param.W, "Executting: \"" + command + "\"");
				system(command.c_str());
			}

			// Mathematica Abs Plot.
			if(absoluteOutput && absColorPlotter == "Mathematica")
			{
				Print(param.W, "Plotting absolute color plots.. ");
				command = "MathematicaScript -script Resources/Scripts/absColorPlotter.m ";
				command += num_to_string(param.W);
				Print(param.W, "Executting: \"" + command + "\"");
				system(command.c_str());
			}

			// 3D Plot. Only available in Mathematica.
			if(plotCsv && csvOutput)
			{
				Print(param.W, "Plotting csv.. ");
				command = "MathematicaScript -script Resources/Scripts/Plotter.m ";
				command += num_to_string(param.W);
				system(command.c_str());

				if(makeVideo)
				{
					command = "sh Resources/Scripts/make_video.sh ";
					command += num_to_string(param.W);
					system(command.c_str());
				}
			}

			// Create video of the color plots with FFMpeg.
			if(makeColorVideo)
			{
				command = "sh Resources/Scripts/make_color_video.sh ";
				command += num_to_string(param.W);
				Print(param.W, "Executting: " + command);
				system(command.c_str());
			}
        }
        else if(sim_mode == "Wave")
        {
        	Grid total = Quantum_Wave(param, q_params, &file);

        	if(csvOutput) { Write_Csv(total, param.W, "Total", csvPrefix, skipEmpty); }
			if(colorOutput && colorPlotter == "Internal") { Write_Bitmap(total, param.W, "Total", colorPrefix); }
			if(absoluteOutput && absColorPlotter == "Internal") { Write_Abs_Bitmap(total, param.W, "Total", absColorPrefix); }
        }
        else
        {
        	Print(param.W, "Unavailable simulation mode: " + sim_mode);
        }

        if(Log) file.close();
        Print(param.W, "Done.");
    }
    else
    {
        cout << "Error: Could not open file." << endl;
        cout << "Usage: QBill [param_file.ini]" << endl;
        return 1;
    }

    return 0;
}
