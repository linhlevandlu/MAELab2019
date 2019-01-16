/*
 * ImageConvert.cpp
 *
 *  Created on: Dec 19, 2016
 *      Author: linh
 */



#include "ImageConvert.h"

QImage ptr_Int_To_QImage(Matrix<int> inputMatrix)
{
	static QVector<QRgb> sColorTable;

	// only create our color table once
	if (sColorTable.isEmpty())
	{
		for (int i = 0; i < 256; i++)
			sColorTable.push_back(qRgb(i, i, i));
	}
	int width = inputMatrix.get_Cols();
	int height = inputMatrix.get_Rows();

	QImage image(width, height, QImage::Format_Indexed8);
	image.setColorTable(sColorTable);
	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c++)
		{
			int value = inputMatrix.get_At_Position(r, c);
			image.setPixel(c, r, value);
		}
	}

	return image;
}
QImage ptr_Double_To_QImage(Matrix<double> inputMatrix)
{
	static QVector<QRgb> sColorTable;

	// only create our color table once
	if (sColorTable.isEmpty())
	{
		for (int i = 0; i < 256; i++)
			sColorTable.push_back(qRgb(i, i, i));
	}
	int width = inputMatrix.get_Cols();
	int height = inputMatrix.get_Rows();

	QImage image(width, height, QImage::Format_Indexed8);
	image.setColorTable(sColorTable);
	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c++)
		{
			int value = inputMatrix.get_At_Position(r, c);
			image.setPixel(c, r, value);
		}
	}

	return image;
}

QImage ptr_RGB_To_QImage(Matrix<RGB> inputMatrix)
{
	int width = inputMatrix.get_Cols();
	int height = inputMatrix.get_Rows();

	QImage image(width, height, QImage::Format_RGB888);
	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c++)
		{
			RGB value = inputMatrix.get_At_Position(r, c);
			image.setPixel(c, r, qRgb(value.get_R(),value.get_G(),value.get_B()));
		}
	}
	return image;
}
