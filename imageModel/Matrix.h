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

#ifndef MATRIX_H_
#define MATRIX_H_
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>

using namespace std;

#include "Point.h"

void rotate_A_Point(int x, int y, Point center, double angle, double scale,
		int &xnew, int &ynew);

template<class T>
class Matrix {
private:
	std::vector<std::vector<T> > data; // store the data
	int rows; // number of rows
	int cols; // number of columns

public:
	// basic constructor
	Matrix();
	// constructor with number of rows and column
	Matrix(int nrows, int ncols);
	// constructor with number of rows, columns and default value
	Matrix(int nrows, int ncols, T value);
	Matrix(const Matrix<T> &tMatrix);

	void operator=(const Matrix<T> &tMatrix);
	void Init();
	void Init_With_Value(T);
	~Matrix() {		
	}
	int get_Rows() {
		return rows;
	}
	int get_Cols() {
		return cols;
	}
	void set_Rows(int nrows) {
		rows = nrows;
	}
	void set_Cols(int ncols) {
		cols = ncols;
	}
	std::vector<std::vector<T> > get_Data() {
		return data;
	}

	void set_Data(std::vector<std::vector<T> > iData) {
		data = iData;
	}

	T get_At_Position(int, int);
	void set_At_Position(int, int, T);
	void print_Matrix();
	void rotation(Point center, double angle, double scale, T defaultValue);
	Matrix<T> translate(int dx, int dy, T defaultValue);
	Matrix<T> transposition(T defaultValue);
	Matrix<T> multiply(Matrix<T> object, T defaultValue);
	Matrix<T> multiply_Scalar(T defaultValue, T scalar);
	Matrix<T> add(Matrix<T> object, T defaultValue);
	Matrix<T> subtract(Matrix<T> object, T defaultValue);
	Matrix<T> extract_Patch(int width, int height, int rowIndex, int colIndex,
			T initValue);
};
typedef Matrix<int>* ptr_IntMatrix;
typedef Matrix<double>* ptr_DoubleMatrix;
typedef Matrix<RGB>* ptr_RGBMatrix;

#endif /* MATRIX_H_ */
