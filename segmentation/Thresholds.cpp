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


#include "Thresholds.h"

ptr_IntMatrix binary_Threshold(ptr_IntMatrix inputMatrix, int tValue,
	int maxValue)
{
	int rows = inputMatrix->get_Rows();
	int cols = inputMatrix->get_Cols();

	/*Matrix<double> kernel = getGaussianKernel(9,1);
	 *inputMatrix = gaussianBlur(*inputMatrix,kernel);*/

	ptr_IntMatrix result = new Matrix<int>(rows, cols, maxValue);
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (inputMatrix->get_At_Position(r, c) > tValue)
				result->set_At_Position(r, c, maxValue);
			else
				result->set_At_Position(r, c, 0);
		}
	}

	return result;
}

ptr_IntMatrix post_Process(ptr_IntMatrix binaryMatrix, int maxValue)
{
	int rows = binaryMatrix->get_Rows();
	int cols = binaryMatrix->get_Cols();
	ptr_IntMatrix result = new Matrix<int>(rows, cols, maxValue);
	*result = *binaryMatrix;
	Point left(0, 0), right(0, 0);
	vector<Line> lines;

	for (int r = 1; r < rows - 1; r++)
	{
		for (int c = 1; c < cols - 1; c++)
		{
			left.set_X(0);
			left.set_Y(0);
			right.set_X(0);
			right.set_Y(0);
			lines.clear();
			int value = binaryMatrix->get_At_Position(r, c);
			int valuel = binaryMatrix->get_At_Position(r, c - 1);
			if (value == maxValue && valuel == 0)
			{

				// xac dinh diem dau tien
				left.set_X(c - 1);
				left.set_Y(r);
				if (left.get_X() != 0)
				{
					// xac dinh diem thu 2
					for (int k = c; k < cols; k++)
					{
						if (binaryMatrix->get_At_Position(r, k) == 0
							&& binaryMatrix->get_At_Position(r, k - 1) == maxValue)
						{
							right.set_X(k);
							right.set_Y(r);
							goto checkbghole;
						}
					}
				}
				// kiem tra hole

				checkbghole: if (left != 0 && right != 0 && right.get_X() > left.get_X())
				{
					// do dong truoc do
					bool beginHole = true;
					if (r - 1 < 0 || r - 1 >= rows)
						beginHole = false;
					else
					{
						for (int l = left.get_X() + 1; l <= right.get_X() - 1; l++)
						{
							if (binaryMatrix->get_At_Position(r - 1, l) == maxValue)
							{
								beginHole = false;
								goto resetValues;
							}
						}
					}
					resetValues: if (beginHole == false)
					{
						left.set_X(0);
						left.set_Y(0);
						right.set_X(0);
						right.set_Y(0);
					}
				}
				if (left != 0 && right != 0)
				{
					bool inhole = true;
					int rnew = r + 1;
					int clnew = 0, crnew = 0;
					lines.push_back(Line(left, right));
					do
					{
						clnew = 0;
						crnew = 0;
						if (rnew < rows)
						{
							// check dong do
							inhole = false;
							for (int n = left.get_X() + 1; n <= right.get_X() - 1; n++)
							{
								if (n < cols && rnew < rows
									&& binaryMatrix->get_At_Position(rnew, n) == maxValue)
								{
									inhole = true;
									goto checkinhole;
								}
							}
							checkinhole: if (inhole)
							{
								// check left;
								int mindist = DBL_MAX;
								for (int l = left.get_X() - 10; l <= left.get_X() + 10; l++)
								{
									if (binaryMatrix->get_At_Position(rnew, l) == 0)
									{
										if (abs(left.get_X() - l) < mindist)
										{
											clnew = l;
											mindist = abs(left.get_X() - l);
										}
									}
								}

								mindist = DBL_MAX;
								checkright: for (int m = right.get_X() - 10;
									m <= right.get_X() + 10; m++)
								{
									if (binaryMatrix->get_At_Position(rnew, m) == 0)
									{
										if (abs(right.get_X() - m) < mindist)
										{
											crnew = m;
											mindist = abs(right.get_X() - m);
											//goto checkandpush;
										}
									}
								}
								/*if (binaryMatrix->get_At_Position(rnew, right.get_X())
								 == maxValue)
								 {
								 for (int m = right.get_X(); m >= left.get_X(); m--)
								 {
								 if (binaryMatrix->get_At_Position(rnew, m) == 0)
								 {
								 crnew = m;
								 goto checkandpush;
								 }
								 }

								 }*/
								checkandpush: if (clnew != 0 && crnew != 0)
								{
									left.set_X(clnew);
									left.set_Y(rnew);
									right.set_X(crnew);
									right.set_Y(rnew);
									lines.push_back(Line(left, right));
								}

							}
						}
						else
						{
							lines.clear();
							inhole = false;
						}
						rnew++;

					}
					while (inhole);

					for (size_t li = 0; li < lines.size(); li++)
					{
						Line line = lines.at(li);
						if (line.get_Begin().get_Y() == line.get_End().get_Y())
						{
							for (int x = line.get_Begin().get_X(); x < line.get_End().get_X();
								x++)
							{
								binaryMatrix->set_At_Position(line.get_Begin().get_Y(), x, 0);
							}
						}
					}
					lines.clear();
				}

			}
		}
	}

	return binaryMatrix;
}

int mean_Histogram(Matrix<double> channel, double totalPixels, int &maxPosition)
{
	double total = 0;
	maxPosition = -1;
	double maxValue = -1;
	for (int c = 0; c < channel.get_Cols(); c++)
	{
		double cValue = channel.get_At_Position(0, c);
		total += (c * cValue);
		if (cValue > maxValue)
		{
			maxValue = cValue;
			maxPosition = c;
		}
	}
	return total / totalPixels;
}

ptr_RGBMatrix color_Threshold(ptr_RGBMatrix rgbImage,
	ptr_RGBMatrix colorHistogram)
{
	//ptr_RGBMatrix rgbImage = inputImage.getRGBMatrix();
	int rows = rgbImage->get_Rows();
	int cols = rgbImage->get_Cols();
	double totalPixels = rows * cols;

	//ptr_RGBMatrix colorHistogram = inputImage.getRGBHistogram(); // matrix with 1 row
	Matrix<double> redHistogram(1, colorHistogram->get_Cols(), 0);
	Matrix<double> greenHistogram(1, colorHistogram->get_Cols(), 0);
	Matrix<double> blueHistogram(1, colorHistogram->get_Cols(), 0);
	for (int c = 0; c < colorHistogram->get_Cols(); c++)
	{
		RGB color = colorHistogram->get_At_Position(0, c);
		redHistogram.set_At_Position(0, c, color.get_R());
		greenHistogram.set_At_Position(0, c, color.get_G());
		blueHistogram.set_At_Position(0, c, color.get_B());
	}
	int maxRed = -1, maxGreen = -1, maxBlue = -1;
	int meanRed = mean_Histogram(redHistogram, totalPixels, maxRed);
	int meanGreen = mean_Histogram(greenHistogram, totalPixels, maxGreen);
	int meanBlue = mean_Histogram(blueHistogram, totalPixels, maxBlue);
	cout << "\nMean Red - Max Red: " << meanRed << "\t" << maxRed;
	cout << "\nMean Green - Max Green: " << meanGreen << "\t" << maxGreen;
	cout << "\nMean Blue - Max Blue: " << meanBlue << "\t" << maxBlue;

	int tRed = 0, tBlue = 0;
	int tGreen = meanGreen;
	tRed = maxRed - (maxRed * 0.2);
	tBlue = maxBlue - (maxBlue * 0.2);
	tGreen = maxGreen - (maxGreen * 0.2);
	cout << "\nThreshold: " << tRed << "\t" << tGreen << "\t" << tBlue << endl;
	RGB color;
	ptr_RGBMatrix thresh = new Matrix<RGB>(rows, cols, color);
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			color = rgbImage->get_At_Position(r, c);
			if (color.get_R() >= tRed)
				color.set_R(0);
			if (color.get_G() >= tGreen)
				color.set_G(0);
			if (color.get_B() >= tBlue)
				color.set_B(0);
			thresh->set_At_Position(r, c, color);
		}
	}
	ptr_RGBMatrix result(rgbImage);
	RGB bck;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			color = thresh->get_At_Position(r, c);
			if (color == 0)
				result->set_At_Position(r, c, bck);
		}
	}
	return result;
}
