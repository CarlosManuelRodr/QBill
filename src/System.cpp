#include "System.h"
#include <cassert>
#include <cstdlib>
#include <fstream>
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

string format_inside_quotes(const string in)
{
    return string("\"") + in + "\"";
}


// System calls.
void mkdir(const string arg)
{
#ifdef __linux__
    string exec = string("mkdir -p \"") + arg + "\"";
    int r = system(exec.c_str()); assert(r >= 0);
#elif _WIN32
	string exec = string("mkdir \"") + format_path(arg) + "\"";
	int r = system(exec.c_str()); assert(r >= 0);
#endif
}
void rm(const string arg)
{
#ifdef __linux__
    string exec = string("rm -f ") + format_inside_quotes(arg);
    int r = system(exec.c_str()); assert(r >= 0);
#elif _WIN32
    std::ifstream infile(arg);
    if(infile.good())
    {
		infile.close();
		string exec = string("del ") + format_path(format_inside_quotes(arg));
		int r = system(exec.c_str()); assert(r >= 0);
    }
#endif
}

// Directories and files.
bool directory_exist( string dir )
{
	const char* pzPath = dir.c_str();
#ifdef __linux__
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
#elif _WIN32
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
