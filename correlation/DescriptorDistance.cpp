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

#include "DescriptorDistance.h"

Matrix<double> createDescriptor(ptr_IntMatrix mImage, Point lefttop,
	Point rightbot, Matrix<double> &gradient)
{
	int rows = mImage->get_Rows();
	int cols = mImage->get_Cols();
	int rpatch = rightbot.get_Y() - lefttop.get_Y() + 1;
	int cpatch = rightbot.get_X() - lefttop.get_X() + 1;
	Matrix<double> orientation(rpatch, cpatch, 0.0);
	int llocation = 0, rlocation = 0, alocation = 0, blocation = 0;
	int i = 0, j = 0;
	for (int r = lefttop.get_Y(); r <= rightbot.get_Y(); r++)
	{
		//cout<<"\n";
		for (int c = lefttop.get_X(); c <= rightbot.get_X(); c++)
		{
			//cout<<mImage->get_At_Position(r,c)<<"\t";
			llocation = rlocation = alocation = blocation = 0;

			if (r - 1 >= 0 && r - 1 < rows)
			{
				alocation = mImage->get_At_Position(r - 1, c);
			}
			if (r + 1 >= 0 && r + 1 < rows)
			{
				blocation = mImage->get_At_Position(r + 1, c);
			}
			if (c - 1 >= 0 && c - 1 < cols)
			{
				llocation = mImage->get_At_Position(r, c - 1);
			}
			if (c + 1 >= 0 && c + 1 < cols)
			{
				rlocation = mImage->get_At_Position(r, c + 1);
			}
			double dy = (double) blocation - (double) alocation;
			double dx = (double) rlocation - (double) llocation;
			double angle= atan2(dy,dx) * 180/M_PI;
			if (angle < 0)
					angle += 360;
				orientation.set_At_Position(i, j, angle);
			/*if (d2 != 0)
			{
				//double angle = atan(d1 / d2) * 180 / M_PI;
				double angle = atan2(d2, d1) * 180 / M_PI;
				if (angle < 0)
					angle += 360;
				orientation.set_At_Position(i, j, angle);

			}
			else
			{
				orientation.set_At_Position(i, j, 0.0);
			}*/
			double magnutide = (dx * dx) + (dy * dy);
			gradient.set_At_Position(i, j, sqrt(magnutide));
			j++;
		}
		i++;
		j = 0;
	}

	return orientation;
}

Matrix<double> createDescriptor_Double(ptr_DoubleMatrix mImage, Point lefttop,
	Point rightbot, Matrix<double> &gradient)
{
	int rows = mImage->get_Rows();
	int cols = mImage->get_Cols();
	int rpatch = rightbot.get_Y() - lefttop.get_Y() + 1;
	int cpatch = rightbot.get_X() - lefttop.get_X() + 1;
	Matrix<double> orientation(rpatch, cpatch, 0.0);
	double llocation = 0, rlocation = 0, alocation = 0, blocation = 0;
	int i = 0, j = 0;
	for (int r = lefttop.get_Y(); r <= rightbot.get_Y(); r++)
	{
		//cout<<"\n";
		for (int c = lefttop.get_X(); c <= rightbot.get_X(); c++)
		{
			//cout<<mImage->get_At_Position(r,c)<<"\t";
			llocation = rlocation = alocation = blocation = 0;

			if (r - 1 >= 0 && r - 1 < rows)
			{
				alocation = mImage->get_At_Position(r - 1, c);
			}
			if (r + 1 >= 0 && r + 1 < rows)
			{
				blocation = mImage->get_At_Position(r + 1, c);
			}
			if (c - 1 >= 0 && c - 1 < cols)
			{
				llocation = mImage->get_At_Position(r, c - 1);
			}
			if (c + 1 >= 0 && c + 1 < cols)
			{
				rlocation = mImage->get_At_Position(r, c + 1);
			}
			double dy = (double) alocation - (double) blocation;
			double dx = (double) rlocation - (double) llocation;

			double angle= atan2(dy,dx) * 180/M_PI;
			if (angle < 0)
				angle += 360;
			orientation.set_At_Position(i, j, angle);
			//cout<<"\t"<<dx <<"\t"<<dy<<"\t"<<angle<<endl;
			/*if (d2 != 0)
			{
				//double angle = atan(d1 / d2) * 180 / M_PI;
				double angle = atan2(d2, d1) * 180 / M_PI;
				if (angle < 0)
					angle += 360;
				orientation.set_At_Position(i, j, angle);

			}
			else
			{
				orientation.set_At_Position(i, j, 0.0);
			}*/
			double magnutide = (dx * dx) + (dy * dy);
			gradient.set_At_Position(i, j, sqrt(magnutide));
			j++;
		}
		i++;
		j = 0;
	}

	return orientation;
}

// Compute the vector descriptor for a small patch 4x4
// Output: the vector with 8-dimensions of the descriptor
vector<double> orientHist4(Matrix<double> gradient, Matrix<double> orientation,
	int rbegin, int cbegin, int size) // default size = 4
{
	vector<double> histograms;
	histograms.resize(8, 0.0);
	double grad = 0.0, orien = 0.0;
	//cout<<"\n";
	for (int r = rbegin; r < rbegin + size; r++)
	{
		for (int c = cbegin; c < cbegin + size; c++)
		{
			if (r >= 0 && c >= 0 && r < gradient.get_Rows() && c < gradient.get_Cols())
			{
				grad = gradient.get_At_Position(r, c);
				orien = orientation.get_At_Position(r, c);
				//cout<<"\t"<<orien;
				int bin = ceil(orien / 45);
				bin = (bin == 0) ? 0 : (bin - 1);
				histograms.at(bin) += grad;
			}
		}
	}
	//cout<<"\n";
	return histograms;
}

// Compute the descriptor for whole the patch (includes many 4x4 patches)
vector<double> orientHist16(Matrix<double> gradient, Matrix<double> orientation,
	int subsize)
{
	vector<double> histograms, hist;
	for (int r = 0; r < gradient.get_Rows(); r += subsize)
	{
		for (int c = 0; c < gradient.get_Cols(); c += subsize)
		{
			
			hist.resize(8, 0.0);
			hist = orientHist4(gradient, orientation, r, c, subsize);
			histograms.insert(histograms.end(), hist.begin(), hist.end());
		}
		
	}
	// normalize the vector (twice)
	double totalValue = 0;
	for (size_t i = 0; i < histograms.size(); i++)
	{
		totalValue += (histograms.at(i) * histograms.at(i));
	}
	totalValue = sqrt(totalValue);
	//  normalizing and theshold < 0.2
	if (totalValue > 0)
	{
		double totalValue2 = 0.0;
		for (int i = 0; i < histograms.size(); i++)
		{
			histograms.at(i) /= totalValue;
			if (histograms.at(i) < 0.2)
				histograms.at(i) = 0;
			totalValue2 += (histograms.at(i) * histograms.at(i));
		}
		//re-normalize
		for (size_t i = 0; i < histograms.size(); i++)
		{
			histograms.at(i) /= totalValue2;
		}
	}

	return histograms;
}

// Compute the L2 distance between two vectors
double l2Distance(vector<double> sourceTarget, vector<double> targetHist)
{
	double distance = 0;
	size_t size = sourceTarget.size();
	if (size == targetHist.size())
	{
		double temp = 0;
		for (size_t i = 0; i < size; i++)
		{
			temp += pow(sourceTarget.at(i) - targetHist.at(i), 2.0);
		}
		distance = sqrt(temp);
	}
	return distance;
}

// Extract a patch in a int_matrix with psize and center point at center
// The method return the top-left and bottom-right corner of patch
Point createPatch(ptr_IntMatrix imageMatrix, int psize, Point center,
	Point &right)
{
	Point left(0, 0);
	right = left;
	int hsize = psize / 2;
	int cx = center.get_X();
	int cy = center.get_Y();
	int rows = imageMatrix->get_Rows();
	int cols = imageMatrix->get_Cols();

	int lx = (cx - hsize) < 0 ? 0 : (cx - hsize);
	int ly = (cy - hsize) < 0 ? 0 : (cy - hsize);
	left.set_X(lx);
	left.set_Y(ly);

	int tempx = cx + hsize;
	int tempy = cy + hsize;
	tempx = (psize % 2 != 0) ? tempx : tempx - 1;
	tempy = (psize % 2 != 0) ? tempy : tempy - 1;
	int rx = tempx >= cols ? cols - 1 : tempx;
	int ry = tempy >= rows ? rows - 1 : tempy;
	right.set_X(rx);
	right.set_Y(ry);
	return left;
}
Point createPatch_Double(ptr_DoubleMatrix imageMatrix, int psize, Point center,
	Point &right)
{
	Point left(0, 0);
	right = left;
	int hsize = psize / 2;
	int cx = center.get_X();
	int cy = center.get_Y();
	int rows = imageMatrix->get_Rows();
	int cols = imageMatrix->get_Cols();

	int lx = (cx - hsize) < 0 ? 0 : (cx - hsize);
	int ly = (cy - hsize) < 0 ? 0 : (cy - hsize);
	left.set_X(lx);
	left.set_Y(ly);

	int tempx = cx + hsize;
	int tempy = cy + hsize;
	tempx = (psize % 2 != 0) ? tempx : tempx - 1;
	tempy = (psize % 2 != 0) ? tempy : tempy - 1;
	int rx = tempx >= cols ? cols - 1 : tempx;
	int ry = tempy >= rows ? rows - 1 : tempy;
	right.set_X(rx);
	right.set_Y(ry);
	return left;
}
// Find the point in lsPoints that nearest with p
Point nearest_Point(vector<Point> lsPoints, Point p)
{
	double minDistance = DBL_MAX;
	Point ePoint, mPoint;
	for (size_t j = 0; j < lsPoints.size(); j++)
	{
		ePoint = lsPoints.at(j);
		Line ltemp(p, ePoint);
		if (ltemp.get_Length() < minDistance)
		{
			minDistance = ltemp.get_Length();
			mPoint.set_X(ePoint.get_X());
			mPoint.set_Y(ePoint.get_Y());
		}
	}
	return mPoint;
}

vector<double> SIFT_Descriptor(ptr_IntMatrix imgMatrix, Point center, int size)
{
	Point mright(0, 0);
// Apply gaussian blur before calculating the SIFT descriptor
	/*Matrix<double> kernelGaussian = getGaussianKernel(5,1.6);
	Matrix<double> gausMatrix = gaussianBlur_Double(*imgMatrix, kernelGaussian);
	Point mleft = createPatch_Double(&gausMatrix, size, center, mright);
	Matrix<double> mgradient(mright.get_Y() - mleft.get_Y() + 1,
		mright.get_X() - mleft.get_X() + 1, 0.0);
	Matrix<double> mOrient = createDescriptor_Double(&gausMatrix, mleft, mright,
		mgradient);
	vector<double> mHistogram = orientHist16(mgradient, mOrient, 3);*/

	Point mleft = createPatch(imgMatrix, size, center, mright);
	Matrix<double> mgradient(mright.get_Y() - mleft.get_Y() + 1,
		mright.get_X() - mleft.get_X() + 1, 0.0);
	Matrix<double> mOrient = createDescriptor(imgMatrix, mleft, mright,
		mgradient);
	vector<double> mHistogram = orientHist16(mgradient, mOrient, 3);
	return mHistogram;
}

// sceneSize and templSize should be is multiple of 16

vector<Point> verify_Descriptors(ptr_IntMatrix model, ptr_IntMatrix scene,
	vector<Point> manualLM, vector<Point> esLandmarks, int templSize,
	int sceneSize)
{
	vector<Point> result;
	int width = model->get_Cols();
	int height = model->get_Rows();
	size_t limit = 0;
	if (manualLM.size() < esLandmarks.size())
		limit = manualLM.size();
	else
		limit = esLandmarks.size();
	cout << "\nLimit: " << limit << endl;
	Point mpi, epi;
	for (size_t i = 0; i < limit; i++)
	{
		Point mleft(0, 0), mright(0, 0), sleft(0, 0), sright(0, 0);
		epi = esLandmarks.at(i);
		mpi = manualLM.at(i);
		if (epi.get_X() >= 0 && epi.get_Y() >= 0 && epi.get_X() < width
			&& epi.get_Y() < height)
		{

			/*mleft = createPatch(model, templSize, mpi, mright);
			 Matrix<double> mgradient(mright.get_Y() - mleft.get_Y() + 1,
			 mright.get_X() - mleft.get_X() + 1, 0.0);
			 Matrix<double> mOrient = createDescriptor(model, mleft, mright,
			 mgradient);
			 vector<double> mHistogram = orientHist16(mgradient, mOrient, templSize);*/
			vector<double> mHistogram;
			mHistogram = SIFT_Descriptor(model, mpi, templSize);
			double minDistance = DBL_MAX, maxDistance = 0.0;
			Point minPoint(0, 0), maxPoint(0, 0);
			//mpi.toString();
			sleft = createPatch(scene, sceneSize, epi, sright);
			int count = 0;
			for (int r = sleft.get_Y(); r < sright.get_Y(); r++)
			{
				for (int c = sleft.get_X(); c < sright.get_X(); c++)
				{
					if (scene->get_At_Position(r, c) == 255)
					{
						Point p(c, r);
						/*Point ssleft(0, 0), ssright(0, 0);
						 ssleft = createPatch(scene, templSize, p, ssright);
						 Matrix<double> sgradient(ssright.get_Y() - ssleft.get_Y() + 1,
						 ssright.get_X() - ssleft.get_X() + 1, 0.0);
						 Matrix<double> sOrient = createDescriptor(scene, ssleft, ssright,
						 sgradient);
						 vector<double> sHistogram = orientHist16(sgradient, sOrient,
						 templSize);*/
						vector<double> sHistogram = SIFT_Descriptor(scene, p, templSize);
						double distance = l2Distance(mHistogram, sHistogram);
						//cout << "\nDistance: " << distance << endl;
						if (distance > maxDistance)
						{
							maxDistance = distance;
							maxPoint.set_X(c);
							maxPoint.set_Y(r);
						}
						if (distance == minDistance)
						{
							count++;
						}
						if (distance < minDistance)
						{
							minDistance = distance;
							minPoint.set_X(c);
							minPoint.set_Y(r);
						}
					}
				}
			}
			//cout << "\nDuplicate: " << count << endl;
			result.push_back(minPoint);
		}
	}
	return result;
}

/*
 * Create the descriptor for each estimated landmark.
 * But firstly is find out the location of closest point in the contours.
 */
vector<Point> verify_Descriptors_2(ptr_IntMatrix model, ptr_IntMatrix scene,
	vector<Point> scenePoints, vector<Point> manualLM, vector<Point> esLandmarks,
	int templSize, int sceneSize)
{
	vector<Point> result;
	int width = model->get_Cols();
	int height = model->get_Rows();
	size_t limit = 0;
	if (manualLM.size() < esLandmarks.size())
		limit = manualLM.size();
	else
		limit = esLandmarks.size();
	Point mpi, epi;
	for (size_t i = 0; i < limit; i++)
	{
		Point mleft(0, 0), mright(0, 0), sleft(0, 0), sright(0, 0);
		epi = esLandmarks.at(i);
		mpi = manualLM.at(i);
		if (epi.get_X() >= 0 && epi.get_Y() >= 0 && epi.get_X() < width
			&& epi.get_Y() < height)
		{

			vector<double> mHistogram = SIFT_Descriptor(model, mpi, templSize);
			/*mleft = createPatch(model, templSize, mpi, mright);
			 Matrix<double> mgradient(mright.get_Y() - mleft.get_Y() + 1,
			 mright.get_X() - mleft.get_X() + 1, 0.0);
			 Matrix<double> mOrient = createDescriptor(model, mleft, mright,
			 mgradient);
			 vector<double> mHistogram = orientHist16(mgradient, mOrient, templSize);*/
			epi = nearest_Point(scenePoints, epi); // find the closest point with estimated landmark
			sleft = createPatch(scene, sceneSize, epi, sright);
			double minDistance = DBL_MAX;
			Point minPoint(0, 0), maxPoint(0, 0);
			//
			int count = 0;
			for (int r = sleft.get_Y(); r < sright.get_Y(); r++)
			{
				for (int c = sleft.get_X(); c < sright.get_X(); c++)
				{
					Point p(c, r);
					/*Point ssleft(0, 0), ssright(0, 0);
					 ssleft = createPatch(scene, templSize, p, ssright);
					 Matrix<double> sgradient(ssright.get_Y() - ssleft.get_Y() + 1,
					 ssright.get_X() - ssleft.get_X() + 1, 0.0);
					 Matrix<double> sOrient = createDescriptor(scene, ssleft, ssright,
					 sgradient);
					 vector<double> sHistogram = orientHist16(sgradient, sOrient,
					 templSize);*/
					vector<double> sHistogram = SIFT_Descriptor(scene, p, templSize);
					double distance = l2Distance(mHistogram, sHistogram);
					//cout << "\nDistance: " << distance << endl;
					if (distance == minDistance)
					{
						count++;
					}
					if (distance < minDistance)
					{
						minDistance = distance;
						minPoint.set_X(c);
						minPoint.set_Y(r);
					}

				}
			}
			//cout << "\nDuplicate: " << count << endl;
			result.push_back(minPoint);
		}
	}
	return result;
}

// estimated the landmarks on list of points belong to the contours
vector<Point> verify_Descriptors_3(ptr_IntMatrix model, ptr_IntMatrix scene,
	vector<Point> contourPoints, vector<Point> manualLM, int patchSize)
{
	vector<Point> result;
	//int width = model->get_Cols();
	//int height = model->get_Rows();
	Point mpi(0, 0), epi(0, 0), minPoint(0, 0);

	for (int i = 0; i < manualLM.size(); i++)
	{
		mpi = manualLM.at(i);
		minPoint.reset();
		double minDistance = DBL_MAX;
		vector<double> mHistogram = SIFT_Descriptor(model, mpi, patchSize);
		for (int j = 0; j < contourPoints.size(); j++)
		{
			epi = contourPoints.at(j);
			vector<double> sHistogram = SIFT_Descriptor(scene, epi, patchSize);
			double distance = l2Distance(mHistogram, sHistogram);
			if (distance < minDistance)
			{
				minDistance = distance;
				minPoint.set_X(epi.get_X());
				minPoint.set_Y(epi.get_Y());
			}
		}
		result.push_back(minPoint);
	}
	return result;
}

double bhatScore(ptr_IntMatrix patch1, ptr_IntMatrix patch2)
{
	int rows1 = patch1->get_Rows();
	int cols1 = patch1->get_Cols();
	int rows2 = patch2->get_Rows();
	int cols2 = patch2->get_Cols();
	if (rows1 != rows2 || cols1 != cols2)
		return 0;
	double values1 = 0, values2 = 0;
	for (int r = 0; r < rows1; r++)
	{
		for (int c = 0; c < cols1; c++)
		{
			values1 += patch1->get_At_Position(r, c);
		}
	}
	for (int r = 0; r < rows2; r++)
	{
		for (int c = 0; c < cols2; c++)
		{
			values2 += patch2->get_At_Position(r, c);
		}
	}
	for (int r = 0; r < rows2; r++)
	{
		for (int c = 0; c < cols2; c++)
		{
			values2 += patch2->get_At_Position(r, c);
		}
	}
	double distance = 0;
	for (int r = 0; r < rows1; r++)
	{
		for (int c = 0; c < cols1; c++)
		{
			double value_1 = sqrt(patch1->get_At_Position(r, c) / values1);
			double value_2 = sqrt(patch2->get_At_Position(r, c) / values2);
			distance += value_1 * value_2;
		}
	}
	return distance;
}

ptr_IntMatrix extractPatchGray(ptr_IntMatrix grayImage, int pSize, Point center)
{
	if (pSize % 2 == 0)
		pSize += 1;
	int hSize = pSize / 2;
	ptr_IntMatrix patch = new Matrix<int>(pSize, pSize, 0);
	int rows = grayImage->get_Rows();
	int cols = grayImage->get_Cols();
	int rbelow = center.get_Y() - hSize;
	int rabow = center.get_Y() + hSize;
	int cleft = center.get_X() - hSize;
	int cright = center.get_X() + hSize;
	if (rbelow < 0 || rabow >= rows || cleft < 0 || cright >= cols)
		return patch;

	int i = 0, j = 0;
	for (int r = rbelow; r <= rabow; r++)
	{
		j = 0;
		for (int c = cleft; c <= cright; c++)
		{
			int value = grayImage->get_At_Position(r, c);
			patch->set_At_Position(i, j, value);
			j++;
		}
		i++;
	}
	return patch;
}

vector<Point> verify_Descriptors_4(ptr_IntMatrix model, ptr_IntMatrix scene,
	vector<Point> contourPoints, vector<Point> manualLM, int patchSize)
{
	vector<Point> result;
	//int width = model->get_Cols();
	//int height = model->get_Rows();
	Point mpi(0, 0), epi(0, 0), minPoint(0, 0);

	for (int i = 0; i < manualLM.size(); i++)
	{
		mpi = manualLM.at(i);
		minPoint.reset();
		double maxDistance = -1;
		ptr_IntMatrix mPatch = extractPatchGray(model, patchSize, mpi);

		for (int j = 0; j < contourPoints.size(); j++)
		{
			epi = contourPoints.at(j);
			ptr_IntMatrix sPatch = extractPatchGray(scene, patchSize, epi);

			double distance = bhatScore(mPatch, sPatch);
			if (distance > maxDistance)
			{
				maxDistance = distance;
				minPoint.set_X(epi.get_X());
				minPoint.set_Y(epi.get_Y());
			}
		}
		result.push_back(minPoint);
	}
	return result;
}

