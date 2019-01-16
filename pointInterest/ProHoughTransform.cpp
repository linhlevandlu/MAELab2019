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

#include "ProHoughTransform.h"

vector<Point> findLandmarks(Point refPoint, Point esPoint,
	vector<Point> refLandmarks, int width, int height, int &positive)
{
	vector<Point> esLandmarks;
	positive = 0;
	Point temp, lm;
	for (size_t t = 0; t < refLandmarks.size(); t++)
	{
		temp = refLandmarks.at(t);

		lm.set_X(temp.get_X());
		lm.set_Y(temp.get_Y());

		int px = refPoint.get_X() - esPoint.get_X();
		int py = refPoint.get_Y() - esPoint.get_Y();
		int x;
		int y;
		x = lm.get_X() - px;
		if (py > 0)
			y = lm.get_Y() - py;
		else
			y = lm.get_Y() + py;
		esLandmarks.push_back(Point(x, y));
		if (x >= 0 && x <= width && y >= 0 && y <= height)
		{
			positive += 1;
		}
	}
	return esLandmarks;
}

Point refPointInScene(PHTEntry entry, vector<Line> matchLines,
	double &angleDiff, vector<Point> refLandmarks, int width, int height)
{

	Point inter(0, 0);
	Point refPoint(width / 2, height / 2);

	if (matchLines.size() <= 0)
		return inter;
	Line objl1 = matchLines.at(0);
	Line objl2 = matchLines.at(1);

	Line lineEntry1 = entry.get_Ref_Line();
	Line lineEntry2 = entry.get_Obj_Line();

	HoughSpace hs1, hs2;
	hs1 = entry.get_List_Hough_Space().at(0);
	hs2 = entry.get_List_Hough_Space().at(1);

	// display the angle
	double angle3 = lineEntry1.angle_Lines(objl1);
	double angle4 = lineEntry1.angle_Lines(objl2);

	vector<Point> intersects1 = objl1.inter_Parallel(objl1, objl2, hs1.distance,
		hs2.distance, width, height);
	int max = 0;
	vector<Point> estLM;
	vector<double> angles;
	Point esPoint;
	vector<Point> lms;
	int positive = 0;
	for (size_t i = 0; i < intersects1.size(); i++)
	{
		esPoint = intersects1.at(i);
		positive = 0;
		lms = findLandmarks(refPoint, esPoint, refLandmarks, width, height,
			positive);
		if (positive > max)
		{
			estLM.clear();
			estLM.push_back(esPoint);
			angles.clear();
			angles.push_back(angle3);
			max = positive;
		}
		else
		{
			if (positive == max)
			{
				estLM.push_back(esPoint);
				angles.push_back(angle3);
			}
		}
	}

	vector<Point> intersects2 = objl1.inter_Parallel(objl1, objl2, hs2.distance,
		hs1.distance, width, height);

	for (size_t i = 0; i < intersects2.size(); i++)
	{
		esPoint = intersects2.at(i);
		positive = 0;
		lms = findLandmarks(refPoint, esPoint, refLandmarks, width, height,
			positive);
		if (positive > max)
		{
			estLM.clear();
			estLM.push_back(esPoint);
			angles.clear();
			angles.push_back(angle4);
			max = positive;
		}
		else
		{
			if (positive == max)
			{
				estLM.push_back(esPoint);
				angles.push_back(angle4);
			}
		}
	}

	intersects1.clear();
	intersects2.clear();

	if (estLM.size() == 1)
	{
		inter = estLM.at(0);
		angleDiff = angles.at(0);
	}
	else
	{
		double angleds = 180;
		Point es;
		for (size_t i = 0; i < estLM.size(); i++)
		{
			es = estLM.at(i);
			double angle = angles.at(i);
			if (angle <= angleds)
			{
				angleds = angle;
				angleDiff = angleds;
				inter = es;
			}
		}
	}
	cout << "\n Reference point in scene: " << inter.get_X() << ", "
		<< inter.get_Y();

	return inter;
}
bool similarPairLines(Line ref1, Line ref2, Line scene1, Line scene2)
{
	int cond1 = 1;
	int cond2 = 1;
	int cond3 = 2;

	double refAngle = ref1.angle_Lines(ref2);
	double rd1 = ref1.perpendicular_Distance(ref2.get_Begin());
	double rd2 = ref1.perpendicular_Distance(ref2.get_End());
	double rd = rd1 + rd2;

	double sceneAngle = scene1.angle_Lines(scene2);
	double sd1 = scene1.perpendicular_Distance(scene2.get_Begin());
	double sd2 = scene1.perpendicular_Distance(scene2.get_End());
	double sd = sd1 + sd2;

	if (abs(refAngle - sceneAngle) < cond1
		&& (abs(
			(ref1.get_Length() / scene1.get_Length())
				- (ref2.get_Length() / scene2.get_Length())) < cond2)
		&& (abs(rd - sd) < cond3))
	{
		return true;
	}
	return false;
}

PHTEntry findHoughSpace(vector<PHTEntry> entryTable, Line line1, Line line2)
{
	PHTEntry entry;
	Line ref1, ref2;
	for (size_t i = 0; i < entryTable.size(); i++)
	{
		ref1 = entryTable.at(i).get_Ref_Line();
		ref2 = entryTable.at(i).get_Obj_Line();
		if (similarPairLines(ref1, ref2, line1, line2))
		{
			entry = entryTable.at(i);
		}
	}
	return entry;
}

PHTEntry matchingInScene(vector<PHTEntry> entryTable, vector<Line> sceneLines,
	int width, int height, vector<Line> &maxVector)
{
	ptr_IntMatrix accumulator = new Matrix<int>(
		floor(sqrt(width * width + height * height)), 361);
	int maxValue = 0;
	PHTEntry maxEntry;
	Line objLine1;
	Line objLine2;
	PHTEntry entry;
	vector<HoughSpace> hspace;
	for (size_t i = 0; i < sceneLines.size(); i++)
	{
		objLine1 = sceneLines.at(i);
		for (size_t j = 0; j < sceneLines.size(); j++)
		{
			objLine2 = sceneLines.at(j);
			if (i != j && closetLine(objLine2, objLine1))
			{
				entry = findHoughSpace(entryTable, objLine1, objLine2);
				hspace = entry.get_List_Hough_Space();
				if (hspace.size() != 0)
				{
					HoughSpace hsp;
					for (size_t k = 0; k < hspace.size(); k++)
					{
						hsp = hspace.at(k);
						int angle = round(hsp.angle);
						int distance = round(hsp.distance);
						if (!isnan(angle) && !isnan(distance) && angle >= 0
							&& distance >= 0)
						{
							int value = accumulator->get_At_Position(distance, angle);
							accumulator->set_At_Position(distance, angle, value + 1);
							if (accumulator->get_At_Position(distance, angle) > maxValue)
							{
								maxVector.clear();
								maxVector.push_back(objLine1);
								maxVector.push_back(objLine2);
								maxValue = accumulator->get_At_Position(distance, angle);
								maxEntry.set_Ref_Line(entry.get_Ref_Line());
								maxEntry.set_Obj_Line(entry.get_Obj_Line());
								maxEntry.set_List_Hough_Space(entry.get_List_Hough_Space());
								//maxEntry= entry;
							}
							else
							{
								if (k == 0
									&& accumulator->get_At_Position(distance, angle) == maxValue)
								{
									maxVector.push_back(objLine1);
									maxVector.push_back(objLine2);
								}
							}
						}
					}
				}
			}
		}
	}

	delete accumulator;
	return maxEntry;
}
vector<Point> phtLandmarks(vector<PHTEntry> entriesTable, Point refPoint,
	vector<Line> sceneLines, int width, int height, vector<Point> mLandmarks,
	double &angleDiff, Point &ePoint)
{
	vector<Point> eLandmarks;
	vector<Line> maxVector;
	PHTEntry entry = matchingInScene(entriesTable, sceneLines, width, height,
		maxVector);
	if (maxVector.size() > 0)
	{

		ePoint = refPointInScene(entry, maxVector, angleDiff, mLandmarks, width,
			height);
		double angle1 = entry.get_Ref_Line().angle_Lines(entry.get_Obj_Line());
		double angle2 = maxVector.at(0).angle_Lines(maxVector.at(1));
		angleDiff += abs(angle1 - angle2);
		int positive = 0;
		eLandmarks = findLandmarks(refPoint, ePoint, mLandmarks, width, height,
			positive);
	}
	maxVector.clear();

	return eLandmarks;
}
ProHoughTransform::ProHoughTransform()
{
	// TODO Auto-generated constructor stub

}

ProHoughTransform::~ProHoughTransform()
{
	// TODO Auto-generated destructor stub
}
PHoughTransform ProHoughTransform::constructPHT()
{
	ptr_IntMatrix grayImage;
	grayImage = (Matrix<int> *) malloc(sizeof(Matrix<int> ));
	*grayImage = Treatments::refImage.get_Gray_Matrix();
	//grayImage = Treatments::refImage.get_Gray_Matrix();
	int width = grayImage->get_Cols();
	int height = grayImage->get_Rows();

	PHoughTransform pht;
	pht.set_Ref_Point(Point(width / 2, height / 2));
	pht.construct_PHT_Table(line_Segment(Treatments::refImage));

	delete grayImage;
	return pht;
}

vector<Point> ProHoughTransform::estimateLandmarks(Image sImage,
	double &angleDiff, Point &ePoint)
{
	vector<Point> eLandmarks;
	Matrix<int> gMatrix = Treatments::refImage.get_Gray_Matrix();
	ptr_IntMatrix mMatrix = &gMatrix;
	int width = mMatrix->get_Cols();
	int height = mMatrix->get_Rows();

	vector<Point> mLandmarks = Treatments::refImage.get_List_Of_Manual_Landmarks();
	vector<Line> mLines = line_Segment(Treatments::refImage);//Treatments::refImage.getListOfLines();
	vector<Line> sLines = line_Segment(sImage);//sImage.getListOfLines();

	Point mPoint(width / 2, height / 2);

	PHoughTransform pht;
	pht.set_Ref_Point(mPoint);
	vector<PHTEntry> entryTable = pht.construct_PHT_Table(mLines);

	eLandmarks = phtLandmarks(entryTable, mPoint, sLines, width, height,
		mLandmarks, angleDiff, ePoint);

	return eLandmarks;

}

void alignSegmentation(int minOx, int minOy, int maxOx, int maxOy, int addition,
	vector<Point> &newModelPoints, vector<Point> &newScenePoints,
	vector<Point> &eslm, Point &mPoint)
{
	Point mi;
	if (minOx != 0)
	{
		mPoint.set_X(mPoint.get_X() - minOx + addition);
		for (size_t i = 0; i < newScenePoints.size(); i++)
		{
			mi = newScenePoints.at(i);
			newScenePoints.at(i).set_X(mi.get_X() - minOx + addition);
		}
		for (size_t i = 0; i < eslm.size(); i++)
		{
			mi = eslm.at(i);
			eslm.at(i).set_X(mi.get_X() - minOx + addition);
		}
		for (size_t i = 0; i < newModelPoints.size(); i++)
		{
			mi = newModelPoints.at(i);
			newModelPoints.at(i).set_X(mi.get_X() - minOx + addition);
		}
	}
	if (maxOx != 0)
	{
		mPoint.set_X(mPoint.get_X() - maxOx - addition);
		for (size_t i = 0; i < newScenePoints.size(); i++)
		{
			mi = newScenePoints.at(i);
			newScenePoints.at(i).set_X(mi.get_X() - maxOx - addition);
		}
		for (size_t i = 0; i < eslm.size(); i++)
		{
			mi = eslm.at(i);
			eslm.at(i).set_X(mi.get_X() - maxOx - addition);
		}
		for (size_t i = 0; i < newModelPoints.size(); i++)
		{
			mi = newModelPoints.at(i);
			newModelPoints.at(i).set_X(mi.get_X() - maxOx - addition);
		}
	}
	if (minOy != 0)
	{
		mPoint.set_Y(mPoint.get_Y() - minOy + addition);
		for (size_t i = 0; i < newScenePoints.size(); i++)
		{
			mi = newScenePoints.at(i);
			newScenePoints.at(i).set_Y(mi.get_Y() - minOy + addition);
		}
		for (size_t i = 0; i < eslm.size(); i++)
		{
			mi = eslm.at(i);
			eslm.at(i).set_Y(mi.get_Y() - minOy + addition);
		}
		for (size_t i = 0; i < newModelPoints.size(); i++)
		{
			mi = newModelPoints.at(i);
			newModelPoints.at(i).set_Y(mi.get_Y() - minOy + addition);
		}
	}
	if (maxOy != 0)
	{
		mPoint.set_Y(mPoint.get_Y() - maxOy - addition);
		for (size_t i = 0; i < newScenePoints.size(); i++)
		{
			mi = newScenePoints.at(i);
			newScenePoints.at(i).set_Y(mi.get_Y() - maxOy - addition);
		}
		for (size_t i = 0; i < eslm.size(); i++)
		{
			mi = eslm.at(i);
			eslm.at(i).set_Y(mi.get_Y() - maxOy - addition);
		}
		for (size_t i = 0; i < newModelPoints.size(); i++)
		{
			mi = newModelPoints.at(i);
			newModelPoints.at(i).set_Y(mi.get_Y() - maxOy - addition);
		}
	}
}
void checkInSize(vector<Point> points, int rows, int cols, int &minOx,
	int &minOy, int &maxOx, int &maxOy)
{
	minOx = minOy = maxOx = maxOy = 0;
	Point pi;
	for (size_t i = 0; i < points.size(); i++)
	{
		pi = points.at(i);
		if (pi.get_X() < 0)
		{
			int temp = pi.get_X() - 0;
			if (temp < minOx)
				minOx = temp;
		}
		if (pi.get_X() >= cols)
		{
			int temp = pi.get_X() - cols;
			if (temp > maxOx)
				maxOx = temp;
		}
		if (pi.get_Y() < 0)
		{
			int temp = pi.get_Y() - 0;
			if (temp < minOy)
				minOy = temp;
		}
		if (pi.get_Y() >= rows)
		{
			int temp = pi.get_Y() - rows;
			if (temp > maxOy)
				maxOy = temp;
		}
	}
}
/*struct comparey
{
	bool operator()(Point p1, Point p2)
	{
		return p1.get_Y() < p2.get_Y();
	}
} yDirection;*/
vector<Point> ProHoughTransform::generalTransform(Image &sImage, double &angle,
	Point &ePoint, Point &mPoint, ptr_IntMatrix &newSceneGray, Point &translation,
	vector<Point> &modelSeg, vector<Point> &sceneSeg,
	vector<Point> &newScenePoints, double &scaleX, double &scaleY,
	Point &moveScale)
{
	int rows = sImage.get_Gray_Matrix().get_Rows();
	int cols = sImage.get_Gray_Matrix().get_Cols();
	Image mImage = Treatments::refImage;
	ptr_IntMatrix mgradirection = new Matrix<int>(rows, cols, -1);
	vector<Point> modelPoints;
	*mgradirection = *(get_Gradient_DMatrix(mImage, modelPoints));
	vector<Point> mLandmarks = mImage.get_List_Of_Manual_Landmarks();

	ptr_IntMatrix gradirection = new Matrix<int>(rows, cols, -1);
	vector<Point> scenePoints;
	*gradirection = *(get_Gradient_DMatrix(sImage, scenePoints));

	vector<Point> eslm;
	translation.set_X(0);
	translation.set_Y(0);
	Point mtranslation;

	/*
	 * ePoint: centroid's location of original scene
	 * mPoint: centroid's location of original model in the scene
	 * angle: different angle between scene and model (include direction)
	 * translation: translating from scene to model in scene
	 * mtranslation: translating of original model and model in scene
	 */

	eslm = generalizing_Hough_Transform(mgradirection, gradirection, mLandmarks,
		ePoint, mPoint, angle, translation, mtranslation);
	int dx = translation.get_X();
	int dy = translation.get_Y();
	// rotation the scene points
	Point mi;
	newScenePoints.clear();

	for (size_t i = 0; i < scenePoints.size(); i++)
	{
		mi = scenePoints.at(i);
		int xnew = mi.get_X() + dx;
		int ynew = mi.get_Y() + dy;
		rotate_A_Point(xnew, ynew, mPoint, angle, 1, xnew, ynew);
		newScenePoints.push_back(Point(xnew, ynew));
	}
	// new location of model points
	vector<Point> newModelPoints;
	Point nmi;

	for (size_t i = 0; i < modelPoints.size(); i++)
	{
		nmi = modelPoints.at(i);
		int x = nmi.get_X() + mtranslation.get_X();
		int y = nmi.get_Y() + mtranslation.get_Y();
		newModelPoints.push_back(Point(x, y));
	}
	// move new scene and new model into the image
	Point osPoint, nsPoint;
	vector<Point> newScene = PCAI_Points(newModelPoints, mPoint, newScenePoints,
		abs(angle));
	newScenePoints = newScene;
	int minOx = 0, minOy = 0, maxOx = 0, maxOy = 0;
	int mminOx = 0, mminOy = 0, mmaxOx = 0, mmaxOy = 0;
	checkInSize(newScenePoints, rows, cols, minOx, minOy, maxOx, maxOy);
	alignSegmentation(minOx, minOy, maxOx, maxOy, 30, newModelPoints,
		newScenePoints, eslm, mPoint);
	checkInSize(newModelPoints, rows, cols, mminOx, mminOy, mmaxOx, mmaxOy);
	alignSegmentation(mminOx, mminOy, mmaxOx, mmaxOy, 30, newModelPoints,
		newScenePoints, eslm, mPoint);

	Line olineScene = principal_Axis(scenePoints, osPoint);
	Line nlineScene = principal_Axis(newScenePoints, nsPoint);
	double sangle = nlineScene.angle_Lines(olineScene);
	Point smove = nsPoint - osPoint;
	olineScene.set_Begin(nlineScene.get_Begin());
	Point eoScene = olineScene.get_End();
	eoScene.set_X(eoScene.get_X() + smove.get_X());
	eoScene.set_Y(eoScene.get_Y() + smove.get_Y());
	olineScene.set_End(eoScene);
	sangle = rotate_Direction(nlineScene, olineScene, sangle);
// move the scene gray to the same and rotate the model
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int value = sImage.get_Gray_Matrix().get_At_Position(r, c);
			int xnew = c + smove.get_X();
			int ynew = r + smove.get_Y();
			rotate_A_Point(xnew, ynew, mPoint, sangle, 1, xnew, ynew);
			if (xnew >= 0 && ynew >= 0 && ynew < rows && xnew < cols)
			{
				newSceneGray->set_At_Position(ynew, xnew, value);
			}
		}
	}
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			int t1 = 0, t2 = 0, t3 = 0, t4 = 0;
			int t = newSceneGray->get_At_Position(r, c);
			if (r - 1 >= 0 && r - 1 < rows)
			{
				t1 = newSceneGray->get_At_Position(r - 1, c);
			}
			if (r + 1 >= 0 && r + 1 < rows)
			{
				t2 = newSceneGray->get_At_Position(r + 1, c);
			}
			if (c - 1 >= 0 && c - 1 < cols)
			{
				t3 = newSceneGray->get_At_Position(r, c - 1);
			}
			if (c + 1 >= 0 && c + 1 < cols)
			{
				t4 = newSceneGray->get_At_Position(r, c + 1);
			}
			if (t == 0)
			{
				newSceneGray->set_At_Position(r, c, (t1 + t2 + t3 + t4) / 4);
			}
		}
	}
	//saveGrayScale("abc.jpg", newSceneGray);
// export two images of segmentation
	modelSeg = modelPoints;
	sceneSeg = scenePoints;

	RGB color(255,0,0);
	// draw mot so diem
	Point sLeft(cols, rows), sRight(0, 0);
	for (size_t i = 0; i < newScenePoints.size(); i++)
	{
		mi = newScenePoints.at(i);
		int x = mi.get_X();
		int y = mi.get_Y();
		if (x >= 0 && y >= 0 && y < rows && x < cols)
		{
			//sImage.get_RGB_Matrix()->set_At_Position(y, x, color);
			if (x < sLeft.get_X())
				sLeft.set_X(x);
			if (y < sLeft.get_Y())
				sLeft.set_Y(y);
			if (x > sRight.get_X())
				sRight.set_X(x);
			if (y > sRight.get_Y())
				sRight.set_Y(y);
		}
	}
	//draw the bounding box
	/*drawingLine(*sImage.get_RGB_Matrix(),
	 Line(sLeft, Point(sRight.get_X(), sLeft.get_Y())), color);
	 drawingLine(*sImage.get_RGB_Matrix(),
	 Line(sLeft, Point(sLeft.get_X(), sRight.get_Y())), color);
	 drawingLine(*sImage.get_RGB_Matrix(),
	 Line(sRight, Point(sLeft.get_X(), sRight.get_Y())), color);
	 drawingLine(*sImage.get_RGB_Matrix(),
	 Line(sRight, Point(sRight.get_X(), sLeft.get_Y())), color);*/
	Point sceneCenter;
	sceneCenter.set_X((sLeft.get_X() + sRight.get_X()) / 2);
	sceneCenter.set_Y((sLeft.get_Y() + sRight.get_Y()) / 2);
	fill_Circle(sImage.get_RGB_Matrix(), sceneCenter, 5, color);
	color.set_G(0);
	Point mLeft, mRight;
	mLeft.set_X(cols);
	mLeft.set_Y(rows);
	mRight.set_X(0);
	mRight.set_Y(0);
	for (size_t k = 0; k < newModelPoints.size(); k++)
	{
		mi = newModelPoints.at(k);
		int x = mi.get_X();
		int y = mi.get_Y();
		if (x >= 0 && y >= 0 && y < rows && x < cols)
		{
			sImage.get_RGB_Matrix().set_At_Position(y, x, color);
			if (x < mLeft.get_X())
				mLeft.set_X(x);
			if (y < mLeft.get_Y())
				mLeft.set_Y(y);
			if (x > mRight.get_X())
				mRight.set_X(x);
			if (y > mRight.get_Y())
				mRight.set_Y(y);
		}
	}
	Point modelCenter;
	modelCenter.set_X((mLeft.get_X() + mRight.get_X()) / 2);
	modelCenter.set_Y((mLeft.get_Y() + mRight.get_Y()) / 2);
	fill_Circle(sImage.get_RGB_Matrix(), modelCenter, 5, color);
	// draw bounding box
	/*drawingLine(*sImage.get_RGB_Matrix(),
	 Line(mLeft, Point(mRight.get_X(), mLeft.get_Y())), color);
	 drawingLine(*sImage.get_RGB_Matrix(),
	 Line(mLeft, Point(mLeft.get_X(), mRight.get_Y())), color);
	 drawingLine(*sImage.get_RGB_Matrix(),
	 Line(mRight, Point(mLeft.get_X(), mRight.get_Y())), color);
	 drawingLine(*sImage.get_RGB_Matrix(),
	 Line(mRight, Point(mRight.get_X(), mLeft.get_Y())), color);*/

	Line mX(mLeft, Point(mRight.get_X(), mLeft.get_Y()));
	Line mY(mLeft, Point(mLeft.get_X(), mRight.get_Y()));
	Line sX(sLeft, Point(sRight.get_X(), sLeft.get_Y()));
	Line sY(sLeft, Point(sLeft.get_X(), sRight.get_Y()));
	scaleX = mX.get_Length() / sX.get_Length();
	scaleY = mY.get_Length() / sY.get_Length();
	color.set_G(255);
	for (size_t i = 0; i < newScenePoints.size(); i++)
	{
		mi = newScenePoints.at(i);
		int x = round(mi.get_X() * scaleX);
		int y = round(mi.get_Y() * scaleY);
		newScenePoints.at(i).set_X(x);
		newScenePoints.at(i).set_Y(y);
		/*if (x >= 0 && y >= 0 && y < rows && x < cols)
		 {
		 sImage.get_RGB_Matrix()->set_At_Position(y, x, color);
		 }*/
	}

	std::sort(newModelPoints.begin(), newModelPoints.end(), yComparation);
	std::sort(newScenePoints.begin(), newScenePoints.end(), yComparation);
	Point diffSort = newModelPoints.at(0) - newScenePoints.at(0);
	moveScale.set_X(diffSort.get_X());
	moveScale.set_Y(diffSort.get_Y());
	for (size_t i = 0; i < newScenePoints.size(); i++)
	{
		mi = newScenePoints.at(i);
		int x = mi.get_X() + diffSort.get_X();
		int y = mi.get_Y() + diffSort.get_Y();
		newScenePoints.at(i).set_X(x);
		newScenePoints.at(i).set_Y(y);
		if (x >= 0 && y >= 0 && y < rows && x < cols)
		{
			sImage.get_RGB_Matrix().set_At_Position(y, x, color);
		}
	}
	Point pi;
	for (size_t k = 0; k < eslm.size(); k++)
	{
		pi = eslm.at(k);
		Point ci = nearest_Point(newScenePoints, pi);
		eslm.at(k).set_X(ci.get_X());
		eslm.at(k).set_Y(ci.get_Y());
		fill_Circle(sImage.get_RGB_Matrix(), ci, 5, color);
	}
	Matrix<RGB> savergb = sImage.get_RGB_Matrix();
	save_RGB("color.jpg", &savergb);
// ================================================================================
	angle = -sangle;
	translation.set_X(-smove.get_X());
	translation.set_Y(-smove.get_Y());
	ePoint = nsPoint;
	return eslm;
}
