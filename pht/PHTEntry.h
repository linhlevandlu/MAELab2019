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

#ifndef PHTENTRY_H_
#define PHTENTRY_H_
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Matrix.h"

struct HoughSpace
{
	double distance;
	double angle;
};
class PHTEntry
{
private:
	Line rLine; // reference line
	Line oLine; // object line
	vector<HoughSpace> listHoughSpace;
public:
	PHTEntry();
	~PHTEntry();
	vector<HoughSpace> get_List_Hough_Space();
	void set_List_Hough_Space(vector<HoughSpace> listHoughSpace);
	Line get_Obj_Line();
	void set_Obj_Line(Line line);
	Line get_Ref_Line();
	void set_Ref_Line(Line line);
	void add_Hough_Space(HoughSpace hSpace);
};
typedef PHTEntry* ptr_PHTEntry;
#endif /* PHTENTRY_H_ */
