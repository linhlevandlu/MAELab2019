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

#include "LineSegment.h"

vector<Line> line_Segment(Image inputImage)
{
	float thresh_value = inputImage.get_Threshold_Value();
	Matrix<int> grayImage = inputImage.get_Gray_Matrix();
	ptr_IntMatrix binMatrix = binary_Threshold(&grayImage, (int) thresh_value,
		MAX_GRAY_VALUE);
	vector<Point> cPoints;
	ptr_IntMatrix cannyMatrix = cannyProcess(binMatrix, (int) thresh_value,
		3 * (int) thresh_value, cPoints);
	vector<Edge> listOfEdges;
	listOfEdges = suzuki(cannyMatrix);

	vector<Line> totalLines;
	double minDistance = 3;	
	Edge ed;
	for (size_t i = 0; i < listOfEdges.size(); i++)
	{
		ed = listOfEdges.at(i);
		vector<Point> breakPoints = ed.segment(minDistance);
		vector<Line> lines = ed.get_Lines(breakPoints);
		totalLines.insert(totalLines.end(), lines.begin(), lines.end());

		breakPoints.clear();
		lines.clear();
	}
	return totalLines;
}