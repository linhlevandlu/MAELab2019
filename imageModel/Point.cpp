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

#include "Point.h"

/*
 * Constructor the Point
 */
Point::Point()
{
	x = 0;
	y = 0;
	color = 0;
}
Point::Point(const Point &cpPoint)
{
	x = cpPoint.x;
	y = cpPoint.y;
	color = cpPoint.color;
}
Point::Point(int xCoor, int yCoor)
{
	x = xCoor;
	y = yCoor;
	color = 0;
}
Point::Point(int xCoor, int yCoor, RGB color)
{
	x = xCoor;
	y = yCoor;
	color = color;
}
Point::~Point()
{
	// TODO Auto-generated destructor stub
}
int Point::get_X()
{
	return x;
}
void Point::set_X(int xCoor)
{
	x = xCoor;
}
int Point::get_Y()
{
	return y;
}
void Point::set_Y(int yCoor)
{
	y = yCoor;
}
RGB Point::get_Color()
{
	return color;
}
void Point::set_Color(RGB rgb)
{
	color = rgb;
}

void Point::to_String()
{
	std::cout << "\n" << x << "\t" << y;
}

void Point::reset()
{
	x = y = 0;
	color = 0;
}
bool Point::operator<(Point cPoint)
{
	if (x < cPoint.x && y < cPoint.y)
		return true;
	return false;
}
bool Point::operator>(Point cPoint)
{
	if (x > cPoint.x && y > cPoint.y)
		return true;
	return false;
}
bool Point::operator==(Point cPoint)
{
	if (x == cPoint.x && y == cPoint.y)
		return true;
	return false;
}
bool Point::operator!=(int value)
{
	if (x != value && y != value)
		return true;
	return false;
}
Point Point::operator-(const Point cPoint)
{
	return Point(x - cPoint.x, y - cPoint.y);
}
Point Point::operator+(const Point cPoint)
{
	return Point(x + cPoint.x, y + cPoint.y);
}
