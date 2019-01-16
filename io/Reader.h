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

#ifndef READER_H_
#define READER_H_
#include <vector>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <dirent.h>
#include <algorithm>

using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Matrix.h"

#include "TPSReader.h"
#include "JPEGReader.h"

Matrix<RGB> read_JPG_To_RGB(const char* filename,int &rows, int &cols);
void save_RGB(const char* filename, ptr_RGBMatrix rgbMatrix);
void save_Gray_Scale(const char* filename, ptr_IntMatrix grayMatrix);
vector<Point> read_TPS_File(const char* filename);
vector<string> read_Directory(const char* dirPath);
#endif /* READER_H_ */
