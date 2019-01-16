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

#ifndef PROHOUGHTRANSFORM_H_
#define PROHOUGHTRANSFORM_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Edge.h"
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"

#include "../segmentation/Canny.h"
#include "../segmentation/Thresholds.h"
#include "../segmentation/LineSegment.h"

#include "../pht/PHTEntry.h"
#include "../pht/PHoughTransform.h"
#include "../pht/GHTInPoint.h"
#include "../io/Reader.h"
#include "../utils/Drawing.h"
#include "../pht/PCA.h"
#include "../correlation/DescriptorDistance.h"
#include "Treatments.h"

PHTEntry matchingInScene(vector<PHTEntry> entryTable, vector<Line> sceneLines,
	int width, int height, vector<Line> &maxVector);
Point refPointInScene(PHTEntry entry, vector<Line> matchLines,
	double &angleDiff, vector<Point> refLandmarks, int width, int height);
vector<Point> phtLandmarks(vector<PHTEntry> entriesTable, Point refPoint,
	vector<Line> sceneLines, int width, int height, vector<Point> mLandmarks,
	double &angleDiff, Point &ePoint);

class ProHoughTransform: public Treatments
{
public:
	ProHoughTransform();
	virtual ~ProHoughTransform();
	PHoughTransform constructPHT();
	vector<Point> estimateLandmarks(Image sImage, double &angleDiff,
		Point &ePoint);

	vector<Point> generalTransform(Image &sImage, double &angle, Point &ePoint,
		Point &mPoint, ptr_IntMatrix &newScene, Point &translation,
		vector<Point> &modelSeg, vector<Point> &sceneSeg, vector<Point> &newScenePoints,double &scaleX, double &scaleY, Point &moveScale);
};

#endif /* PROHOUGHTRANSFORM_H_ */
