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

void mkdir(const std::string arg);
void rm(const std::string arg);
bool directory_exist( std::string dir );
bool file_exist(const char *fileName);

#endif
