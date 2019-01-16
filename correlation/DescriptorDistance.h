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

#ifndef DESCRIPTORDISTANCE_H_
#define DESCRIPTORDISTANCE_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <iomanip>
using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Matrix.h"
#include "../segmentation/Filters.h"

Point nearest_Point(vector<Point> lsPoints, Point p);
vector<double> SIFT_Descriptor(ptr_IntMatrix imgMatrix, Point center, int size);
vector<Point> verify_Descriptors(ptr_IntMatrix model, ptr_IntMatrix scene,
	vector<Point> manualLM, vector<Point> esLandmarks, int templSize,
	int sceneSize);
vector<Point> verify_Descriptors_2(ptr_IntMatrix model, ptr_IntMatrix scene,
	vector<Point> scenePoints, vector<Point> manualLM, vector<Point> esLandmarks,
	int templSize, int sceneSize);
vector<Point> verify_Descriptors_3(ptr_IntMatrix model, ptr_IntMatrix scene,
	vector<Point> contourPoints, vector<Point> manualLM, int patchSize);
vector<Point> verify_Descriptors_4(ptr_IntMatrix model, ptr_IntMatrix scene,
	vector<Point> contourPoints, vector<Point> manualLM, int patchSize);
vector<Point> test_SIFT_On_RGB(ptr_RGBMatrix model, vector<Point> mLandmarks,
	ptr_RGBMatrix scene,vector<Point> contourPoints, int pSize);
#endif /* DESCRIPTORDISTANCE_H_ */
