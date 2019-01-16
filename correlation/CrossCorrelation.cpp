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

#include "CrossCorrelation.h"


ptr_IntMatrix createTemplate(ptr_IntMatrix inputImage, Point centerPoint,
	int tsize, Point &location, Point &distance, Point &rlocation)
{
	int hsize = tsize / 2;
	int cx = centerPoint.get_X(); //col
	int cy = centerPoint.get_Y(); // row
	if (cx < 0 || cy < 0)
		return inputImage;
	int rows = inputImage->get_Rows();
	int cols = inputImage->get_Cols();

	int lx = (cx - hsize) < 0 ? 0 : (cx - hsize);
	int ly = (cy - hsize) < 0 ? 0 : (cy - hsize);
	location.set_X(lx);
	location.set_Y(ly);
	distance.set_X(cx - lx);
	distance.set_Y(cy - ly);

	int rx = (cx + hsize) >= cols ? cols - 1 : (cx + hsize);
	int ry = (cy + hsize) >= rows ? rows - 1 : (cy + hsize);
	rlocation.set_X(rx);
	rlocation.set_Y(ry);

	ptr_IntMatrix result = new Matrix<int>(ry - ly, rx - lx);
	int i = -1, j = -1;
	for (int row = ly; row < ry; row++)
	{
		i++;
		j = -1;
		for (int col = lx; col < rx; col++)
		{
			j++;
			if (i >= 0 && i < ry - ly && j >= 0 && j < rx - lx)
			{
				result->set_At_Position(i, j, inputImage->get_At_Position(row, col));
			}
		}
	}
	return result;

}

Point matCrossCorrelation(ptr_IntMatrix templ, ptr_IntMatrix image)
{
	int width = image->get_Cols() - templ->get_Cols() + 1;
	int height = image->get_Rows() - templ->get_Rows() + 1;

	int imgrows = image->get_Rows();
	int imgcols = image->get_Cols();
	int tmprows = templ->get_Rows();
	int tmpcols = templ->get_Cols();
	Point location(0, 0);

	if (width > 0 && height > 0)
	{
		double maxCoeff = 0;
		for (int row = 0; row <= imgrows - tmprows; row++)
		{
			for (int col = 0; col <= imgcols - tmpcols; col++)
			{
				double acoeff = 0, bcoeff1 = 0, bcoeff2 = 0;
				for (int r = 0; r < tmprows; r++)
				{
					for (int c = 0; c < tmpcols; c++)
					{
						double te = templ->get_At_Position(r, c);
						double im = image->get_At_Position(row + r, col + c);

						acoeff += (te * im);
						bcoeff1 += (te * te);
						bcoeff2 += (im * im);
					}
				}
				double coeff;
				if (bcoeff1 == 0 || bcoeff2 == 0)
				{
					coeff = 0;
				}
				else
				{
					coeff = acoeff / sqrt(bcoeff1 * bcoeff2);
				}
				if (coeff > maxCoeff)
				{
					maxCoeff = coeff;
					location.set_X(col);
					location.set_Y(row);
				}
			}
		}
	}
	return location;
}

vector<Point> verify_Landmarks(Image mImage, Image sImage,
	vector<Point> manualLM, vector<Point> esLandmarks, int templSize,
	int sceneSize, double angleDiff, Point ePoint)
{
	Matrix<int> mgrayData = mImage.get_Gray_Matrix();
	ptr_IntMatrix mMatrix =&mgrayData;
	//sImage.rotate(ePoint, angleDiff, 1);
	Matrix<int> sgrayData = sImage.get_Gray_Matrix();
	ptr_IntMatrix sRotateImg = &sgrayData;
	int width = mMatrix->get_Cols();
	int height = mMatrix->get_Rows();

	std::vector<Point> mcResult;
	Point epi;
	Point mpi;
	Point tLocation(0, 0);
	Point tDistance(0, 0);
	Point iLocation(0, 0);
	Point iDistance(0, 0);
	Point trLocation(0,0),irLocation(0,0);
	ptr_IntMatrix templ;
	ptr_IntMatrix sceneM;
	Point maxLoc(0, 0);
	for (size_t i = 0; i < esLandmarks.size(); i++)
	{
		epi = esLandmarks.at(i);
		mpi = manualLM.at(i);
		if (epi.get_X() > 0 && epi.get_Y() > 0 && epi.get_X() < width
			&& epi.get_Y() < height)
		{
			tLocation.set_X(0);
			tLocation.set_Y(0);
			tDistance.set_X(0);
			tDistance.set_Y(0);
			iLocation.set_X(0);
			iLocation.set_Y(0);
			iDistance.set_X(0);
			iDistance.set_Y(0);
			maxLoc.set_X(0);
			maxLoc.set_Y(0);

			templ = createTemplate(mMatrix, mpi, templSize, tLocation, tDistance,trLocation);
			sceneM = createTemplate(sRotateImg, epi, sceneSize, iLocation, iDistance,irLocation);

			maxLoc = matCrossCorrelation(templ, sceneM);
			int lmx = iLocation.get_X() + maxLoc.get_X() + tDistance.get_X();
			int lmy = iLocation.get_Y() + maxLoc.get_Y() + tDistance.get_Y();
			mcResult.push_back(Point(lmx, lmy));
		}

	}

	Point p;
	for (size_t k = 0; k < mcResult.size(); k++)
	{
		p = mcResult.at(k);
		cout << "\nEstimated landmark : " << p.get_X() << "\t" << p.get_Y();

	}

	return mcResult;
}
vector<Point> verify_Landmarks_2(ptr_IntMatrix mImage, ptr_IntMatrix sImage,
	vector<Point> manualLM, vector<Point> esLandmarks, int templSize,
	int sceneSize)
{
	int width = mImage->get_Cols();
	int height = mImage->get_Rows();
	ptr_IntMatrix mMatrix = new Matrix<int>(height, width, 0);
	*mMatrix = *(mImage);
	ptr_IntMatrix sMatrix = new Matrix<int>(height, width, 0);
	*sMatrix = *(sImage);

	std::vector<Point> mcResult;
	Point epi;
	Point mpi;
	Point tLocation(0, 0);
	Point tDistance(0, 0);
	Point iLocation(0, 0);
	Point iDistance(0, 0);
	Point trLocation(0,0),irLocation(0,0);
	ptr_IntMatrix templ;
	ptr_IntMatrix sceneM;
	Point maxLoc(0, 0);
	size_t limit = 0;
	if (manualLM.size() < esLandmarks.size())
		limit = manualLM.size();
	else
		limit = esLandmarks.size();
	for (size_t i = 0; i < limit; i++)
	{
		epi = esLandmarks.at(i);
		mpi = manualLM.at(i);
		if (epi.get_X() >= 0 && epi.get_Y() >= 0 && epi.get_X() < width
			&& epi.get_Y() < height)
		{
			tLocation.set_X(0);
			tLocation.set_Y(0);
			tDistance.set_X(0);
			tDistance.set_Y(0);
			iLocation.set_X(0);
			iLocation.set_Y(0);
			iDistance.set_X(0);
			iDistance.set_Y(0);
			maxLoc.set_X(0);
			maxLoc.set_Y(0);

			templ = new Matrix<int>(
				*createTemplate(mMatrix, mpi, templSize, tLocation, tDistance,trLocation));
			sceneM = new Matrix<int>(
				*createTemplate(sMatrix, epi, sceneSize, iLocation, iDistance,irLocation));

			maxLoc = matCrossCorrelation(templ, sceneM);
			int lmx = iLocation.get_X() + maxLoc.get_X() + tDistance.get_X();
			int lmy = iLocation.get_Y() + maxLoc.get_Y() + tDistance.get_Y();
			mcResult.push_back(Point(lmx, lmy));
			delete templ;
			delete sceneM;
		}else
		{
			mcResult.push_back(epi);
		}

	}
	delete mMatrix;
	delete sMatrix;
	return mcResult;
}

