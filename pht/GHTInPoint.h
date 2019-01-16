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

#ifndef GHTINPOINT_H_
#define GHTINPOINT_H_
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <float.h>
#include <string.h>
using namespace std;

#include "../utils/Converter.h"
#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"
#include "../segmentation/Thresholds.h"
#include "../segmentation/Canny.h"
#include "PCA.h"

struct PolarValue
{
	Point p;
	double angle;
	double distance;
};

struct REntry
{
	double gradient;
	vector<PolarValue> polarValues;
};
struct RTable
{
	Point center;
	vector<REntry> entriesTable;
};
ptr_IntMatrix get_Gradient_DMatrix(Image grayImage,vector<Point> &edgePoints);
double angle_Vector(Point p1, Point p2, Point q1, Point q2);
RTable rTable_Construct(ptr_IntMatrix gradMatrix, Point center);
Point hough_Space(ptr_IntMatrix gradMatrix, RTable rentries);
vector<Point> detect_Landmarks(Point refPoint, Point ePoint,
		vector<Point> mlandmarks);
//Point centroidPoint(ptr_IntMatrix gradMatrix);
double avg_Distance(vector<Point> listPoints, Line axis);
Line principal_Axis(ptr_IntMatrix gradMatrix, Point &cPoint);
vector<Point> generalizing_Hough_Transform(ptr_IntMatrix mGradient,
		ptr_IntMatrix sGradient, vector<Point> mLandmarks, Point &ePoint,
		Point &mPoint, double &angle, Point &translation, Point &mtranslation);
vector<Point> ght_With_Entries(RTable rEntries, Line mLine,
	Point mcPoint, ptr_IntMatrix sGradient, vector<Point> mLandmarks,
	Point &ePoint, Point &mPoint, double &angle, Point &translation, Point &mtranslation);
#endif /* GHTINPOINT_H_ */
