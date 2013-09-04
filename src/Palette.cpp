#include "Palette.h"
using namespace cmrm;

ESTFractalColor::ESTFractalColor()
{

}
void ESTFractalColor::SetStyle(EST_STYLES style)
{
	// Sets the parameters of each color style.
	switch(style)
	{
	case SUMMER_DAY:
		{
			redInt = 255;
			redMed = 34;
			redDes = 8;
			greenInt = 201;
			greenMed = 28;
			greenDes = 12;
			blueInt = 255;
			blueMed = 21;
			blueDes = 9;
			paletteSize = 60;
			break;
		}
	case COOL_BLUE:
		{
			redInt = 252;
			redMed = 104;
			redDes = 37;
			greenInt = 255;
			greenMed = 96;
			greenDes = 34;
			blueInt = 255;
			blueMed = 73;
			blueDes = 40;
			paletteSize = 300;
			break;
		}
	case HARD_RED:
		{
			redInt = 210;
			redMed = 19;
			redDes = 14;
			greenInt = 102;
			greenMed = 26;
			greenDes = 10;
			blueInt = 97;
			blueMed = 29;
			blueDes = 10;
			paletteSize = 60;
			break;
		}
	case BLACK_AND_WHITE:
		{
			redInt = 200;
			redMed = 30;
			redDes = 9;
			greenInt = 200;
			greenMed = 30;
			greenDes = 9;
			blueInt = 200;
			blueMed = 30;
			blueDes = 9;
			paletteSize = 60;
			break;
		}
	case PASTEL:
		{
			redInt = 205;
			redMed = 23;
			redDes = 14;
			greenInt = 196;
			greenMed = 26;
			greenDes = 11;
			blueInt = 198;
			blueMed = 38;
			blueDes = 9;
			paletteSize = 60;
			break;
		}
	case PSYCH_EXPERIENCE:
		{
			redInt = 126;
			redMed = 3;
			redDes = 11;
			greenInt = 200;
			greenMed = 36;
			greenDes = 9;
			blueInt = 200;
			blueMed = 24;
			blueDes = 12;
			paletteSize = 60;
			break;
		}
	case VIVID_COLORS:
		{
			redInt = 200;
			redMed = 0;
			redDes = 10;
			greenInt = 200;
			greenMed = 17;
			greenDes = 10;
			blueInt = 200;
			blueMed = 37;
			blueDes = 10;
			paletteSize = 60;
			break;
		}
    case CUSTOM:
            break;

	};
}

GradFractalColor::GradFractalColor()
{

}
void GradFractalColor::SetStyle(GRAD_STYLES style)
{
	// Sets the wxString parameter of each color style.
	switch(style)
	{
	case RETRO:
		{
			grad = wxT("rgb(4,108,164);rgb(136,171,14);rgb(255,255,255);rgb(171,27,27);rgb(61,43,94);rgb(4,108,164);");
			paletteSize = 300;
		}
		break;
	case HAKIM:
		{
			grad = wxT("rgb(255,255,255);rgb(91,91,91);rgb(0,0,0);rgb(125,199,44);rgb(228,213,12);rgb(192,5,5);rgb(61,43,94);rgb(255,255,255);");
			paletteSize = 300;
		}
		break;
	case AGUAMARINA:
		{
			grad = wxT("rgb(30,102,162);rgb(255,255,255);rgb(91,18,122);rgb(0,0,0);rgb(16,33,112);rgb(30,145,87);rgb(30,102,162);");
			paletteSize = 300;
		}
		break;
    case GRAD_CUSTOM:
		break;
	};
}

// Palette
Palette::Palette()
{
    // Creates default color palette.
	relativeColor = true;
	gradPaletteSize = 300;
	colorPaletteMode = GRADIENT;
	estStyle = SUMMER_DAY;
	gradStyle = RETRO;
	gradient.fromString(wxString(wxT("rgb(4,108,164);rgb(136,171,14);rgb(255,255,255);rgb(171,27,27);rgb(61,43,94);rgb(4,108,164);")));
	gradient.setMin(0);
	gradient.setMax(gradPaletteSize);

    estPaletteSize = 60;
	paletteSize = 300;
	redPalette = new Uint8[paletteSize];
	greenPalette = new Uint8[paletteSize];
	bluePalette = new Uint8[paletteSize];
	palette = new Color[paletteSize];
	varGradientStep = paletteSize/60;

	redInt = 255;
	greenInt = 201;
	blueInt = 255;

	redMed = 34;
	greenMed = 28;
	blueMed = 12;

	redDes = 8;
	greenDes = 12;
	blueDes = 9;

	if(colorPaletteMode == GRADIENT)
	{
		wxColour myWxColor;
		for(int i=0; i<paletteSize; i++)
		{
			myWxColor = gradient.getColorAt(i);
			redPalette[i] = myWxColor.Red();
			greenPalette[i] = myWxColor.Green();
			bluePalette[i] = myWxColor.Blue();
		}
	}
	else
	{
		for(int i=0; i<paletteSize; i++)
		{
			redPalette[i] = CalcGradient(i, red);
			greenPalette[i] = CalcGradient(i, green);
			bluePalette[i] = CalcGradient(i, blue);
		}
	}

	for(int i=0; i<paletteSize; i++)
	{
		palette[i] = Color(redPalette[i], greenPalette[i], bluePalette[i]);
	}
}
Palette::~Palette()
{
    delete[] redPalette;
	delete[] greenPalette;
	delete[] bluePalette;
	delete[] palette;
}
Color Palette::CalcColor(double colorNum)
{
    if(colorNum <= 0) colorNum = 0;
    int intColor = (int)colorNum;

    if(colorNum == 0)
    {
        return Color(255,255,255);
    }
    else
    {
        if(relativeColor)
        {
            colorNum = ((colorNum-minValue)/(maxValue-minValue))*paletteSize;
            if(colorNum >= paletteSize) colorNum = paletteSize-1;
            intColor = (int)colorNum;
        }
        else
        {
            intColor = intColor % paletteSize;
        }
        return palette[intColor];
	}
}
Uint8 Palette::CalcGradient(int colorNum, COLOR col)
{
	colorNum = colorNum % paletteSize;
	// Return color with a normal distribution.
	if(col == red)
	{
		return static_cast<Uint8>(redInt*NormalDist(colorNum, redMed, redDes) + redInt*NormalDist(colorNum, paletteSize+redMed, redDes));
	}
	if(col == green)
	{
		return static_cast<Uint8>(greenInt*NormalDist(colorNum, greenMed, greenDes) + greenInt*NormalDist(colorNum, paletteSize+greenMed, greenDes));
	}
	if(col == blue)
	{
		return static_cast<Uint8>(blueInt*NormalDist(colorNum, blueMed, blueDes)+blueInt*NormalDist(colorNum, paletteSize+blueMed, blueDes));
	}
	return 0;
}
double Palette::NormalDist(int x, double mean, double stdDev)
{
	return exp(-(pow(x-mean,2)/(2*pow(stdDev,2))));
}
void Palette::SetInt(int intensidad, COLOR col)
{
	// Changes intensity value.
	colorPaletteMode = EST_MODE;
	if(col == red)
	{
		redInt = intensidad;
	}
	else if(col == green)
	{
		greenInt = intensidad;
	}
	else if(col == blue)
	{
		blueInt = intensidad;
	}
	this->RebuildPalette();
}
void Palette::SetMed(double med, COLOR col)
{
	// Changes mean value.
	colorPaletteMode = EST_MODE;
	if(col == red)
	{
		redMed = med;
	}
	else if(col == green)
	{
		greenMed = med;
	}
	else if(col == blue)
	{
		blueMed = med;
	}
	this->RebuildPalette();
}
void Palette::SetDes(double des, COLOR col)
{
	// Changes standard deviation value.
	colorPaletteMode = EST_MODE;
	if(col == red)
	{
		redDes = des;
	}
	else if(col == green)
	{
		greenDes = des;
	}
	else if(col == blue)
	{
		blueDes = des;
	}
	this->RebuildPalette();
}
int Palette::GetInt(COLOR col)
{
	if(col == red)
	{
		return redInt;
	}
	else if(col == green)
	{
		return greenInt;
	}
	else if(col == blue)
	{
		return blueInt;
	}
	else return 0;
}
double Palette::GetMed(COLOR col)
{
	if(col == red)
	{
		return redMed;
	}
	else if(col == green)
	{
		return greenMed;
	}
	else if(col == blue)
	{
		return blueMed;
	}
	else return 0;
}
double Palette::GetDes(COLOR col)
{
	if(col == red)
	{
		return redDes;
	}
	else if(col == green)
	{
		return greenDes;
	}
	else if(col == blue)
	{
		return blueDes;
	}
	else return 0;
}

// Color styles.
void Palette::SetESTStyle(EST_STYLES _estStyle)
{
	// Changes color palette.
	estStyle = _estStyle;
	ESTFractalColor c;
	c.SetStyle(estStyle);

	// Sets parameters for new palette.
	redInt = c.redInt;
	greenInt = c.greenInt;
	blueInt = c.blueInt;

	redMed = c.redMed;
	greenMed = c.greenMed;
	blueMed = c.blueMed;

	redDes = c.redDes;
	greenDes = c.greenDes;
	blueDes = c.blueDes;

	this->SetPaletteSize(c.paletteSize);
	for(int i=0; i<paletteSize; i++)
	{
		redPalette[i] = CalcGradient(i, red);
		greenPalette[i] = CalcGradient(i, green);
		bluePalette[i] = CalcGradient(i, blue);
	}
	for(int i=0; i<paletteSize; i++)
	{
		palette[i] = Color(redPalette[i], greenPalette[i], bluePalette[i]);
	}
}
EST_STYLES Palette::GetESTSyle()
{
	return estStyle;
}
void Palette::SetGradStyle(GRAD_STYLES _gradStyle)
{
	gradStyle = _gradStyle;
}
GRAD_STYLES Palette::GetGradStyle()
{
	return gradStyle;
}

// Gradient color.
wxGradient* Palette::GetGradient()
{
	return &gradient;
}
void Palette::SetGradient(wxGradient grad)
{
	// Copy gradient.
	colorPaletteMode = GRADIENT;
	gradient = grad;
	delete[] redPalette;
	delete[] greenPalette;
	delete[] bluePalette;
	delete[] palette;
	gradPaletteSize = paletteSize = gradient.getMax()-gradient.getMin();
	redPalette = new Uint8[paletteSize];
	greenPalette = new Uint8[paletteSize];
	bluePalette = new Uint8[paletteSize];
	palette = new Color[paletteSize];
	varGradientStep = paletteSize/60;
	this->RebuildPalette();
}
void Palette::SetGradientSize(unsigned int size)
{
	colorPaletteMode = GRADIENT;
	delete[] redPalette;
	delete[] greenPalette;
	delete[] bluePalette;
	delete[] palette;
	gradient.setMax(size);
	gradPaletteSize = paletteSize = size;
	redPalette = new Uint8[paletteSize];
	greenPalette = new Uint8[paletteSize];
	bluePalette = new Uint8[paletteSize];
	palette = new Color[paletteSize];
	varGradientStep = paletteSize/60;
	this->RebuildPalette();
}
void Palette::RebuildPalette()
{
	if(colorPaletteMode == GRADIENT)
	{
		wxColour myWxColor;
		for(int i=0; i<paletteSize; i++)
		{
			myWxColor = gradient.getColorAt(i);
			redPalette[i] = myWxColor.Red();
			greenPalette[i] = myWxColor.Green();
			bluePalette[i] = myWxColor.Blue();
		}
	}
	else
	{
		for(int i=0; i<paletteSize; i++)
		{
			redPalette[i] = CalcGradient(i, red);
			greenPalette[i] = CalcGradient(i, green);
			bluePalette[i] = CalcGradient(i, blue);
		}
	}

	for(int i=0; i<paletteSize; i++)
	{
		palette[i] = Color(redPalette[i], greenPalette[i], bluePalette[i]);
	}
}

// ColorMode.
void Palette::SetPaletteMode(COLOR_MODE mode)
{
	colorPaletteMode = mode;
	delete[] redPalette;
	delete[] greenPalette;
	delete[] bluePalette;
	delete[] palette;
	if(mode == GRADIENT)
	{
		paletteSize = gradient.getMax()-gradient.getMin();
	}
	else
	{
		paletteSize = estPaletteSize;
	}
	redPalette = new Uint8[paletteSize];
	greenPalette = new Uint8[paletteSize];
	bluePalette = new Uint8[paletteSize];
	palette = new Color[paletteSize];
	varGradientStep = paletteSize/60;
	this->RebuildPalette();
}
COLOR_MODE Palette::GetColorMode()
{
	return colorPaletteMode;
}
int Palette::GetPaletteSize()
{
	return paletteSize;
}
void Palette::SetPaletteSize(int size)
{
	delete[] redPalette;
	delete[] greenPalette;
	delete[] bluePalette;
	delete[] palette;
	estPaletteSize = paletteSize = size;
	redPalette = new Uint8[paletteSize];
	greenPalette = new Uint8[paletteSize];
	bluePalette = new Uint8[paletteSize];
	palette = new Color[paletteSize];
	varGradientStep = paletteSize/60;
	this->RebuildPalette();
}
void Palette::SetVarGradient(int n)
{
	varGradientStep = n % paletteSize;
}
void Palette::SetRelativeColor(bool mode)
{
	relativeColor = mode;
	this->RebuildPalette();
}
bool Palette::GetRelativeColorMode()
{
	return relativeColor;
}
