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

#ifndef IMAGE_H_
#define IMAGE_H_

#include <iostream>
#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>
#include <string>
#include <algorithm>
using namespace std;

#include "Point.h"
#include "Line.h"
#include "Edge.h"
#include "Matrix.h"
#include "../io/Reader.h"

const int MAX_GRAY_VALUE = 255;

class Image
{
private:
	std::string fileName;
	vector<Line> listOfLines;
	vector<Point> manualLandmarks;
	vector<Point> autoLandmarks;
	Matrix<int> grayMatrix;
	Matrix<RGB> imgMatrix;
	Matrix<int> grayHistogram;
	Matrix<RGB> rgbHistogram;
	float medianHistogram;
	float meanHistogram;
	float thresholdValue;

	void calc_Histogram ();
	void calc_Threshold_Value();

public:
	Image();
	Image(const Image &cpimage);
	~Image();
	Image(std::string);
	void set_File_Name(std::string);
	std::string get_File_Name();
	string get_Name();
	void set_Manual_Landmarks(string);
	void set_RGB_Matrix(Matrix<RGB>);
	void set_Gray_Matrix(Matrix<int>);
	void set_Auto_Landmarks(vector<Point>);
	Matrix<int> get_Gray_Matrix();
	Matrix<RGB> get_RGB_Matrix();
	Matrix<int> get_Gray_Histogram();
	Matrix<RGB> get_RGB_Histogram();
	float get_Median_Histogram();
	float get_Mean_Histogram();
	float get_Threshold_Value();
	//vector<Line> get_List_Of_Lines();
	vector<Point> get_List_Of_Manual_Landmarks();
	vector<Point> get_List_Of_Auto_Landmarks();

	//vector<Edge> canny_Algorithm(vector<Point> &cPoints);
	//vector<Line> get_Approximate_Lines(double minDistance);
	vector<Point> read_Manual_Landmarks(string fileName);
	vector<Matrix<int> > split_Channels();
	Matrix<RGB> merge_Channels(vector<Matrix<int> > channels);
	/*ptr_DoubleMatrix getRotationMatrix2D(Point center, double angle,
		double scale);*/
	void rotate(Point center, double angle, double scale);
};
typedef Image* ptr_Image;
#endif /* IMAGE_H_ */
