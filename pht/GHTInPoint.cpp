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

#include "GHTInPoint.h"

ptr_IntMatrix get_Gradient_DMatrix(Image grayImage, vector<Point> &edgePoints) {

	int rows = grayImage.get_Gray_Matrix().get_Rows();
	int cols = grayImage.get_Gray_Matrix().get_Cols();
	ptr_IntMatrix mMatrix = new Matrix<int>(rows, cols, 0);
	*mMatrix = grayImage.get_Gray_Matrix();
	int mThresholdValue = (int) grayImage.get_Threshold_Value();
	ptr_IntMatrix mbinMatrix = new Matrix<int>(
			*binary_Threshold(mMatrix, mThresholdValue, 255));
	ptr_IntMatrix mgradirection = new Matrix<int>(rows, cols, -1);
	ptr_IntMatrix mcannyMatrix = new Matrix<int>(
			*cannyProcess2(mbinMatrix, mThresholdValue, 3 * mThresholdValue,
					mgradirection, edgePoints));
	//delete mMatrix;
	//delete mbinMatrix;
	//delete mcannyMatrix;
	return mgradirection;
}
double angle_Vector(Point p1, Point p2, Point q1, Point q2) {
	Point vp(p2.get_X() - p1.get_X(), p2.get_Y() - p1.get_Y());
	Point vq(q2.get_X() - q1.get_X(), q2.get_Y() - q1.get_Y());

	double lengthVP = sqrt(
			(double) (vp.get_X() * vp.get_X())
					+ (double) (vp.get_Y() * vp.get_Y()));
	double lengthVQ = sqrt(
			(double) (vq.get_X() * vq.get_X())
					+ (double) (vq.get_Y() * vq.get_Y()));

	double vpq = (double) (vp.get_X() * vq.get_X())
			+ (double) (vp.get_Y() * vq.get_Y());

	return acos(vpq / (lengthVP * lengthVQ));
}
RTable rTable_Construct(ptr_IntMatrix gradMatrix, Point center) {
	RTable rtable;
	rtable.center.set_X(center.get_X());
	rtable.center.set_Y(center.get_Y());
	rtable.entriesTable.resize(361);

	// in default, rtable includes 360 rows for 360 degrees of gradient directions.
	int rows = gradMatrix->get_Rows();
	int cols = gradMatrix->get_Cols();

	int gd;
	REntry rentry;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			gd = gradMatrix->get_At_Position(r, c);
			if (gd != -1) {
				PolarValue pv;
				pv.p.set_X(c);
				pv.p.set_Y(r);
				Point hPoint(pv.p.get_X() + 100, pv.p.get_Y());
				pv.angle = angle_Vector(pv.p, hPoint, pv.p, rtable.center);
				pv.distance = (Line(rtable.center, pv.p)).get_Length();
				rentry = rtable.entriesTable.at((gd));

				(rtable.entriesTable.at((gd))).gradient = gd;
				(rtable.entriesTable.at((gd))).polarValues.push_back(pv);
			}
		}
	}
	return rtable;
}

Point hough_Space(ptr_IntMatrix gradMatrix, RTable rentries) {
	int rows = gradMatrix->get_Rows();
	int cols = gradMatrix->get_Cols();
	ptr_IntMatrix acc = new Matrix<int>(rows, cols, 0);

	int gradient = 0;
	REntry entry;
	int maxValue = 0, maxXIndex = 0, maxYIndex = 0;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			gradient = gradMatrix->get_At_Position(r, c);
			if (gradient != -1) {
				entry = rentries.entriesTable.at(gradient);
				if (entry.gradient == gradient) {
					vector<PolarValue> polarValues = entry.polarValues;
					if (polarValues.size() > 0) {
						//PolarValue pvalue;
						double avalue = 0, dvalue = 0;
						int xindex = 0, yindex = 0, accValue = 0;
						for (size_t k = 0; k < polarValues.size(); k++) {
							PolarValue pvalue = polarValues.at(k);
							avalue = pvalue.angle;
							dvalue = pvalue.distance;

							xindex = c + (dvalue * cos(avalue));
							yindex = r + (dvalue * sin(avalue));
							//cout << "\n" << xindex << "\t" << yindex;
							if (xindex >= 0 && yindex >= 0 && xindex < cols
									&& yindex < rows) {
								// increase the value in accumulator
								accValue = acc->get_At_Position(yindex, xindex);
								accValue += 1;
								acc->set_At_Position(yindex, xindex, accValue);
							}
							if (accValue > maxValue && xindex > 0
									&& xindex < cols && yindex > 0
									&& yindex < rows) {
								maxValue = accValue;
								maxXIndex = xindex;
								maxYIndex = yindex;
							}
						}
					}
				}
			}
		}
	}
	cout << "\n center: ";
	//rentries.center.toString();
	cout << "\nMax Value:" << maxValue;
	cout << "\nMax X index:" << maxXIndex;
	cout << "\nMax Y index:" << maxYIndex << endl;
	return Point(maxXIndex, maxYIndex);
}

vector<Point> detect_Landmarks(Point refPoint, Point ePoint,
		vector<Point> mlandmarks) {
	vector<Point> esLandmarks;
	Point mlm;
	int deltaX = 0, deltaY = 0, xn = 0, yn = 0;
	for (size_t i = 0; i < mlandmarks.size(); i++) {
		mlm = mlandmarks.at(i);
		deltaX = ePoint.get_X() - refPoint.get_X();
		deltaY = ePoint.get_Y() - refPoint.get_Y();

		xn = mlm.get_X() + deltaX;
		yn = mlm.get_Y() + deltaY;
		esLandmarks.push_back(Point(xn, yn));
	}
	return esLandmarks;
}

/*
 * ePoint: centroid's location of original scene
 * mPoint: centroid's location of original model in the scene
 * angle: different angle between scene and model (include direction)
 * translation: translating from scene to model in scene
 * mtranslation: translating of original model and model in scene
 */
vector<Point> ght_With_Entries(RTable rEntries, Line mLine, Point mcPoint,
		ptr_IntMatrix sGradient, vector<Point> mLandmarks, Point &ePoint,
		Point &mPoint, double &angle, Point &translation, Point &mtranslation) {

	Point sPoint = hough_Space(sGradient, rEntries);
	vector<Point> eslm = detect_Landmarks(mcPoint, sPoint, mLandmarks);
	Line sLine = principal_Axis(sGradient, ePoint);

	// compute angle and rotation direction
	double anglek = mLine.angle_Lines(sLine); // compute angle difference between two images
	cout << "\nAngle k: " << anglek << endl;
	int dx1 = mcPoint.get_X() - ePoint.get_X();
	int dy1 = mcPoint.get_Y() - ePoint.get_Y();

	Point mBegin = mLine.get_Begin();
	sLine.set_Begin(mBegin);
	Point sEnd = sLine.get_End();
	sEnd.set_X(sEnd.get_X() + dx1);
	sEnd.set_Y(sEnd.get_Y() + dy1);
	sLine.set_End(sEnd);
	// set new location of mLine
	angle = rotate_Direction(mLine, sLine, anglek);

	// translate of model
	int dxold = sPoint.get_X() - mcPoint.get_X();
	int dyold = sPoint.get_Y() - mcPoint.get_Y();
	mtranslation.set_X(dxold);
	mtranslation.set_Y(dyold);


	// detect presence of model in scene
	mcPoint = sPoint;
	int dx = mcPoint.get_X() - ePoint.get_X();
	int dy = mcPoint.get_Y() - ePoint.get_Y();
	cout << "\nTranslate: " << dx << "\t" << dy << endl;

	cout << "\nAngle rotation: " << angle << endl;
	cout << "\nLandmarks by GHT: " << eslm.size() << endl;

	// set new location of model centroid
	mPoint.set_X(mcPoint.get_X());
	mPoint.set_Y(mcPoint.get_Y());
	// translate between model and scene
	translation.set_X(dx);
	translation.set_Y(dy);
	return eslm;
}

vector<Point> generalizing_Hough_Transform(ptr_IntMatrix mGradient,
		ptr_IntMatrix sGradient, vector<Point> mLandmarks, Point &ePoint,
		Point &mPoint, double &angle, Point &translation, Point &mtranslation) {
	//int rows = mGradient->get_Rows();
	//int cols = mGradient->get_Cols();
	//Point center(cols / 2, rows / 2);
	Point mTemp;
	Line mLine = principal_Axis(mGradient, mTemp);
	RTable rentries = rTable_Construct(mGradient, mTemp);
	vector<Point> eslm;

	eslm = ght_With_Entries(rentries, mLine, mTemp, sGradient, mLandmarks, ePoint,
			mPoint, angle, translation, mtranslation);

	return eslm;
}
