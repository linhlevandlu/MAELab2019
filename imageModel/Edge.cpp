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

#include "Edge.h"

//=================================================== Constructor ===========================================

Edge::Edge() {
	//listOfLines = NULL;
	//listOfPoints = NULL;
	//listOfBreakPoints = NULL;

}
Edge::Edge(const Edge &cpedge) {
	listOfPoints = cpedge.listOfPoints;
	listOfBreakPoints = cpedge.listOfBreakPoints;
}
Edge::~Edge() {
	//listOfPoints.clear();
	//listOfBreakPoints.clear();
}

Edge::Edge(std::vector<Point> points) {
	listOfPoints = points;
}
//=================================================== Get and set methods ===========================================

std::vector<Point> Edge::get_Points() {
	return listOfPoints;
}

void Edge::set_Points(std::vector<Point> points) {
	listOfPoints = points;
}

//=================================================== Private methods ===========================================

/*
 *
 * Check a point is exist in list of Break Point or not.
 *
 */
bool check_Point_In_List(std::vector<Point> listPoints, Point point) {
	Point p;
	for (size_t i = 0; i < listPoints.size(); i++) {
		p = listPoints.at(i);
		if (point.get_X() == p.get_X() && point.get_Y() == p.get_Y())
			return true;
	}
	return false;
}

static std::vector<Point> vvp; // used to keep the break points after recursive time
void Edge::break_Edge(double minDistance) { // in old program, minDistance is constant with value is 3
	if (listOfPoints.size() <= 0)
		return;
	Point p0;
	Point pend;
	int size = listOfPoints.size();

	p0 = listOfPoints.at(0);
	pend = listOfPoints.at(size - 1);

	if (size > 2) {
		Line line(p0, pend);
		double distance = 0, maxDistance = 0;
		size_t imax = 0;
		Point pi;
		for (int i = 1; i < size - 1; i++) {
			pi = listOfPoints.at(i);
			distance = line.perpendicular_Distance(pi);
			if (distance > maxDistance) {
				maxDistance = distance;
				imax = i;
			}
		}
		if (maxDistance > minDistance) { // continue break the edge
			std::vector<Point> part1(listOfPoints.begin(),
					listOfPoints.begin() + imax + 1);
			std::vector<Point> part2(listOfPoints.begin() + imax,
					listOfPoints.end());
			Edge edge1(part1);
			Edge edge2(part2);
			edge1.break_Edge(minDistance);
			edge2.break_Edge(minDistance);
		}
	}

	if (!check_Point_In_List(vvp, p0))
		vvp.push_back(p0);
	if (!check_Point_In_List(vvp, pend))
		vvp.push_back(pend);

}

//=================================================== Public methods ===========================================
std::vector<Point> Edge::segment(double minDistance) {
	listOfBreakPoints.clear();
	vvp.clear();
	break_Edge(minDistance);
	listOfBreakPoints = vvp;

	return listOfBreakPoints;
}
vector<Line> Edge::get_Lines(vector<Point> listPoints) {
	vector<Line> listLines;

	if (listPoints.size() > 0) {
		Point p0 = listPoints.at(0);
		Point p1;
		for (size_t i = 1; i < listPoints.size(); i++) {
			p1 = listPoints.at(i);
			Line l(p0, p1);
			listLines.push_back(l);
			p0 = p1;
		}

	}
	return listLines;
}
void Edge::sort_By_X() {
	std::sort(listOfPoints.begin(), listOfPoints.end(), xComparation);
}
void Edge::sort_By_Y() {
	std::sort(listOfPoints.begin(), listOfPoints.end(), yComparation);
}

