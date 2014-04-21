/**
* @file System.h
* @brief System related functions for creating, deleting and searching files.
*
* @copyright GNU Public License v3
* @author Carlos Manuel Rodriguez Martinez
* @date 2/18/2014
*
* This file is part of QBill: https://github.com/cmrm/QBill
*/

#pragma once
#ifndef _SYSTEMF
#define _SYSTEMF

#include <string>

extern bool silent;

const std::string GetCurrentDateTime();
void Print(std::string in);
void LoadBar(int x, int n, int r, int w, double phi);
void Setup_Directories(double omega);

#endif
