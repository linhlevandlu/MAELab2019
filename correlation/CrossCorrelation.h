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

#ifndef CROSSCORRELATION_H_
#define CROSSCORRELATION_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"

vector<Point> verify_Landmarks(Image mImage, Image sImage,
		vector<Point> manualLM, vector<Point> esLandmarks, int templSize,
		int sceneSize, double angleDiff, Point ePoint);
vector<Point> verify_Landmarks_2(ptr_IntMatrix mImage, ptr_IntMatrix sImage,
	vector<Point> manualLM, vector<Point> esLandmarks, int templSize,
	int sceneSize);
ptr_IntMatrix create_Template_2(ptr_IntMatrix inputImage, Point centerPoint,
	int tsize, Point &tlCorn, Point &brCorner, Point &distance);
vector<Point> verify_Landmarks_3(ptr_IntMatrix mImage, ptr_IntMatrix sImage,
	vector<Point> manualLM, vector<Point> esLandmarks, int templSize,
	int sceneSize,vector<Point> lsEdgePoints);

#endif /* CROSSCORRELATION_H_ */
