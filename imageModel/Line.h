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

#ifndef LINE_H_
#define LINE_H_
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>

using namespace std;

#include "Point.h"

class Line
{

  private:
    Point begin;
    Point end;
    int dx; // slope of x
    int dy; // slope of y
    double length;
    std::vector<double> equation;

    bool is_Point();
    double length_Of_Line();
    std::vector<double> equation_Of_Line();

  public:
    Line();
    Line(const Line&);
    Line(Point, Point);
    ~Line();
    Point get_Begin();
    Point get_End();
    double get_Length();
    std::vector<double> get_Equation();
    void set_Begin(Point);
    void set_End(Point);
    double perpendicular_Distance(Point);
    double angle_Lines(Line);
    Point intersection(Line);
    bool check_Belong_Point(Point);
    vector<Point> inter_Parallel(Line line1, Line line2, double distance1,
    	double distance2, int width, int height);
};
//typedef Line* ptr_Line;

double distance_Points(Point p1, Point p2);
#endif /* LINE_H_ */
