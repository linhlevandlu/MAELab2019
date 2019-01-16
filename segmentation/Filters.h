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

#ifndef FILTERS_H_
#define FILTERS_H_
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>
#include <float.h>

using namespace std;
#include "../imageModel/Point.h"
#include "../imageModel/Matrix.h"
#include "Canny.h"

const int KERNEL_SIZE_DEFAULT = 3;

Matrix<double> get_Gaussian_Kernel(int ksize, double sigma);
Matrix<int> gaussian_Blur(Matrix<int> inputMatrix, Matrix<double> kernel);
Matrix<double> gaussian_Blur_Double(Matrix<int> inputMatrix, Matrix<double> kernel);
Matrix<int> Robert_Operation(ptr_IntMatrix grayMatrix);
Matrix<int> Sobel_Operation(ptr_IntMatrix grayMatrix);
Matrix<double> Sobel_Operation_Double(ptr_IntMatrix grayMatrix);

Matrix<int> post_Filter(Matrix<int> sobelResult);
ptr_IntMatrix erode(ptr_IntMatrix binMatrix, int kernelSize);
ptr_IntMatrix dilate(ptr_IntMatrix binMatrix, int kernelSize);
ptr_IntMatrix open_Binary(ptr_IntMatrix binMatrix, int kernelSize);
ptr_IntMatrix close_Binary(ptr_IntMatrix binMatrix, int kernelSize);
int threshold_Otsu(Matrix<int> sobelResult);
#endif /* FILTERS_H_ */
