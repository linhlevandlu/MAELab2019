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

#include "Filters.h"

Matrix<double> get_Gaussian_Kernel(int ksize, double sigma)
{
	Matrix<double> kernel(ksize, ksize, 0.0);
	if (sigma <= 0)
	{
		sigma = 0.3 * ((ksize - 1) * 0.5 - 1) + 0.8;
	}
	double mean = ksize / 2;
	double scale = (2 * sigma * sigma);
	double evalue;
	double sum = 0;
	for (int i = 0; i < ksize; i++)
	{
		for (int j = 0; j < ksize; j++)
		{
			evalue = 0.0;
			double r = pow(i - mean, 2) + pow(j - mean, 2);
			evalue = exp(-r / scale) / (scale * M_PI);
			kernel.set_At_Position(i, j, evalue);
			sum += evalue;
		}
	}
	// normalized
	for (int i = 0; i < ksize; i++)
	{
		for (int j = 0; j < ksize; j++)
		{
			kernel.set_At_Position(i, j, kernel.get_At_Position(i, j) / sum);
			//cout << "\t" << kernel.get_At_Position(i, j);
		}
		//cout << endl;
	}
	return kernel;
}

Matrix<int> gaussian_Blur(Matrix<int> inputMatrix, Matrix<double> kernel)
{
	int rows = inputMatrix.get_Rows();
	int cols = inputMatrix.get_Cols();
	int krows = kernel.get_Rows();
	int kcols = kernel.get_Cols();

	int begin = krows / 2;
	//Matrix<int> result(rows, cols);
	for (int r = -begin; r < rows - begin; r++)
	{
		for (int c = -begin; c < cols - begin; c++)
		{
			double sum = 0;
			for (int i = 0; i < krows; i++)
			{
				for (int j = 0; j < kcols; j++)
				{
					double kvalue = kernel.get_At_Position(i, j);
					double gValue =
						(i + r < 0 || j + c < 0 || i + r >= rows || j + c >= cols) ?
							getBorderValue(&inputMatrix, i + r, j + c) :
							(double) inputMatrix.get_At_Position(i + r, j + c);
					sum += kvalue * gValue;
				}
			}
			inputMatrix.set_At_Position(r + begin, c + begin, sum);
		}
	}
	return inputMatrix;
}
Matrix<double> gaussian_Blur_Double(Matrix<int> inputMatrix, Matrix<double> kernel)
{
	int rows = inputMatrix.get_Rows();
	int cols = inputMatrix.get_Cols();
	int krows = kernel.get_Rows();
	int kcols = kernel.get_Cols();

	int begin = krows / 2;
	Matrix<double> result(rows, cols);
	for (int r = -begin; r < rows - begin; r++)
	{
		for (int c = -begin; c < cols - begin; c++)
		{
			double sum = 0;
			for (int i = 0; i < krows; i++)
			{
				for (int j = 0; j < kcols; j++)
				{
					double kvalue = kernel.get_At_Position(i, j);
					double gValue =
						(i + r < 0 || j + c < 0 || i + r >= rows || j + c >= cols) ?
							getBorderValue(&inputMatrix, i + r, j + c) :
							(double) inputMatrix.get_At_Position(i + r, j + c);
					sum += kvalue * gValue;
				}
			}
			result.set_At_Position(r + begin, c + begin, sum);
		}
	}
	return result;
}
Matrix<int> Robert_Operation(ptr_IntMatrix grayMatrix)
{
	int rows = grayMatrix->get_Rows();
	int cols = grayMatrix->get_Cols();
	Matrix<int> dMatrix(rows, cols, 0);
	int v1 = 0, v2 = 0;
	int v3 = 0, v4 = 0;
	int gx = 0, gy = 0, gxy = 0;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			v1 = grayMatrix->get_At_Position(r, c);
			if (r + 1 < rows)
			{
				v2 = grayMatrix->get_At_Position(r + 1, c);
				if (c + 1 < cols)
				{
					v3 = grayMatrix->get_At_Position(r + 1, c + 1);
				}
			}
			if (c + 1 < cols)
			{
				v4 = grayMatrix->get_At_Position(r, c + 1);
			}
			gx = v1 - v3;
			gy = v4 - v2;
			gxy = abs(gx) + abs(gy);
			//gxy = pow(pow(gx, 2) + pow(gy, 2), 0.5);
			dMatrix.set_At_Position(r, c, gxy);
		}
	}
	return dMatrix;
}

Matrix<int> Sobel_Operation(ptr_IntMatrix grayMatrix)
{
	int rows = grayMatrix->get_Rows();
	int cols = grayMatrix->get_Cols();
	Matrix<int> dMatrix(rows, cols, 0);
	int v1 = 0, v2 = 0, v3 = 0;
	int v4 = 0, v5 = 0, v6 = 0;
	int v7 = 0, v8 = 0, v9 = 0;
	int gx = 0, gy = 0, gxy = 0;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			v1 = v2 = v3 = v4 = v5 = v6 = 0;
			v7 = v8 = v9 = gx = gy = gxy = 0;

			v5 = grayMatrix->get_At_Position(r, c);
			if (r - 1 >= 0)
			{
				v2 = grayMatrix->get_At_Position(r - 1, c);
				if (c - 1 >= 0)
				{
					v1 = grayMatrix->get_At_Position(r - 1, c - 1);
				}
				if (c + 1 < cols)
				{
					v3 = grayMatrix->get_At_Position(r - 1, c + 1);
				}
			}
			if (r + 1 < rows)
			{
				v8 = grayMatrix->get_At_Position(r + 1, c);
				if (c - 1 >= 0)
				{
					v7 = grayMatrix->get_At_Position(r + 1, c - 1);
				}
				if (c + 1 < cols)
				{
					v9 = grayMatrix->get_At_Position(r + 1, c + 1);
				}
			}
			if (c - 1 >= 0)
			{
				v4 = grayMatrix->get_At_Position(r, c - 1);
			}
			if (c + 1 < cols)
			{
				v6 = grayMatrix->get_At_Position(r, c + 1);
			}
			gx = ((v7 + 2 * v8 + v9) - (v1 + 2 * v2 + v3)) / 4;
			gy = ((v3 + 2 * v6 + v9) - (v1 + 2 * v4 + v7)) / 4;
			gxy = abs(gx) + abs(gy);
			dMatrix.set_At_Position(r, c, gxy);
		}
	}
	return dMatrix;
}
Matrix<double> Sobel_Operation_Double(ptr_IntMatrix grayMatrix)
{
	int rows = grayMatrix->get_Rows();
	int cols = grayMatrix->get_Cols();
	//cout<<"rows - cols: "<<rows <<"\t"<<cols<<endl;
	Matrix<double> dMatrix(rows, cols, 0.0);
	int v1 = 0, v2 = 0, v3 = 0;
	int v4 = 0, v5 = 0, v6 = 0;
	int v7 = 0, v8 = 0, v9 = 0;
	double gx = 0, gy = 0, gxy = 0;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			v1 = v2 = v3 = v4 = v5 = v6 = 0;
			v7 = v8 = v9 = gx = gy = gxy = 0;

			v5 = grayMatrix->get_At_Position(r, c);
			if (r - 1 >= 0)
			{
				v2 = grayMatrix->get_At_Position(r - 1, c);
				if (c - 1 >= 0)
				{
					v1 = grayMatrix->get_At_Position(r - 1, c - 1);
				}
				if (c + 1 < cols)
				{
					v3 = grayMatrix->get_At_Position(r - 1, c + 1);
				}
			}
			if (r + 1 < rows)
			{
				v8 = grayMatrix->get_At_Position(r + 1, c);
				if (c - 1 >= 0)
				{
					v7 = grayMatrix->get_At_Position(r + 1, c - 1);
				}
				if (c + 1 < cols)
				{
					v9 = grayMatrix->get_At_Position(r + 1, c + 1);
				}
			}
			if (c - 1 >= 0)
			{
				v4 = grayMatrix->get_At_Position(r, c - 1);
			}
			if (c + 1 < cols)
			{
				v6 = grayMatrix->get_At_Position(r, c + 1);
			}
			gx = (double)(((v7 + 2 * v8 + v9) - (v1 + 2 * v2 + v3))) / 4;
			gy = double(((v3 + 2 * v6 + v9) - (v1 + 2 * v4 + v7))) / 4;
			gxy = abs(gx) + abs(gy);
			dMatrix.set_At_Position(r, c, gxy);
			//cout<<"\t"<<round(atan(gy/gx) * 180/M_PI);
		}
		//cout<<"\n";
	}
	return dMatrix;
}

Matrix<int> post_Filter(Matrix<int> sobelResult)
{
	int rows = sobelResult.get_Rows();
	int cols = sobelResult.get_Cols();
	/*
	 * Inverse the result
	 */
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int value = sobelResult.get_At_Position(r, c);
			if (value == 0)
			{
				sobelResult.set_At_Position(r, c, 0);
			}
			else
			{
				sobelResult.set_At_Position(r, c, 255);
			}
		}
	}

	return sobelResult;
}

double intraClasses(double pro[256], int threshold)
{
	double weight1 = 0, weight2 = 0;
	for (int i = 0; i < 256; i++)
	{
		double p = pro[i];
		if (i < threshold)
		{
			weight1 += p;
		}
		else
		{
			weight2 += p;
		}
	}
	//cout<<"\nWeights: "<<weight1<<"\t"<<weight2<<endl;
	double muy1 = 0, muy2 = 0;
	for (int i = 0; i < 256; i++)
	{
		double p = pro[i];
		if (i < threshold)
		{
			muy1 += (i *(p/weight1));
		}
		else
		{
			muy2 += (i *(p/weight2));
		}
	}
	double sigma = weight1 * weight2 * pow((muy1 - muy2),2);
	return sigma;
}

int threshold_Otsu(Matrix<int> sobelResult)
{
	int rows = sobelResult.get_Rows();
	int cols = sobelResult.get_Cols();
	int values = rows * cols;
	int hist[256] =
	{ };
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int value = sobelResult.get_At_Position(r, c);
			hist[value] += 1;
		}
	}
	double pro[256] =
	{ };

	for (int i = 0; i < 256; i++)
	{
		pro[i] = (double)hist[i] / (double)values;
	}
	double sigmaMax = 0;
	int index = 0;
	for (int i = 0; i < 256; i++) {
		double sigma = intraClasses(pro,i);
		if(sigma > sigmaMax)
		{
			sigmaMax = sigma;
			index = i;
		}
	}
	return index;
}

// =================================== Some binary operations =============================
ptr_IntMatrix erode(ptr_IntMatrix binMatrix, int kernelSize)
{
	if (kernelSize % 2 == 0)
		kernelSize += 1;
	int hSize = kernelSize / 2;
	int rows = binMatrix->get_Rows();
	int cols = binMatrix->get_Cols();
	ptr_IntMatrix result = new Matrix<int>(rows, cols, 0);
	*result = *binMatrix;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int countValue = 0;
			for (int rs = r - hSize; rs <= r + hSize; rs++)
			{
				for (int cs = c - hSize; cs <= c + hSize; cs++)
				{
					int value;
					if (rs < 0 || rs >= rows || cs < 0 || cs >= cols)
					{
						value = 0;
					}
					else
					{
						value = binMatrix->get_At_Position(rs, cs);
					}
					if (value == 0)
						countValue++;
				}
			}
			if (countValue != (kernelSize * kernelSize))
				result->set_At_Position(r, c, 255);
		}
	}
	return result;
}
ptr_IntMatrix dilate(ptr_IntMatrix binMatrix, int kernelSize)
{
	if (kernelSize % 2 == 0)
		kernelSize += 1;
	int hSize = kernelSize / 2;
	int rows = binMatrix->get_Rows();
	int cols = binMatrix->get_Cols();
	ptr_IntMatrix result = new Matrix<int>(rows, cols, 0);
	*result = *binMatrix;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int countValue = 0;
			for (int rs = r - hSize; rs <= r + hSize; rs++)
			{
				for (int cs = c - hSize; cs <= c + hSize; cs++)
				{
					int value;
					if (rs < 0 || rs >= rows || cs < 0 || cs >= cols)
					{
						value = 0;
					}
					else
					{
						value = binMatrix->get_At_Position(rs, cs);
					}
					if (value == 0)
						countValue++;
				}
			}
			if (countValue != 0)
				result->set_At_Position(r, c, 0);
		}
	}
	return result;
}

ptr_IntMatrix open_Binary(ptr_IntMatrix binMatrix, int kernelSize)
{
	ptr_IntMatrix erosion = erode(binMatrix, kernelSize);
	return dilate(erosion, kernelSize);
}

ptr_IntMatrix close_Binary(ptr_IntMatrix binMatrix, int kernelSize)
{
	ptr_IntMatrix dilation = dilate(binMatrix, kernelSize);
	return erode(dilation, kernelSize);
}
