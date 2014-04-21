/**
* @file StringFuncs.h
* @brief Some useful string related functions.
*
* @author Carlos Manuel Rodriguez y Martinez
* @copyright GNU Public License v3
* @date 7/17/2012
*
* This file is part of QBill: https://github.com/cmrm/QBill
*/

#pragma once
#ifndef __stringFuncs
#define __stringFuncs

#include <sstream>
#include <vector>

// File management.
std::string replace_ext(const std::string filepath, const std::string newExt);
bool check_ext(const std::string filename, const std::string ext);

// Substr checking.
bool is_there_substr(const std::string cad, const std::string cadBus);

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
