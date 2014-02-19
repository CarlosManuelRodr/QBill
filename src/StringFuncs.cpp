#include "StringFuncs.h"
#include <algorithm>
#include <cstdlib>
#include <string>
using namespace std;

#ifdef USE_WX

// Conversion.
wxString ToWxStr(std::string str)
{
	return wxString(str.c_str(), wxConvUTF8);
}
std::string ToStlStr(wxString std)
{
	return string(std.mb_str());
}
wxString wx_bool_to_string(const bool d)
{
	if(d) return wxString(wxT("True"));
	else return wxString(wxT("False"));
}

// Str utils.
bool wx_is_there_function(const wxString input)
{
	// Look for a sign outside the parenthesis.
	int signLevel = 0;
	for(unsigned int i=0; i<input.length(); i++)
	{
		if(input[i] == '(') signLevel--;
		if(input[i] == ')') signLevel++;
		if(signLevel == 0)
		{
			switch((char)input[i])
			{
			case '+':
			case '-':
			case '*':
			case '^':
			case'/': return false;
				break;
			}
		}
	}
	// Search function.
	bool pLeft = false, pRight = false;
	for(unsigned int i=0; i<input.length(); i++)
	{
		if(input[i] == '(') pLeft = true;
		if(input[i] == ')') pRight = true;
	}
	if(pLeft && pRight) return true;
	else return false;
}
bool wx_is_there_substr(const wxString cad, const wxString cadBus)
{
	int nCad, nCadBus;
	nCad=cad.length();
	nCadBus=cadBus.length();
	wxString subCad;

	for(int i=0; i<=nCad-nCadBus; i++)
	{
		subCad = cad.substr(i, nCadBus);
		if(subCad == cadBus) return true;
	}
	return false;
}

#else

// File management.
string replace_ext(const string filepath, const string newExt)
{
	// Changes file extension.
	unsigned int extDot;
	string out = filepath;
	for(unsigned int i=0; i<filepath.size(); i++)
	{
		if(filepath[i] == '.') extDot = i;
	}
	out.erase(extDot, filepath.size()-extDot);
	out += newExt;
	return out;
}
bool check_ext(const string filename, const string ext)
{
	for(unsigned int i=0; i<filename.length(); i++)
	{
		if(filename[i] == '.' && i+1 != filename.length())
		{
			if(filename.substr(i+1, filename.size()-1-i) == ext)
				return true;
		}
	}
	return false;
}

// Conversion.
string bool_to_string(const bool d)
{
	if(d) return string("True");
	else return string("False");
}

// Formatting.
string rem_whitespaces(string in)
{
	in.erase(remove(in.begin(), in.end(), ' '), in.end());
	return in;
}

// Str utils.
bool is_there_function(const string input)
{
	// Look for a sign outside the parenthesis.
	int signLevel = 0;
	for(unsigned int i=0; i<input.length(); i++)
	{
		if(input[i] == '(') signLevel--;
		if(input[i] == ')') signLevel++;
		if(signLevel == 0)
		{
			switch(input[i])
			{
			case '+':
			case '-':
			case '*':
			case '^':
			case'/': return false;
				break;
			}
		}
	}
	// Search function.
	bool pLeft = false, pRight = false;
	for(unsigned int i=0; i<input.length(); i++)
	{
		if(input[i] == '(') pLeft = true;
		if(input[i] == ')') pRight = true;
	}
	if(pLeft && pRight) return true;
	else return false;
}
bool is_there_substr(const string cad, const string cadBus)
{
	int nCad, nCadBus;
	nCad=cad.length();
	nCadBus=cadBus.length();
	string subCad;

	for(int i=0; i<=nCad-nCadBus; i++)
	{
		subCad = cad.substr(i, nCadBus);
		if(subCad == cadBus) return true;
	}
	return false;
}


#endif // USE_WX
