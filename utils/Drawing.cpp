/*
 * Drawing.cpp
 *
 *  Created on: Dec 18, 2016
 *      Author: linh
 */


#include "Drawing.h"

// draw a line using Breshenham algorithm
vector<Point> detect_Line(Line line)
{
	Point begin = line.get_Begin();
	Point end = line.get_End();
	//begin.setColor(color);
	//end.setColor(color);
	int dx = abs(end.get_X() - begin.get_X());
	int dy = abs(end.get_Y() - begin.get_Y());

	int x = begin.get_X();
	int y = begin.get_Y();
	int c = dx - dy;
	int c2 = 2 * c;

	int x_unit = 1, y_unit = 1;

	if (end.get_X() - begin.get_X() < 0)
		x_unit = -x_unit;
	if (end.get_Y() - begin.get_Y() < 0)
		y_unit = -y_unit;
	vector<Point> listOfPoints;
	listOfPoints.push_back(Point(x, y));
	// vertical line
	if (begin.get_X() == end.get_X())
	{
		while (y != end.get_Y())
		{
			y += y_unit;
			listOfPoints.push_back(Point(x, y));
		}
	}
	else
	{ // horizontal line
		if (begin.get_Y() == end.get_Y())
		{
			while (x != end.get_X())
			{
				x += x_unit;
				listOfPoints.push_back(Point(x, y));
			}
		}
		else
		{
			if (begin.get_X() != end.get_X() && begin.get_Y() != end.get_Y())
			{
				while (x != end.get_X())
				{
					c2 = 2 * c;
					if (c2 > -dy)
					{
						c = c - dy;
						x += x_unit;
					}
					if (c2 < dx)
					{
						c += dx;
						y += y_unit;
					}

					listOfPoints.push_back(Point(x, y));
				}
			}
		}
	}
	return listOfPoints;
}

vector<Point> put8pixel(Point center, Point p)
{
	int xc = center.get_X();
	int yc = center.get_Y();
	int x = p.get_X();
	int y = p.get_Y();
	vector<Point> eightPoints;
	eightPoints.push_back(Point(x + xc, y + yc));
	eightPoints.push_back(Point(-x + xc, y + yc));
	eightPoints.push_back(Point(x + xc, -y + yc));
	eightPoints.push_back(Point(-x + xc, -y + yc));
	eightPoints.push_back(Point(y + xc, x + yc));
	eightPoints.push_back(Point(-y + xc, x + yc));
	eightPoints.push_back(Point(y + xc, -x + yc));
	eightPoints.push_back(Point(-y + xc, -x + yc));
	return eightPoints;
}
vector<Point> detect_Circle(Point center, int radius)
{
	int x = 0;
	int y = radius;
	int f = 1 - radius;
	vector<Point> result;
	vector<Point> drawPoints = put8pixel(center, Point(x, y));
	result.insert(result.end(), drawPoints.begin(), drawPoints.end());
	while (x < y)
	{
		if (f < 0)
		{
			f += 2 * x + 3;
		}
		else
		{
			f += 2 * (x - y) + 5;
			y--;
		}
		x++;
		drawPoints = put8pixel(center, Point(x, y));
		result.insert(result.end(), drawPoints.begin(), drawPoints.end());
	}
	return result;
}

vector<Point> detect_Fill_Circle(Point center, int radius, RGB color)
{
	vector<Point> result;
	for (int y = -radius; y <= radius; y++)
	{
		for (int x = -radius; x <= radius; x++)
		{
			if (x * x + y * y <= radius * radius)
			{
				result.push_back(
						Point(center.get_X() + x, center.get_Y() + y, color));
			}
		}
	}
	return result;
}
Matrix<RGB> drawing_Circle(Matrix<RGB> mat, Point center, int radius, RGB color)
{
	vector<Point> dpoints = detect_Circle(center, radius);
	int rows = mat.get_Rows();
	int cols = mat.get_Cols();
	Point p1;
	for (size_t k = 0; k < dpoints.size(); k++)
	{
		p1 = dpoints.at(k);
		if (p1.get_X() >= 0 && p1.get_X() < cols && p1.get_Y() >= 0
				&& p1.get_Y() < rows)
		{
			mat.set_At_Position(p1.get_Y(), p1.get_X(), color);
		}
	}
	return mat;
}
Matrix<RGB> fill_Circle(Matrix<RGB> mat, Point center, int radius, RGB color)
{
	vector<Point> dpoints = detect_Fill_Circle(center, radius, color);
	int rows = mat.get_Rows();
	int cols = mat.get_Cols();
	Point p1;
	for (size_t k = 0; k < dpoints.size(); k++)
	{
		p1 = dpoints.at(k);
		if (p1.get_X() >= 0 && p1.get_X() < cols && p1.get_Y() >= 0
				&& p1.get_Y() < rows)
		{
			mat.set_At_Position(p1.get_Y(), p1.get_X(), color);
		}
	}
	return mat;
}
Matrix<RGB> drawing_Line(Matrix<RGB> mat, Line line, RGB color)
{
	vector<Point> dpoints = detect_Line(line);
	int rows = mat.get_Rows();
	int cols = mat.get_Cols();
	Point p1;
	for (size_t k = 0; k < dpoints.size(); k++)
	{
		p1 = dpoints.at(k);
		if (p1.get_X() >= 0 && p1.get_X() < cols && p1.get_Y() >= 0
				&& p1.get_Y() < rows)
		{
			mat.set_At_Position(p1.get_Y(), p1.get_X(), color);
		}
	}
	return mat;
}
