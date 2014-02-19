/**
* @file StringFuncs.h
* @brief Some useful string related functions.
*
* @author Carlos Manuel Rodriguez y Martinez
* @copyright GNU Public License v3
* @date 7/17/2012
*
* This file is part of CMRMLib: https://github.com/cmrm/CMRMLib
*/

#pragma once
#ifndef __stringFuncs
#define __stringFuncs

#include <sstream>
#include <vector>

#ifdef USE_WX
	#include <wx/wx.h>

	bool wx_is_there_substr(const wxString cad, const wxString cadBus);
	bool wx_is_there_function(const wxString input);

	wxString ToWxStr(std::string str);
	std::string ToStlStr(wxString std);
	wxString wx_bool_to_string(const bool d);

	template <class N> bool wx_chk_string_to_num(const wxString& s, N &num)
	{
		std::string temp(s.mb_str());
		std::istringstream i(temp);
		if (!(i >> num))
			return false;
		return true;
	}
	template <class N> wxString wx_num_to_string(const N d)
	{
		std::ostringstream oss;
		oss << d;
		wxString numOut(oss.str().c_str(), wxConvUTF8);
		return numOut;
	}
	template <class N> N wx_string_to_num(const wxString& s)
	{
		std::string temp(s.mb_str());
		std::istringstream i(temp);
		N x;
		if (!(i >> x))
			return 0;
		return static_cast<N>(x);
	}

#endif

// File management.
std::string replace_ext(const std::string filepath, const std::string newExt);
bool check_ext(const std::string filename, const std::string ext);

// Substr checking.
bool is_there_substr(const std::string cad, const std::string cadBus);
bool is_there_function(const std::string input);

// Formatting.
std::string rem_whitespaces(std::string in);

template <class N> N string_to_num(const std::string& s)
{
	std::istringstream i(s);
	N x;
	if (!(i >> x))
		return 0;
	return static_cast<N>(x);
}

std::string bool_to_string(const bool d);

template <class N> std::string num_to_string(const N d)
{
	std::ostringstream oss;
	oss << d;
	return oss.str();
}

#endif //__stringFuncs
