/**
* @file CsvWriter.h
* @brief Writes Csv files.
*
* @author Carlos Manuel Rodriguez y Martinez
* @copyright GNU Public License v3
* @date 08/12/2013
*
* This file is part of CMRMLib: https://github.com/cmrm/CMRMLib
*/

#pragma once
#ifndef _CSVWRITER
#define _CSVWRITER

#include <string>
#include <fstream>
#include <vector>
#include "StringFuncs.h"

#ifdef USE_WX
std::vector<wxString> csv_to_strings<wxString>(wxString in)
{
	vector<wxString> out;
	in.Replace(wxT(" "), wxT(""));
	while(in.length() > 1)
	{
		out.push_back(in.BeforeFirst(','));
		in = in.AfterFirst(',');
	}
	if(out.size() == 0) out.push_back(in);
	return out;
}

#else

template <class N> std::vector<N> csv_to_vector(std::string in)
{
	std::vector<N> out;
	int startPos = 0;

	for(unsigned int i=0; i<in.length(); i++)
	{
		if(in[i] == ',')
		{
			out.push_back(string_to_num<N>(in.substr(startPos, i-startPos)));
			startPos = i+1;
		}
		if(i+1 == in.length())
		{
			if(in[in.length()-1] == '\n' || in[in.length()-1] == '\r')
				out.push_back(string_to_num<N>(in.substr(startPos, in.length()-startPos-1)));
			else
				out.push_back(string_to_num<N>(in.substr(startPos, in.length()-startPos)));
		}
	}
	return out;
}
#endif

class CsvWriter
{
	std::ofstream m_file;
	bool m_isopen;
	std::string m_delimiter;

public:
	CsvWriter();
	CsvWriter(std::string filename);
	~CsvWriter();

	// File operations.
	void Open(std::string filename);
	void Close();
	bool IsOpen();

	// Csv operations.
	void SetDelimiter(std::string delimiter);
	void Push(std::vector<std::string> data);
	void Push(std::string d1, std::string d2, std::string d3 = "", std::string d4 = "");
	template <class N> void Push(std::vector<N> data);
	template <class N> void Push(N d1, N d2);
	template <class N> void Push(N d1, N d2, N d3);
	template <class N> void Push(N d1, N d2, N d3, N d4);
};

// Csv operations.
template <class N> void CsvWriter::Push(std::vector<N> data)
{
	for(unsigned int i=0; i<data.size(); i++)
	{
		if(i != data.size()-1)
			m_file << num_to_string<N>(data[i]) << m_delimiter;
		else
			m_file << num_to_string<N>(data[i]);
	}
	m_file << std::endl;
}
template <class N> void CsvWriter::Push(N d1, N d2)
{
	m_file << d1 << m_delimiter;
	m_file << d2 << std::endl;
}
template <class N> void CsvWriter::Push(N d1, N d2, N d3)
{
	m_file << d1 << m_delimiter;
	m_file << d2 << m_delimiter;
	m_file << d3 << std::endl;
}
template <class N> void CsvWriter::Push(N d1, N d2, N d3, N d4)
{
	m_file << d1 << m_delimiter;
	m_file << d2 << m_delimiter;
	m_file << d3 << m_delimiter;
	m_file << d4 << std::endl;
}

#endif
