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

#ifndef RGB_H_
#define RGB_H_

class RGB
{
private:
	unsigned short int R;
	unsigned short int G;
	unsigned short int B;
public:
	RGB();
	RGB(unsigned short int r, unsigned short int g, unsigned short int b);
	~RGB();
	void set_R(unsigned short int red);
	void set_G(unsigned short int green);
	void set_B(unsigned short int blue);
	unsigned short int get_R();
	unsigned short int get_G();
	unsigned short int get_B();
	RGB operator+(RGB color);
	RGB operator-(RGB color);
	RGB operator*(RGB color);	
	RGB operator+=(RGB color);
	RGB operator/(double color);
	void operator=(unsigned short int i);
	bool operator==(unsigned short int i);
	bool operator!=(unsigned short int i);
	bool operator>(unsigned short int i);
	bool operator>=(unsigned short int i);
};


#endif