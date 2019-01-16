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

#ifndef SHAPEHISTOGRAM_H_
#define SHAPEHISTOGRAM_H_
#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"

enum AngleAccuracy
{
	HaftDegree = 0,
	Degree = 1,
	TwoTimeDegree = 2,
	FourTimeDegree = 4,
	SixTimeDegree = 6,
	TwelveTimeDegree = 12,
	SixtyTimeDegree = 60
};
struct GFeature
{
	double angle; // angle in degree
	double dmin;
	double dmax;
};

struct LocalHistogram
{
	vector<GFeature> features;
	double maxDistance;
};

class ShapeHistogram
{
private:
	ptr_IntMatrix matrix;
	vector<LocalHistogram> listLocalHistogram;
	double totalEntries;
	double max_distance;
public:
	ShapeHistogram();
	virtual ~ShapeHistogram();

	double get_Entries();
	void set_Entries(double);
	ptr_IntMatrix get_Matrix();
	void set_Matrix(ptr_IntMatrix);
	void set_Max_Distance(double);
	double get_Max_Distance();
	void set_Local_Histogram(vector<LocalHistogram>);
	vector<LocalHistogram> get_Local_Histogram();

	vector<LocalHistogram> construct_PGH(vector<Line> listOfLines);
	ptr_IntMatrix construct_PGH_Matrix(vector<LocalHistogram> localHists,
			AngleAccuracy angleAcc, int cols);
};

#endif /* LOCALHISTOGRAM_H_ */
