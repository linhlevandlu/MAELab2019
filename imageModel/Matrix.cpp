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


#include "Matrix.h"

template<class T>
Matrix<T>::Matrix()
{
	rows = 0;
	cols = 0;
}
template<class T>
Matrix<T>::Matrix(int nrows, int ncols)
{
	rows = nrows;
	cols = ncols;
	// Initialization the matrix
	Init();
}
template<class T>
Matrix<T>::Matrix(int nrows, int ncols, T value)
{
	rows = nrows;
	cols = ncols;
	Init_With_Value(value);
}
template<class T>
Matrix<T>::Matrix(const Matrix<T> &tMatrix)
{
	this->rows = tMatrix.rows;
	this->cols = tMatrix.cols;
	Init();
	this->data = tMatrix.data;
}
template<class T>
void Matrix<T>::Init()
{
	data.resize(get_Rows());
	for (int i = 0; i < get_Rows(); i++)
		data[i].resize(get_Cols());
}

template<class T>
void Matrix<T>::Init_With_Value(T value)
{
	data.resize(get_Rows());
	for (int i = 0; i < get_Rows(); i++)
		data[i].resize(get_Cols(), value);
}

template<class T>
T Matrix<T>::get_At_Position(int r, int c)
{
	return data[r][c];
}

template<class T>
void Matrix<T>::set_At_Position(int r, int c, T value)
{
	data[r][c] = value;
}

template<class T>
void Matrix<T>::print_Matrix()
{

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			//cout << data[i][j] << "\t";
		}
		cout << endl;
	}

}
template<class T>
void Matrix<T>::operator=(const Matrix<T> &tMatrix)
{
	rows = tMatrix.rows;
	cols = tMatrix.cols;
	//Init();
	data = tMatrix.data;
}

template<class T>
void Matrix<T>::rotation(Point center, double angle, double scale,
	T defaultValue)
{
	Matrix<T> result(rows, cols);
	result.Init_With_Value(defaultValue);
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			T value = data[row][col];

			int xnew = 0; //round(a00 * col + a01 * row + b00);
			int ynew = 0; //round(a10 * col + a11 * row + b10);
			rotate_A_Point(col, row, center, angle, scale, xnew, ynew);
			if (xnew >= 0 && xnew < cols && ynew >= 0 && ynew < rows)
			{
				result.data[ynew][xnew] = value;
			}
		}
	}
	for (int row = 0; row < rows; row++)
	{
		for (int col = 1; col < cols - 1; col++)
		{
			T value = result.get_At_Position(row, col);
			T leftValue = result.get_At_Position(row, col - 1);
			T rightValue = result.get_At_Position(row, col + 1);
			if (value == 0 && leftValue > 0 && rightValue >= 0)
			{
				result.data[row][col] = (leftValue + rightValue) / 2;
			}
		}
	}
	data = result.data;
	//return result;
}
template<class T>
Matrix<T> Matrix<T>::translate(int dx, int dy, T defaultValue)
{
	Matrix<T> result(rows, cols);
	result.Init_With_Value(defaultValue);
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			T value = data[row][col];

			int xnew = col + dx;
			int ynew = row + dy;
			if (xnew >= 0 && xnew < cols && ynew >= 0 && ynew < rows)
			{
				result.data[ynew][xnew] = value;
			}
		}
	}
	return result;
}
template<class T>
Matrix<T> Matrix<T>::transposition(T defaultValue)
{
	Matrix<T> result(cols, rows);
	result.Init_With_Value(defaultValue);
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			T value = data[row][col];
			result.data[col][row] = value;
		}
	}
	return result;
}

template<class T>
Matrix<T> Matrix<T>::add(Matrix<T> object, T defaultValue)
{
	int objRows = object.rows;
	int objCols = object.cols;
	Matrix<T> result(objRows, objCols);
	result.Init_With_Value(defaultValue);
	if (rows != objRows || cols != objCols)
		return result;
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			T sum = defaultValue;
			T value1 = data[row][col];
			T value2 = object.data[row][col];
			sum = value1 + value2;

			result.data[row][col] = sum;
		}
	}
	return result;
}
template<class T>
Matrix<T> Matrix<T>::subtract(Matrix<T> object, T defaultValue)
{
	int objRows = object.rows;
	int objCols = object.cols;
	Matrix<T> result(objRows, objCols);
	result.Init_With_Value(defaultValue);
	if (rows != objRows || cols != objCols)
		return result;
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			T sum = defaultValue;
			T value1 = data[row][col];
			T value2 = object.data[row][col];
			sum = value1 - value2;

			result.data[row][col] = sum;
		}
	}
	return result;
}
template<class T>
Matrix<T> Matrix<T>::multiply(Matrix<T> object, T defaultValue)
{
	int objRows = object.rows;
	int objCols = object.cols;
	Matrix<T> result(rows, objCols);
	result.Init_With_Value(defaultValue);
	if (cols != objRows)
		return result;
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < objCols; col++)
		{
			T sum = defaultValue;
			for (int k = 0; k < cols; k++)
			{
				T value1 = data[row][k];
				T value2 = object.data[k][col];
				sum += value1 * value2;
			}
			result.data[row][col] = sum;
		}
	}
	return result;
}

template<class T>
Matrix<T> Matrix<T>::multiply_Scalar(T defaultValue, T scalar)
{

	Matrix<T> result(rows, cols);
	result.Init_With_Value(defaultValue);
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			T value = data[row][col];
			T svalue = scalar * value;
			result.data[row][col] = svalue;
		}
	}
	return result;
}
template<class T>
Matrix<T> Matrix<T>::extract_Patch(int width, int height, int rowIndex,
	int colIndex, T initValue)
{
	Matrix<T> result(height, width);
	result.Init_With_Value(initValue);
	Point lleft, lright;
	if (width % 2 == 0)
		width += 1;
	if (height % 2 == 0)
		height += 1;
	int wh = width / 2;
	int hh = height / 2;
	int i = 0, j = 0;

	for (int r = rowIndex - hh; r < rowIndex + hh; r++)
	{
		j = 0;
		for (int c = colIndex - wh; c < colIndex + wh; c++)
		{
			T value = initValue;
			if (r >= 0 && c >= 0 && r < rows && c < cols)
			{
				value = data[r][c];
			}
			result.data[i][j] = value;
			j++;
		}
		i++;
	}
	return result;
}


// get rotation matrix with angle (in degree) and scale
ptr_DoubleMatrix get_Rotation_Matrix_2D(Point center, double angle, double scale)
{
	double alpha = cos(angle * M_PI / 180) * scale;
	double beta = sin(angle * M_PI / 180) * scale;

	ptr_DoubleMatrix rotateM = new Matrix<double>(2, 3, 0);

	rotateM->set_At_Position(0, 0, alpha);
	rotateM->set_At_Position(0, 1, beta);
	rotateM->set_At_Position(0, 2,
		(1 - alpha) * center.get_X() - beta * center.get_Y());
	rotateM->set_At_Position(1, 0, -beta);
	rotateM->set_At_Position(1, 1, alpha);
	rotateM->set_At_Position(1, 2,
		beta * center.get_X() + (1 - alpha) * center.get_Y());

	return rotateM;
}
// angle in degree
void rotate_A_Point(int x, int y, Point center, double angle, double scale,
	int &xnew, int &ynew)
{
	xnew = 0;
	ynew = 0;
	ptr_DoubleMatrix rotationMatrix = get_Rotation_Matrix_2D(center, angle, scale);

	double a00 = rotationMatrix->get_At_Position(0, 0);
	double a01 = rotationMatrix->get_At_Position(0, 1);
	double b00 = rotationMatrix->get_At_Position(0, 2);
	double a10 = rotationMatrix->get_At_Position(1, 0);
	double a11 = rotationMatrix->get_At_Position(1, 1);
	double b10 = rotationMatrix->get_At_Position(1, 2);
	xnew = round(a00 * x + a01 * y + b00);
	ynew = round(a10 * x + a11 * y + b10);
	delete rotationMatrix;
}
template class Matrix<int> ;
template class Matrix<double> ;
template class Matrix<float> ;
template class Matrix<RGB> ;

