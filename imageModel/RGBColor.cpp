/*
 Morphometry with Automatic Extraction of Landmarks (MAELab)
 Copyright (C) 2017  Le Van Linh (van-linh.le@u-bordeaux.fr)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#include "RGBColor.h"

RGB::RGB()
{
	R = 0;
	G = 0;
	B = 0;
}
RGB::RGB(unsigned short int r, unsigned short int g, unsigned short int b)
{
	R = r;
	G = g;
	B = b;
}
RGB::~RGB()
{	
}
void RGB::set_R(unsigned short int red)
{
	R = red;
}
void RGB::set_G(unsigned short int green)
{
	G = green;
}
void RGB::set_B(unsigned short int blue)
{
	B = blue;
}
unsigned short int RGB::get_R() {return R;}
unsigned short int RGB::get_G() {return G;}
unsigned short int RGB::get_B() {return B;}
RGB RGB::operator+(RGB color)
{
	RGB result;
	result.R = color.R + R;
	result.G = color.G + G;
	result.B = color.B + B;
	return result;
}
RGB RGB::operator-(RGB color)
{
	RGB result;
	result.R = color.R - R;
	result.G = color.G - G;
	result.B = color.B - B;
	return result;
}
RGB RGB::operator*(RGB color)
{
	RGB result;
	result.R = color.R * R;
	result.G = color.G * G;
	result.B = color.B * B;
	return result;
}

RGB RGB::operator+=(RGB color)
{
	RGB result;
	result.R = color.R + R;
	result.G = color.G + G;
	result.B = color.B + B;
	return result;
}
RGB RGB::operator/(double c)
{
	RGB result;
	result.R = R / c;
	result.G = G / c;
	result.B = B / c;
	return result;
}
void RGB::operator=( unsigned short int value)
{
	
	this->R = value;
	this->G = value;
	this->B = value;

}

bool RGB::operator==( unsigned short int i)
{
	if (R == i && G == i && B == i)
		return true;
	return false;
}
bool RGB::operator!=( unsigned short int i)
{
	if (R != i && G != i && B != i)
		return true;
	return false;
}
bool RGB::operator>( unsigned short int i)
{
	if (R > i && G > i && B > i)
		return true;
	return false;
}
bool RGB::operator>=( unsigned short int i)
{
	if (R >= i && G >= i && B >= i)
		return true;
	return false;
}
