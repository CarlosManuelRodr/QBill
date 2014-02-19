/**
* @file BmpWriter.h
* @brief A class to write Bmp files.
*
* I made this implementation to avoid problems with memory on insanely large files.
* @copyright GNU Public License v3
* @author Carlos Manuel Rodriguez y Martinez
* @date 7/18/2012
*
* This file is part of CMRMLib: https://github.com/cmrm/CMRMLib
*/

#pragma once
#ifndef __bmpWriter
#define __bmpWriter

#include <fstream>
#include "BinaryOps.h"

struct BMPHeader
{
	uint16 identifier;
	uint32 size;
	uint16 appSpecific1;
	uint16 appSpecific2;
	uint32 bitmapData;
};

struct DIBHeader
{
	uint32 headerSize;
	int32 width;
	int32 height;
	uint16 nPlanes;
	uint16 colorDepth;
	uint32 compression;
	uint32 bmpBytes;
	int32 hRes;
	int32 vRes;
	uint32 nColors;
	uint32 nImpColors;
};

class BMPPixel
{
public:
	char r, g, b;
	BMPPixel();
	BMPPixel(char mR, char mG, char mB);
	bool operator==(const BMPPixel &other);
};

class BMPWriter
{
	BMPHeader* m_bmpHdr;
	DIBHeader* m_dibHdr;
	std::ofstream m_file;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_paddingBytes;
	int m_dataSize;
	unsigned int m_indexHeight;

public:
	BMPWriter(const char* filepath, unsigned int width, unsigned int height);
	~BMPWriter();
	void WriteLine(BMPPixel* data);
	void CloseBMP();
};

#endif
