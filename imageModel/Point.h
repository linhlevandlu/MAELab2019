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

#ifndef POINT_H_
#define POINT_H_
#include<iostream>
#include "RGBColor.h"

class Point
{
private:
	int x;
	int y;
	RGB color;
public:
	Point();
	Point(const Point &cpPoint);
	Point(int xCoor, int yCoor);
	Point(int xCoor, int yCoor, RGB color);
	~Point();
	int get_X();
	void set_X(int xCoor);
	int get_Y();
	void set_Y(int yCoor);
	RGB get_Color();
	void set_Color(RGB rgb);
	void to_String();
	void reset();
	bool operator<(Point cPoint);
	bool operator>(Point cPoint);
	bool operator==(Point cPoint);
	bool operator!=(int value);
	Point operator-(Point cPoint);
	Point operator+(Point cPoint);
};

typedef Point *ptr_Point;

#endif /* POINT_H_ */
