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

#ifndef EDGE_H_
#define EDGE_H_
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>
#include <istream>
#include <algorithm>

using namespace std;

#include "Point.h"
#include "Line.h"

bool check_Point_In_List(std::vector<Point>, Point);
extern struct comparex
	{
		bool operator()(Point p1, Point p2)
		{
			return p1.get_X() < p2.get_X();
		}
	} xComparation;
extern struct comparey
	{
		bool operator()(Point p1, Point p2)
		{
			return p1.get_Y() < p2.get_Y();
		}
	} yComparation;
class Edge
{
private:
	std::vector<Point> listOfPoints; // list of points in the edge
	std::vector<Point> listOfBreakPoints; // list of break points in edge. Break point is connection point between two lines on edge.
	void break_Edge(double);

public:
	

	Edge();
	Edge(const Edge &cpedge);
	~Edge();

	Edge(std::vector<Point>);
	std::vector<Point> get_Points();
	void set_Points(std::vector<Point>);
	std::vector<Point> segment(double);

	std::vector<Line> get_Lines(std::vector<Point>);

	void sort_By_X();
	void sort_By_Y();

};
typedef Edge* ptr_Edge;
#endif /* EDGE_H_ */
