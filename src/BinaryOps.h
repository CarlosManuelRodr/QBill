/**
* @file BinaryOps.h
* @brief Some binary related functions.
*
* @copyright GNU Public License v3
* @author Carlos Manuel Rodriguez y Martinez
* @date 7/18/2012
*
* This file is part of CMRMLib: https://github.com/cmrm/CMRMLib
*/

#ifndef _BinaryOps
#define _BinaryOps

#include "Types.h"

uint16 SwitchEndianess16(uint16 data);
uint32 SwitchEndianess32(uint32 data);
char* ToByte(void* ptr);
void BoolToChar(char& target, bool in);
void CharToBool(bool& target, char in);
void UInt32ToChar(char* target, uint32 in);
void CharToUInt32(uint32& target, char* in);

#endif
