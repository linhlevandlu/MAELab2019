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

#include "PCA.h"
double avgDistance(vector<Point> listPoints, Line axis)
{
	double totalDist = 0;
	size_t nPoints = listPoints.size();
	for (size_t j = 0; j < nPoints; j++)
	{
		Point pj = listPoints.at(j);
		double distance = axis.perpendicular_Distance(pj);
		totalDist += distance;
	}
	return totalDist / (int) nPoints;
}

// compute centroid point from the gradient matrix of image
// output: centroid point and list points of edge.
Point centroidPoint(ptr_IntMatrix gradMatrix, vector<Point> &listPoints)
{
	int rows = gradMatrix->get_Rows();
	int cols = gradMatrix->get_Cols();
	int totalX = 0, totalY = 0, count = 0;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (gradMatrix->get_At_Position(r, c) != -1)
			{
				listPoints.push_back(Point(c, r));
				totalX += c;
				totalY += r;
				count++;
			}
		}
	}
	return Point(totalX / count, totalY / count);
}

Line principal_Axis(vector<Point> listOfPoints, Point &cPoint)
{
	double minAvgDist = DBL_MAX;
	Point sPoint;
	size_t nPoints = listOfPoints.size();
	int totalX = 0, totalY = 0;
	Point pi;
	for (size_t i = 0; i < nPoints; i++)
	{
		pi = listOfPoints.at(i);
		totalX += pi.get_X();
		totalY += pi.get_Y();
	}
	cPoint.set_X(totalX / (int) nPoints);
	cPoint.set_Y(totalY / (int) nPoints);
	pi.set_X(0);
	pi.set_Y(0);
	for (size_t i = 0; i < nPoints; i++)
	{
		pi = listOfPoints.at(i);
		Line l(cPoint, pi);
		double avgDist = avgDistance(listOfPoints, l);
		if (avgDist < minAvgDist)
		{
			minAvgDist = avgDist;
			sPoint.set_X(pi.get_X());
			sPoint.set_Y(pi.get_Y());
		}
	}

	return Line(cPoint, sPoint);
}
Line principal_Axis(ptr_IntMatrix gradMatrix, Point &cPoint)
{
	vector<Point> listOfPoints;
	cPoint = centroidPoint(gradMatrix, listOfPoints);
	return principal_Axis(listOfPoints, cPoint);

}
// negative for clockwise, positive for counterclockwise
// two lines have the same origin
double rotate_Direction(Line refLine, Line objLine, double angle)
{
	int xnew = 0, ynew = 0;
	Point origin = refLine.get_Begin();
	Point rpoint = objLine.get_End();
	rotate_A_Point(rpoint.get_X(), rpoint.get_Y(), origin, angle, 1, xnew, ynew);
	double distance1 = refLine.perpendicular_Distance(Point(xnew, ynew));
	xnew = ynew = 0;
	rotate_A_Point(rpoint.get_X(), rpoint.get_Y(), origin, -angle, 1, xnew, ynew);
	double distance2 = refLine.perpendicular_Distance(Point(xnew, ynew));
	if (distance2 < distance1)
		angle = -angle;
	return angle;
}
// estimated by using PCAI
/*
struct comparey
{
	bool operator()(Point p1, Point p2)
	{
		return p1.get_Y() < p2.get_Y();
	}
} yComparation;
struct comparex
{
	bool operator()(Point p1, Point p2)
	{
		return p1.get_X() < p2.get_X();
	}
} xComparation;
*/
vector<Point> PCAI_Points(vector<Point> modelPoints, Point mPoint,
	vector<Point> scenePoints, double angle)
{

	size_t nlimit = modelPoints.size() / 2;
	std::sort(modelPoints.begin(), modelPoints.end(), yComparation);
	vector<Point> newModel(modelPoints.begin(), modelPoints.begin() + nlimit);
	vector<Point> sceneTemp(scenePoints); // copy the points of scene
	int index = 0;
	double minAngle = 360;
	cout << "\nFirst angle: " << angle << endl;
	while (angle > 1.5)
	{
		index++;
		std::sort(sceneTemp.begin(), sceneTemp.end(), yComparation);
		vector<Point> newScene(sceneTemp.begin(), sceneTemp.begin() + nlimit);
		Point cPoint1, cPoint2;
		Line l1 = principal_Axis(newModel, cPoint1);
		Line l2 = principal_Axis(newScene, cPoint2);
		angle = l1.angle_Lines(l2);
		if (index > 20 || angle < 1.5)
		{
			if (angle < minAngle)
				minAngle = angle;
		}
		Point diff = cPoint1 - cPoint2;
		l2.set_Begin(l1.get_Begin());
		l2.set_End(
			Point(l2.get_End().get_X() + diff.get_X(),
				l2.get_End().get_Y() + diff.get_Y()));
		double angleR2 = rotate_Direction(l1, l2, l1.angle_Lines(l2));

		Point psn;
		for (size_t i = 0; i < sceneTemp.size(); i++)
		{
			psn = sceneTemp.at(i);
			int xnew = psn.get_X() + diff.get_X(), ynew = psn.get_Y() + diff.get_Y();
			rotate_A_Point(xnew, ynew, mPoint, angleR2, 1, xnew, ynew);
			sceneTemp.at(i).set_X(xnew);
			sceneTemp.at(i).set_Y(ynew);
		}
		if (index > 30)
		{
			if ((int) angle == (int) minAngle)
			{
				break;
			}
		}
	}
	cout << "\nMin angle: " << minAngle << endl;
	if (minAngle > 10)
	{
		sceneTemp = scenePoints;
	}
	return sceneTemp;
}

Point checkOneDirection(vector<Point> contours, Point pi, Point closest)
{
	int mIndex = -1;
	Point p(0, 0);
	for (int k = 0; k < contours.size(); k++)
	{
		if (closest == contours.at(k))
		{
			mIndex = k;
			break;
		}
	}
	if (mIndex != -1)
	{

		Point pyNext(0, 0), pyPrev(0, 0);

		Point p1(0, 0), p2(0, 0);
		if (mIndex < contours.size() - 1)
		{
			pyNext = contours.at(mIndex + 1);
			// check line 1
			vector<Point> pOnLine1 = detect_Line(Line(pyNext, closest));
			bool onLine = check_Point_In_List(pOnLine1, pi);
			if (onLine)
			{
				p1 = pi;
			}
			else
			{
				Point pi1 = nearest_Point(pOnLine1, pi);
				p1 = pi1;
			}
		}
		if (mIndex != 0)
		{
			pyPrev = contours.at(mIndex - 1);
			// check line 2
			vector<Point> pOnLine2 = detect_Line(Line(pyPrev, closest));
			bool onLine2 = check_Point_In_List(pOnLine2, pi);
			if (onLine2)
			{
				p2 = pi;
			}
			else
			{
				Point pi2 = nearest_Point(pOnLine2, pi);
				p2 = pi2;
			}
		}
		Line l1(pi, p1);
		Line l2(pi, p2);
		if (l1.get_Length() <= l2.get_Length())
			p = p1;
		else
			p = p2;
	}
	return p;
}
vector<Point> refine(vector<Point> contours, vector<Point> estlm)
{
	vector<Point> results;
	for (size_t i = 0; i < estlm.size(); i++)
	{
		Point pi = estlm.at(i);
		bool onCurve = check_Point_In_List(contours, pi);
		if (!onCurve)
		{
			Point mPoint = nearest_Point(contours, pi);
			std::sort(contours.begin(), contours.end(), yComparation);
			Point py = checkOneDirection(contours, pi, mPoint);

			std::sort(contours.begin(), contours.end(), yComparation);
			Point px = checkOneDirection(contours, pi, mPoint);

			Line lx(pi, px);
			Line ly(pi, py);
			if (lx.get_Length() <= ly.get_Length())
				results.push_back(px);
			else
				results.push_back(py);
		}
		else
		{
			results.push_back(pi);
		}
	}
	return results;
}
vector<Point> PCAI(vector<Point> modelPoints, Image &sceneGray,
	vector<Point> mnLandmarks)
{
	int rows = sceneGray.get_Gray_Matrix().get_Rows();
	int cols = sceneGray.get_Gray_Matrix().get_Cols();
	ptr_IntMatrix lastModel = new Matrix<int>(rows, cols, 0);
	ptr_IntMatrix lastScene = new Matrix<int>(rows, cols, 0);

	Point mPoint;
	Line mAxis = principal_Axis(modelPoints, mPoint);

	vector<Point> scenePoints;
	ptr_IntMatrix sceneGrandient = new Matrix<int>(rows, cols, -1);
	*sceneGrandient = *(get_Gradient_DMatrix(sceneGray, scenePoints));
	vector<Point> scenePoints2(scenePoints); // keep the original points of the scene
	Point sPoint;
	Line sAxis = principal_Axis(sceneGrandient, sPoint);
	Point send = sAxis.get_End();
	double angle = mAxis.angle_Lines(sAxis);

	// difference between two centroids
	int dx = mPoint.get_X() - sPoint.get_X();
	int dy = mPoint.get_Y() - sPoint.get_Y();

	// move the scene to the model
	sAxis.set_Begin(mPoint);
	send.set_X(send.get_X() + dx);
	send.set_Y(send.get_Y() + dy);
	sAxis.set_End(send);
	Point pi;
	// Detecting the rotated direction
	double angleR = rotate_Direction(mAxis, sAxis, angle);

	RGB color;
	// rotate and translate the scene to match with the model
	
	for (size_t i = 0; i < scenePoints.size(); i++)
	{
		pi = scenePoints.at(i);
		int xnew = 0, ynew = 0;
		rotate_A_Point(pi.get_X() + dx, pi.get_Y() + dy, mPoint, angleR, 1, xnew, ynew);
		scenePoints.at(i).set_X(xnew);
		scenePoints.at(i).set_Y(ynew);
	}
	
	color.set_B(0);
	// hien thu ket qua lan thu nhat cua model
	for (size_t i = 0; i < modelPoints.size(); i++)
	{
		pi = modelPoints.at(i);
		if (pi.get_X() >= 0 && pi.get_X() < cols && pi.get_Y() >= 0
			&& pi.get_Y() < rows)
		{
			lastModel->set_At_Position(pi.get_Y(), pi.get_X(), 255);
		}
	}
	vector<Point> sceneTemp;
	sceneTemp = PCAI_Points(modelPoints, mPoint, scenePoints, angle);
	// display scene points
	color.set_R(0);
	color.set_B(255);
	for (size_t i = 0; i < sceneTemp.size(); i++)
	{
		pi = sceneTemp.at(i);
		if (pi.get_X() >= 0 && pi.get_X() < cols && pi.get_Y() >= 0
			&& pi.get_Y() < rows)
		{
			lastScene->set_At_Position(pi.get_Y(), pi.get_X(), 255);
		}
	}


// indicate the bounding box of the edges
	Point mLeft, mRight;
	std::sort(modelPoints.begin(), modelPoints.end(), xComparation);
	mLeft.set_X(modelPoints.at(0).get_X());
	mRight.set_X(modelPoints.at(modelPoints.size() - 1).get_X());
	std::sort(modelPoints.begin(), modelPoints.end(), yComparation);
	mLeft.set_Y(modelPoints.at(0).get_Y());
	mRight.set_Y(modelPoints.at(modelPoints.size() - 1).get_Y());

	Point sLeft, sRight;
	std::sort(sceneTemp.begin(), sceneTemp.end(), xComparation);
	sLeft.set_X(sceneTemp.at(0).get_X());
	sRight.set_X(sceneTemp.at(sceneTemp.size() - 1).get_X());
	std::sort(sceneTemp.begin(), sceneTemp.end(), yComparation);
	sLeft.set_Y(sceneTemp.at(0).get_Y());
	sRight.set_Y(sceneTemp.at(sceneTemp.size() - 1).get_Y());
	Line mX(mLeft, Point(mRight.get_X(), mLeft.get_Y()));
	Line mY(mLeft, Point(mLeft.get_X(), mRight.get_Y()));
	Line sX(sLeft, Point(sRight.get_X(), sLeft.get_Y()));
	Line sY(sLeft, Point(sLeft.get_X(), sRight.get_Y()));
	double scaleX = mX.get_Length() / sX.get_Length();
	double scaleY = mY.get_Length() / sY.get_Length();
	vector<Point> scenePointbk = sceneTemp;
	for (size_t i = 0; i < sceneTemp.size(); i++)
	{
		Point mi = sceneTemp.at(i);
		int x = round(mi.get_X() * scaleX);
		int y = round(mi.get_Y() * scaleY);
		sceneTemp.at(i).set_X(x);
		sceneTemp.at(i).set_Y(y);
		/*if (x >= 0 && y >= 0 && y < rows && x < cols)
		 {
		 sceneGray.getRGBMatrix()->set_At_Position(y, x, color);
		 }*/
	}
	std::sort(modelPoints.begin(), modelPoints.end(), yComparation);
	std::sort(sceneTemp.begin(), sceneTemp.end(), yComparation);
	Point diffSort = modelPoints.at(0) - sceneTemp.at(0);
	color.set_R(0);
	for (size_t i = 0; i < sceneTemp.size(); i++)
	{
		Point mi = sceneTemp.at(i);
		int x = mi.get_X() + diffSort.get_X();
		int y = mi.get_Y() + diffSort.get_Y();
		sceneTemp.at(i).set_X(x);
		sceneTemp.at(i).set_Y(y);
		/*if (x >= 0 && y >= 0 && y < rows && x < cols)
		 {
		 sceneGray.getRGBMatrix()->set_At_Position(y, x, color);
		 }*/
	}
	vector<Point> eslm;
	for (size_t k = 0; k < mnLandmarks.size(); k++)
	{
		Point pi = mnLandmarks.at(k);
		Point ci = nearest_Point(sceneTemp, pi);
		eslm.push_back(ci);
		//fillCircle(*(sceneGray.getRGBMatrix()), ci, 3, color);
	}
// end indicate the bounding box of the edges
	vector<Point> result;
	if (mnLandmarks.size() > 0)
	{
		//result = verifyLandmarks2(lastModel, lastScene, mnLandmarks, mnLandmarks,
		// 100, 300);
		result = verify_Descriptors_2(lastModel, lastScene, sceneTemp, mnLandmarks,
			eslm, 9, 36);
	}

	// compute the difference between original scene and rotated scene
	// to move-back the landmarks
	Point lsPoint1, lsPoint2;
	Line sl1 = principal_Axis(scenePoints2, lsPoint1);
	Line sl2 = principal_Axis(scenePointbk, lsPoint2);
	Point mback = lsPoint1 - lsPoint2;
	double mangle = sl1.angle_Lines(sl2);
	sl2.set_Begin(sl1.get_Begin());
	sl2.set_End(
		Point(sl2.get_End().get_X() + mback.get_X(),
			sl2.get_End().get_Y() + mback.get_Y()));
	mangle = rotate_Direction(sl1, sl2, mangle);
	for (size_t i = 0; i < result.size(); i++)
	{
		pi = result.at(i);
		pi.set_X((pi.get_X() - diffSort.get_X()) / scaleX);
		pi.set_Y((pi.get_Y() - diffSort.get_Y()) / scaleY);
		int xnew = pi.get_X() + mback.get_X(), ynew = pi.get_Y() + mback.get_Y();
		rotate_A_Point(xnew, ynew, lsPoint1, mangle, 1, xnew, ynew);
		pi.set_X(xnew);
		pi.set_Y(ynew);
		result.at(i).set_X(xnew);
		result.at(i).set_Y(ynew);
	}

	result = refine(scenePoints2, result);
	//delete sceneGrandient;
	return result;
}
vector<Point> PCAI(Image modelGray, Image sceneGray, vector<Point> mnLandmarks)
{
	// PCA
	int rows = modelGray.get_Gray_Matrix().get_Rows();
	int cols = modelGray.get_Gray_Matrix().get_Cols();
	vector<Point> modelPoints;
	ptr_IntMatrix modelGrandient = new Matrix<int>(rows, cols, -1);
	*modelGrandient = *(get_Gradient_DMatrix(modelGray, modelPoints));

	vector<Point> result = PCAI(modelPoints, sceneGray, mnLandmarks);

	delete modelGrandient;
	//delete sceneGrandient;
	return result;
}
