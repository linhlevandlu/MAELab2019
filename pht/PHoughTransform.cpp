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


#include "PHoughTransform.h"

bool closetLine(Line line1, Line line2)
{
	int cond1 = 60;
	int cond2 = 15;
	int cond3 = 5;

	double distance1 = line2.perpendicular_Distance(line1.get_Begin());
	double distance2 = line2.perpendicular_Distance(line1.get_End());
	if (line1.get_Length() > cond1 && line2.get_Length() > cond1
		&& line1.angle_Lines(line2) >= cond2
		&& (distance1 <= cond3 || distance2 <= cond3))
		return true;
	return false;
}
double computeDistance(Line objLine, Point rPoint)
{
	return objLine.perpendicular_Distance(rPoint);
}

Point closestPoint(Line objLine, Point origin)
{
	vector<double> equation = objLine.get_Equation();
	double a = equation.at(0);
	double b = equation.at(1);
	double c = equation.at(2);
	double x, y;

	if (a == 0 && b == 1)
	{
		x = origin.get_X();
		y = c;
	}
	else
	{
		if (a == 1 && b == 0)
		{
			x = c;
			y = origin.get_Y();
		}
		else
		{
			x = (b * (b * origin.get_X() - a * origin.get_Y()) - (a * c))
				/ (a * a + b * b);
			y = (a * (-b * origin.get_X() + a * origin.get_Y()) - (b * c))
				/ (a * a + b * b);
		}
	}
	return Point(x, y);
}

double computeAngle(Line objLine, Point rPoint)
{
	Line oX(rPoint,
		Point(rPoint.get_X() + 100, rPoint.get_Y()));
	Point pCloset = closestPoint(objLine, rPoint);
	Line distanceLine(rPoint, pCloset);
	double theta = oX.angle_Lines(distanceLine);

	if (pCloset.get_Y() > rPoint.get_Y())
	{
		return 360 - theta;

	}
	return theta;
}

//==================================== Methods in class ==================================================
PHoughTransform::PHoughTransform()
{
	// TODO Auto-generated constructor stub

}

PHoughTransform::~PHoughTransform()
{
	// TODO Auto-generated destructor stub
}

void PHoughTransform::set_Ref_Point(Point rpoint)
{
	refPoint = rpoint;
}
Point PHoughTransform::get_Ref_Point()
{

	return refPoint;
}
vector<PHTEntry> PHoughTransform::get_PHT_Entries()
{
	//if (!phtEntries.empty())
	return phtEntries;
	//return null;
}
vector<PHTEntry> PHoughTransform::construct_PHT_Table(vector<Line> lines)
{

	vector<PHTEntry> entries;
	Line line1,line2;
	for (size_t i = 0; i < lines.size(); i++)
	{
		line1 = lines.at(i);
		HoughSpace hs1;
		hs1.angle = computeAngle(line1, refPoint);
		hs1.distance = computeDistance(line1, refPoint);

		for (size_t j = 0; j < lines.size(); j++)
		{
			line2 = lines.at(j);
			if (i != j && closetLine(line1, line2))
			{
				HoughSpace hs2;
				hs2.angle = computeAngle(line2, refPoint);
				hs2.distance = computeDistance(line2, refPoint);

				PHTEntry entry;// = new PHTEntry();
				entry.set_Ref_Line(line1);
				entry.set_Obj_Line(line2);
				entry.add_Hough_Space(hs1);
				entry.add_Hough_Space(hs2);

				entries.push_back(entry);
			}
		}
	}
	phtEntries = entries;

	return entries;
}
