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

#ifndef MAELAB_H_
#define MAELAB_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "imageModel/Point.h"
#include "imageModel/Line.h"
#include "imageModel/Edge.h"
#include "imageModel/Matrix.h"
#include "imageModel/Image.h"

#include "segmentation/Thresholds.h"
#include "segmentation/Canny.h"
#include "segmentation/Suzuki.h"
#include "segmentation/Filters.h"
#include "segmentation/LineSegment.h"


#include "histograms/ShapeHistogram.h"
#include "pht/PHTEntry.h"
#include "pht/PHoughTransform.h"
#include "correlation/CrossCorrelation.h"

#include "pointInterest/Treatments.h"
//#include "pointInterest/Segmentation.h"
#include "pointInterest/GeometricHistgoram.h"
#include "pointInterest/ProHoughTransform.h"
#include "pointInterest/LandmarkDetection.h"

ptr_IntMatrix mae_get_Gray_Histogram(Image* inputImage);
ptr_RGBMatrix mae_get_RGB_Histogram(Image* inputImage);
ptr_IntMatrix mae_Binary_Threshold(Image* inputImage);
vector<Point> mae_Canny_Algorithm(Image* inputImage);
vector<Edge> mae_Suzuki_Algorithm(Image* inputImage);
vector<Line> mae_Line_Segment(Image* inputImage);
Matrix<RGB> mae_Gaussian_Filter(Image* inputImage, Matrix<double> kernel);
Matrix<int> mae_Robert_Filter(Image* inputImage);
Matrix<double> mae_Sobel_Filter(Image* inputImage);
Matrix<int> mae_Erode(Image* inputImage);
Matrix<int> mae_Dilate(Image* inputImage);
Matrix<int> mae_Open_Binary(Image* inputImage);
Matrix<int> mae_Close_Binary(Image* inputImage);
vector<Point> mae_PHT_Method(Image modelImage, string modelLandmarks, Image sceneImage, AngleAccuracy acc, int cols, int templSize, int sceneSize, Point &ePoint,
	double &angleDiff);
vector<Point> mae_PCAI_Method(Image modelImage,string modelLandmarks, Image sceneImage);
vector<Point> mae_Sobel_SIFT_Method(Image modelImage, string modelLandmarks, Image sceneImage,int patchsize);
vector<Point> mae_Canny_SIFT_Method(Image modelImage, string modelLandmarks, Image sceneImage,int patchsize);


/*
vector<Line> segment(ptr_Treatments treatment, Image sceneImage,
	int minDistance);
ShapeHistogram geomHistogram(ptr_Treatments treatment, Image image,
	AngleAccuracy angleAcc, int cols);
double bhattacharyyaDistance(ptr_Treatments treatment, Image sceneImage,
	AngleAccuracy angleAcc, int cols);
PHoughTransform phtEntriesTable(ptr_Treatments treatment, Image image);
vector<Point> estimatedLandmarks(ptr_Treatments treatment, Image sceneImage,
	AngleAccuracy acc, int cols, int templSize, int sceneSize,Point &ePoint, double angleDiff);
*/
double measure_Centroid_Point(vector<Point> landmarks, Point &ebary);

#endif /* ANALYSIS_H_ */
