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

#ifndef LANDMARKDETECTION_H_
#define LANDMARKDETECTION_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Edge.h"
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"

#include "../segmentation/Canny.h"
#include "../segmentation/Thresholds.h"
#include "../segmentation/LineSegment.h"

#include "../histograms/ShapeHistogram.h"
#include "../pht/PHTEntry.h"
#include "../pht/PHoughTransform.h"
#include "../pht/GHTInPoint.h"
#include "../correlation/CrossCorrelation.h"
#include "../correlation/DescriptorDistance.h"
#include "../utils/Drawing.h"
#include "../io/Reader.h"

#include "Treatments.h"
//#include "Segmentation.h"
#include "GeometricHistgoram.h"
#include "ProHoughTransform.h"

class LandmarkDetection: public Treatments
{
private:
	vector<Point> refineLandmarks(vector<Point> estLandmarks,vector<Point> segmentation);
public:
	LandmarkDetection();
	virtual ~LandmarkDetection();
	vector<Point> landmarksAutoDectect(Image sceneImage, AngleAccuracy acc,
		int cols, int templSize, int sceneSize, Point &ePoint,
		double &angleDiff);
	//void landmarksOnDir(string modelName,string folderScene,
	//	vector<string> sceneImages, AngleAccuracy acc, int cols, int templSize,
	//	int sceneSize, Point &ePoint, double &angleDiff,string saveFolder);
	vector<Point> landmarksAutoDectect2(Image &sceneImage,
			int templSize, int sceneSize);
	vector<Point> landmarksWithSIFT(Image &sceneImage,
			int templSize, int sceneSize);
	//void landmarksOnDir2(string modelName, string folderScene,
	//	vector<string> sceneImages, string saveFolder);
	//void landmarksOnDir3(string modelName, string folderScene,
	//	vector<string> sceneImages, string saveFolder);
	//void landmarksOnDir4(string modelName, string folderScene,
	//	vector<string> sceneImages, string saveFolder, string folderLandmarks,
	//	vector<string> landmarks);
};

#endif /* LANDMARKDETECTION_H_ */
