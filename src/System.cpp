#include "System.h"
#include "StringFuncs.h"
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <iostream>
#include <limits>
using namespace std;

#ifdef _WIN32
#include <direct.h>
#include <Shlobj.h>

#elif __linux__
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#endif

// Global.
bool silent = false;

string format_inside_quotes(const string in)
{
    return string("\"") + in + "\"";
}


// System calls.
void mkdir(const string arg)
{
#if defined(__linux__) || defined(__APPLE__)
    string exec = string("mkdir -p \"") + arg + "\"";
    int r = system(exec.c_str()); assert(r >= 0);
#elif defined(_WIN32)
    string exec = string("mkdir \"") + format_path(arg) + "\"";
    int r = system(exec.c_str()); assert(r >= 0);
#endif
}
void rm(const string arg)
{
#if defined(__linux__) || defined(__APPLE__)
    string exec = string("rm -rf ") + format_inside_quotes(arg);
    int r = system(exec.c_str()); assert(r >= 0);
#elif defined(_WIN32)
    std::ifstream infile(arg);
    if(infile.good())
    {
        infile.close();
        string exec = string("del ") + format_path(format_inside_quotes(arg));
        int r = system(exec.c_str()); assert(r >= 0);
    }
#endif
}
bool directory_exist( string dir )
{
    const char* pzPath = dir.c_str();
#if defined(__linux__) || defined(__APPLE__)
    if ( pzPath == NULL) return false;

    DIR *pDir;
    bool bExists = false;

    pDir = opendir (pzPath);

    if (pDir != NULL)
    {
        bExists = true;
        (void) closedir (pDir);
    }

    return bExists;
#elif defined(_WIN32)
    DWORD ftyp = GetFileAttributesA(pzPath);
    if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;

    return false;
#endif
}
bool file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}
void Setup_Directories(double omega)
{
    string temp;
    string omega_folder = string("W=") + num_to_string(omega);
    if(directory_exist(omega_folder))
    {
        temp = omega_folder + "/out/colorPlots";
        if(directory_exist(temp)) { rm(temp); }

        temp = omega_folder + "/out/nModesPlots";
        if(directory_exist(temp)) { rm(temp); }

        temp = omega_folder + "/out/csv";
        if(directory_exist(temp)) { rm(temp); }

        temp = omega_folder + "/out/plots3d";
        if(directory_exist(temp)) { rm(temp); }

        mkdir(omega_folder + "/out/colorPlots");
        mkdir(omega_folder + "/out/nModesPlots");
        mkdir(omega_folder + "/out/csv");
        mkdir(omega_folder + "/out/plots3d");
    }
    else
    {
        mkdir(omega_folder);
        mkdir(omega_folder + "/out/colorPlots");
        mkdir(omega_folder + "/out/nModesPlots");
        mkdir(omega_folder + "/out/csv");
        mkdir(omega_folder + "/out/plots3d");
    }
}


// Printing.
const string GetCurrentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void Print(string in)
{
    if(!silent)
    {
        time_t     now = time(0);
        struct tm  tstruct;
        char        buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%X", &tstruct);
        cout << "[" << buf << "] " << in << endl;
    }
}

// Process has done i out of n rounds,
// and we want a bar of width w and resolution r.
void LoadBar(int x, int n, int r, int w, double phi)
{
#if defined(__linux__) || defined(__APPLE__)
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
    if(phi != numeric_limits<double>::infinity())
    {
        cout << "] Phi: " << phi;
    }
    else
    {
        cout << "]";
    }
    printf("\n\033[F\033[J");
#elif defined(_WIN32)
    system("cls");
    cout << 100*(x/(float)n) << " %   Phi: " << phi << endl;
#endif
}
