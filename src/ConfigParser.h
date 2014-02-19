/**
* @file ConfigParser.h
* @brief This header file contains the ConfigParser class.
*
* The ConfigParser class is used to retrieve values from a configuration file.
* @copyright GNU Public License v3
* @author Carlos Manuel Rodriguez y Martinez
* @date 7/18/2012
*
* This file is part of CMRMLib: https://github.com/cmrm/CMRMLib
*/

#pragma once
#ifndef _configParser
#define _configParser
#include <vector>
#include <string>

/**
* @class ConfigParser
* @brief Analyzes a configuration file.
*
* It's used when the program starts to load parameters from the "config.ini" file.
*/

class ConfigParser
{
	std::vector<std::string> m_labels;   ///< Vector that contains all the labels found.
	std::vector<std::string> m_args;	///< Vector that contains all the arguments found.
	bool m_fileOpened;		///< Status of the opened file.
	bool m_error;
	std::string m_filename;

public:
	///@brief Constructor
	///@param filename File to analyze.
	ConfigParser(const std::string filename);

	///@brief Ask for the state of the opened file.
	///@return true if file was opened. false if not.
	bool FileOpened();

	///@brief Ask if there has been an error parsing an arg or opening the file. Then clears the flag.
	///@return true if there is an error. false if not.
	bool Error();

	///@brief Replaces the argument of the label.
	///@param label Label to replace argument.
	///@param replaceArg New argument.
	void ReplaceArg(const std::string label, const std::string replaceArg);

	///@brief Gets int value from expresion.
	///@param myVar int var to store value.
	///@param expr Label to find.
	///@param defaultValue If expr isn't found stores this default value.
	///@return true if expr was found. False if not.
	bool IntArgToVar(int& myVar, const std::string expr, const int defaultValue);

	///@brief Gets uInt value from expresion.
	///@param myVar uInt var to store value.
	///@param expr Label to find.
	///@param defaultValue If expr isn't found stores this default value.
	///@return true if expr was found. False if not.
	bool UIntArgToVar(unsigned int& myVar, const std::string expr, const unsigned int defaultValue);

	///@brief Gets double value from expresion.
	///@param myVar double var to store value.
	///@param expr Label to find.
	///@param defaultValue If expr isn't found stores this default value.
	///@return true if expr was found. False if not.
	bool DblArgToVar(double& myVar, const std::string expr, const double defaultValue);

	///@brief Gets bool value from expresion.
	///@param myVar bool var to store value.
	///@param expr Label to find.
	///@param defaultValue If expr isn't found stores this default value.
	///@return true if expr was found. False if not.
	bool BoolArgToVar(bool& myVar, const std::string expr, const bool defaultValue);

	///@brief Gets std::string value from expresion.
	///@param myVar std::string var to store value.
	///@param expr Label to find.
	///@param defaultValue If expr isn't found stores this default value.
	///@return true if expr was found. False if not.
	bool StringArgToVar(std::string& myVar, const std::string expr, const std::string defaultValue);

	/**
	* @brief Stores M element in myVar.
	*
	* A vector of labels to search is given in the options parameter, also a vector of values.
	* In order to work properly they must have the same order.
	* @param myVar M var to store value.
	* @param expr Label to find.
	* @param options Vector which contains all the labels of the options to find.
	* @param values Vector of values to store in myVar.
	* @param defaultValue If expr isn't found stores this default value.
	*/
	template<class M> void OptionToVar(M& myVar, const std::string expr, const std::vector<std::string> options, const std::vector<M> values, const M defaultValue);
};

template<class M> void ConfigParser::OptionToVar(M& myVar, const std::string expr, const std::vector<std::string> options,
														const std::vector<M> values, const M defaultValue)
{
	bool found = false;
	if(options.size() == values.size())
	{
		for(unsigned int i=0; i<m_labels.size() && !found; i++)
		{
			if(m_labels[i] == expr)
			{
				for(unsigned int j=0; j<options.size(); j++)
				{
					if(m_args[i] == options[j])
					{
						myVar = values[j];
						found = true;
						break;
					}
				}
			}
		}
		if(!found)
		{
			myVar = defaultValue;
			m_error = true;
		}
	}
	else myVar = defaultValue;
}

#endif
