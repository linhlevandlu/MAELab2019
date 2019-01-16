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

#ifndef PCA_H_
#define PCA_H_
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include <algorithm>
using namespace std;
#include "../utils/Converter.h"
#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Edge.h"
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"
#include "../segmentation/Thresholds.h"
#include "../segmentation/Canny.h"
#include "../correlation/CrossCorrelation.h"
#include "../correlation/DescriptorDistance.h"
#include "../utils/Drawing.h"
#include "../io/Reader.h"
#include "GHTInPoint.h"

Line principal_Axis(ptr_IntMatrix gradMatrix, Point &cPoint);
Line principal_Axis(vector<Point> listOfPoints, Point &cPoint);
double rotate_Direction(Line refLine, Line objLine, double angle);
vector<Point> PCAI_Points(vector<Point> modelPoints, Point mPoint,
		vector<Point> scenePoints, double angle);
vector<Point> PCAI(Image modelGray, Image sceneGray, vector<Point> mnLandmarks);
/*void pcaiFolder(string folderScene, vector<string> sceneImages, Image model,
	vector<Point> mnLandmarks,string savePath);*/
#endif /* PCA_H_ */
