#ifndef _PALETTE
#define _PALETTE

#include <CMRMLib/CMRM.h>
#include "gradient.h"
#include "Styles.h"

typedef class cmrm::BMPPixel Color;
typedef char Uint8;

enum COLOR_MODE
{
	GRADIENT,
	EST_MODE
};

enum COLOR
{
	red,
	green,
	blue
};

class Palette
{
public:
    COLOR_MODE colorPaletteMode;
	wxGradient gradient;					///< Gradient to be used.
	Color white;
	Color fSetColor;					///< Color of points belonging to the set.
	Color transparent;
	Uint8* redPalette;
	Uint8* greenPalette;
	Uint8* bluePalette;
	Color* palette;
	int redInt, greenInt, blueInt;			// Intensity parameters.
	double redMed, greenMed, blueMed;		// Mean parameters.
	double redDes, greenDes, blueDes;		// Standard deviation parameters.
	bool relativeColor;
	bool colorSet;							///< Activates internal coloring.
	bool colorMode;							///< Activates external coloring.
	EST_STYLES estStyle;					///< EST color palette to be used.
	GRAD_STYLES gradStyle;					///< Grad color palette to be used.
	int paletteSize;
	int estPaletteSize;
	int gradPaletteSize;
	int varGradientStep;
	int maxColorMapVal;
	double minValue, maxValue;

	Palette();
	~Palette();

	// Color styles.
	void SetESTStyle( EST_STYLES _estStyle );
	EST_STYLES GetESTSyle();
	void SetGradStyle( GRAD_STYLES _gradStyle );
	GRAD_STYLES GetGradStyle();

	Uint8 CalcGradient( int colorNum, COLOR col );
	Color CalcColor( double colorNum );
	double NormalDist(int x, double mean, double stdDev);

	void SetInt( int intensidad, COLOR col );
	void SetMed( double med, COLOR col );
	void SetDes( double des, COLOR col );
	int GetInt( COLOR col );
	double GetMed( COLOR col );
	double GetDes( COLOR col );
	void ChangeVarGradient();
	void SetPaletteSize( int size );
	int GetPaletteSize();
	wxGradient* GetGradient();
	COLOR_MODE GetColorMode();
	void SetGradient( wxGradient grad );
	void SetGradientSize( unsigned int size );
	void SetPaletteMode( COLOR_MODE mode );
	void SetRelativeColor( bool mode );
	bool GetRelativeColorMode();
	void SetVarGradient(int n);
	void RebuildPalette();
};

#endif
