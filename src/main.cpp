#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include "QCutBill.h"
#include "muParserX/mpParser.h"
#include <CMRMLib/CMRM.h>
using namespace mup;
using namespace std;
using namespace cmrm;

// Process has done i out of n rounds,
// and we want a bar of width w and resolution r.
static inline void loadBar(int x, int n, int r, int w, double phi)
{
#ifdef __linux__
    // Only update r times.
    if ( x % (n/r) != 0 ) return;

    // Calculuate the ratio of complete-to-incomplete.
    float ratio = x/(float)n;
    int   c     = ratio * w;

    // Show the percentage complete.
    printf("%3d%% [", (int)(ratio*100) );

    // Show the load bar.
    for (int x=0; x<c; x++)
       printf("=");

    for (int x=c; x<w; x++)
       printf(" ");

    // ANSI Control codes to go back to the
    // previous line and clear it.
    //printf("]\n\033[F\033[J");
     cout << "] Phi: " << phi;
     printf("\n\033[F\033[J");
#else
	system("cls");
	cout << 100*(x/(float)n) << " %   Phi: " << phi << endl;
#endif
}

// Parser
mup::ParserX parser;
mup::Value xVal;
void Parser_Init(string expr)
{
#ifdef UNICODE
    std::wstring wexpr = std::wstring(expr.begin(), expr.end());
#else
	string wexpr = expr;
#endif
    parser.SetExpr(wexpr);
    parser.DefineVar(_T("x"), mup::Variable(&xVal));
}
double Parser_Eval(double x)
{
    xVal = x;
    return parser.Eval().GetFloat();
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
        // Billiard params.
        BillParams param;
        double Min_Phi, Max_Phi, Phi_Step;
        string perturbation;

        cp.DblArgToVar(param.W, "Omega", 2.0);
        cp.DblArgToVar(Min_Phi, "Min_phi", -1.57);
        cp.DblArgToVar(Max_Phi, "Max_phi", 1.57);
        cp.DblArgToVar(Phi_Step, "Phi_step", 0.3);
        cp.DblArgToVar(param.delta, "Delta", 10.0);
        cp.DblArgToVar(param.gridSize, "Grid_size", 0.005);
        cp.BoolArgToVar(param.closed, "Closed", true);
        cp.IntArgToVar(param.iter, "Iterations", 5000);
        cp.StringArgToVar(perturbation, "Perturbation", "sin(x)^2");

        // Color output.
        Palette palette;
		wxGradient grad;
		bool relative, colorOutput;
		int gradMax;
		string colorPrefix, gradString;
		cp.BoolArgToVar(colorOutput, "Color_Output", false);
		cp.IntArgToVar(gradMax, "Palette_Size", 300);
		cp.StringArgToVar(colorPrefix, "Color_Prefix", "out_");
		cp.StringArgToVar(gradString, "Color", "rgb(0,0,0);rgb(255,255,255);");
		cp.BoolArgToVar(relative, "Relative_Color", true);

		grad.setMin(0);
		grad.setMax(gradMax);
		grad.fromString(gradString);
		palette.SetGradient(grad);
		palette.SetRelativeColor(relative);

		// Csv output.
		bool csvOutput, skipEmpty;
		string csvPath, csvPrefix;
		cp.BoolArgToVar(csvOutput, "Csv_Output", false);
		cp.StringArgToVar(csvPrefix, "Csv_Prefix", "out_");
		cp.BoolArgToVar(skipEmpty, "Skip_Empty", false);

		// Plotting.
		bool plotCsv, makeVideo;
		string plotter;
		cp.BoolArgToVar(plotCsv, "Plot_Csv", false);
		cp.BoolArgToVar(makeVideo,"Make_Video", false);
		cp.StringArgToVar(plotter, "Plotter", "Mathematica");

		// Format output directories.
		system("sh Resources/Scripts/create_directories.sh");

		// Create log.
		ofstream file;
		file.open("log.txt");
		file << "QBill log - ";
		file << cmrm::GetCurrentDateTime() << endl;
		file << "-------------------------------" << endl;

        // Start simulation.
        Parser_Init(perturbation);
        int i = 0, steps = (int)((Max_Phi-Min_Phi)/Phi_Step);
        cout << "Simulating..." << endl;
        for(double phi = Min_Phi; phi <= Max_Phi; phi += Phi_Step)
        {
            if(Min_Phi != Max_Phi) loadBar(i, steps, steps, 30, phi);
            param.phi = phi;
            Grid qb = QuantumBill(param, &Parser_Eval, true, &file);

            if(colorOutput)
            {
            	int gSize = qb.GetSize();
				double maxAmplitude = 0;
				double minAmplitude = 1e50;
				if(palette.GetRelativeColorMode())
				{
					for(int i=0; i<gSize; i++)
					{
						for(int j=0; j<gSize; j++)
						{
							if(qb[i][j].m_amplitude > maxAmplitude) maxAmplitude = qb[i][j].m_amplitude;
							if(qb[i][j].m_amplitude < minAmplitude && qb[i][j].m_amplitude != 0.0) minAmplitude = qb[i][j].m_amplitude;
						}
					}
					palette.maxValue = maxAmplitude;
					palette.minValue = minAmplitude;
				}

				// Draw bitmap.
				cmrm::Matrix<Color> out(gSize, gSize);
				for(int i=0; i<gSize; i++)
				{
					for(int j=0; j<gSize; j++)
					{
						if(palette.GetRelativeColorMode())
							out[i][j] = palette.CalcColor(qb[j][i].m_amplitude);
						else
							out[i][j] = palette.CalcColor((qb[j][i].m_amplitude+1)*30);
					}
				}

				string outName = colorPrefix;
				outName += num_to_string(param.phi);
				outName += ".bmp";
				BMPPlot(out, outName);
            }
            if(csvOutput)
            {
            	string outName = csvPrefix;
            	outName += num_to_string(param.phi);
            	outName += ".csv";
            	CsvWriter cw;
            	cw.Open(outName);

            	int gSize = qb.GetSize();
            	for(unsigned int i=0; i<gSize; i++)
            	{
            		for(unsigned int j=0; j<gSize; j++)
					{
            			if(skipEmpty)
            			{
            				if(qb[i][j].m_amplitude != 0)
            					cw.Push(qb[i][j].m_x, qb[i][j].m_y, qb[i][j].m_amplitude);
            			}
            			else
            				cw.Push(qb[i][j].m_x, qb[i][j].m_y, qb[i][j].m_amplitude);
					}
            	}
            	cw.Close();
            }

            i++;
        }
        cout << "Done." << endl;
        file.close();

        if(plotCsv && csvOutput)
        {
        	cout << "Plotting csv.. " << endl;
        	if(plotter == "Mathematica")
        		system("math -script Resources/Scripts/Plotter.m");

        	if(makeVideo)
        		system("sh Resources/Scripts/make_video.sh");

        	cout << "Done." << endl;
        }
    }
    else
    {
        cout << "Error: Could not open file." << endl;
        cout << "Usage: QBill [param_file.ini]" << endl;
        return 1;
    }

    return 0;
}
