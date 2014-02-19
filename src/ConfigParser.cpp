#include "ConfigParser.h"
#include "StringFuncs.h"
#include <fstream>
#include <algorithm>
using namespace std;

ConfigParser::ConfigParser(const string filename)
{
	m_filename = filename;
	ifstream file;
	file.open(filename.c_str());

	// Analyzes the file.
	if(file.is_open())
	{
		const int BUFFER_SIZE = 200;
		char buffer[BUFFER_SIZE];
		string line;

		while(!file.eof())
		{
			file.getline(buffer, BUFFER_SIZE);
			line = buffer;
			int equalPos = -1;
			for(unsigned int i=0; i<line.length(); i++)
			{
				if(line[i] == '=')
				{
					equalPos = i;
					break;
				}
			}
			if(equalPos != -1)
			{
				string labelTxt = rem_whitespaces(line.substr(0,equalPos));
				m_labels.push_back(labelTxt);

            #ifdef _WIN32
				m_args.push_back(line.substr(equalPos+1, line.length()-equalPos-1));
            #elif __linux__
                // Search for last position.
                unsigned int lastPos = line.length();
                for(unsigned int i=equalPos+1; i<line.length(); i++)
                {
                    if(line[i] == '\r')
                    {
                        lastPos = i;
                        break;
                    }
                }
                m_args.push_back(line.substr(equalPos+1, lastPos-equalPos-1));
            #endif
			}
		}
		m_fileOpened = true;
		m_error = false;
	}
	else
	{
		m_fileOpened = false;
		m_error = true;
	}
	file.close();
}
bool ConfigParser::FileOpened()
{
	return m_fileOpened;
}
bool ConfigParser::Error()
{
	bool out = m_error;
	m_error = false;
	return out;
}
void ConfigParser::ReplaceArg(const string label, const string replaceArg)
{
	ifstream inFile;
	inFile.open(m_filename.c_str());

	// Analyze every label.
	bool found = false;
	const int BUFFER_SIZE = 200;
	char buffer[BUFFER_SIZE];
	string line, outText = "";
	while(!inFile.eof())
	{
		inFile.getline(buffer, BUFFER_SIZE);
		line = buffer;

		int equalPos = -1;
		for(unsigned int i=0; i<line.length(); i++)
		{
			if(line[i] == '=')
			{
				equalPos = i;
				break;
			}
		}
		if(equalPos != -1)
		{
			// If there is a label match replaces the argument.
			string lineLabel  = line.substr(0, equalPos);
			if(is_there_substr(lineLabel, label))
			{
				outText += line.substr(0, equalPos);
				outText += "=";
				outText += replaceArg;
				found = true;
			}
			else outText += line;
		}
		else outText += line;
		outText += "\n";
	}
	inFile.close();

	// Write output file.
	ofstream outFile;
	outFile.open(m_filename.c_str());
	for(unsigned int i=0; i<outText.length(); i++)
		outFile << outText[i];
	outFile.close();

	if(!found) m_error = true;
}

bool ConfigParser::IntArgToVar(int& myVar, const string expr, const int defaultValue)
{
	bool found = false;
	for(unsigned int i=0; i<m_labels.size(); i++)
	{
		if(m_labels[i] == expr)
		{
			myVar = string_to_num<int>(m_args[i]);
			found = true;
			break;
		}
	}
	if(!found)
	{
		myVar = defaultValue;
		m_error = true;
	}
	return found;
}
bool ConfigParser::UIntArgToVar(unsigned int& myVar, const string expr, const unsigned int defaultValue)
{
	bool found = false;
	for(unsigned int i=0; i<m_labels.size(); i++)
	{
		if(m_labels[i] == expr)
		{
			myVar = string_to_num<int>(m_args[i]);
			found = true;
			break;
		}
	}
	if(!found)
	{
		myVar = defaultValue;
		m_error = true;
	}
	return found;
}
bool ConfigParser::DblArgToVar(double& myVar, const string expr, const double defaultValue)
{
	bool found = false;
	for(unsigned int i=0; i<m_labels.size(); i++)
	{
		if(m_labels[i] == expr)
		{
			myVar = string_to_num<double>(m_args[i]);
			found = true;
			break;
		}
	}
	if(!found)
	{
		myVar = defaultValue;
		m_error = true;
	}
	return found;
}
bool ConfigParser::BoolArgToVar(bool& myVar, const string expr, const bool defaultValue)
{
	bool found = false;
	for(unsigned int i=0; i<m_labels.size(); i++)
	{
		if(m_labels[i] == expr)
		{
			string arg = rem_whitespaces(m_args[i]);
			if(arg == "TRUE" || arg == "True" || arg == "true") myVar = true;
			else myVar = false;
			found = true;
			break;
		}
	}
	if(!found)
	{
		myVar = defaultValue;
		m_error = true;
	}
	return found;
}

bool ConfigParser::StringArgToVar(string& myVar, const string expr, const string defaultValue)
{
	bool found = false;
	for(unsigned int i=0; i<m_labels.size(); i++)
	{
		if(m_labels[i] == expr)
		{
			size_t first = m_args[i].find_first_of("\"") + 1;
			size_t last = m_args[i].find_last_of("\"");
			myVar = m_args[i].substr(first, last-first);
			found = true;
			break;
		}
	}
	if(!found)
	{
		myVar = defaultValue;
		m_error = true;
	}
	return found;
}
