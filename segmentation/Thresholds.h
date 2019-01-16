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

#ifndef THRESHOLDS_H_
#define THRESHOLDS_H_
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
#include "../imageModel/Line.h"
#include "../imageModel/Edge.h"
#include "../imageModel/Matrix.h"

#include "Filters.h"

ptr_IntMatrix binary_Threshold(ptr_IntMatrix inputMatrix, int tValue, int maxValue);
ptr_IntMatrix post_Process(ptr_IntMatrix binaryMatrix, int maxValue);
ptr_RGBMatrix color_Threshold(ptr_RGBMatrix rgbImage, ptr_RGBMatrix colorHistogram);
#endif /* THRESHOLDS_H_ */
