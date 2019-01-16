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

#include "GeometricHistgoram.h"

double bhattacharyyaMetric(ShapeHistogram refHist, ShapeHistogram sceneHist)
{
	double ref_size = refHist.get_Entries();
	double scene_size = sceneHist.get_Entries();
	double distance = 0;

	ptr_IntMatrix refMatrix = refHist.get_Matrix();
	ptr_IntMatrix sceneMatrix = sceneHist.get_Matrix();

	for (int r = 0; r < refMatrix->get_Rows(); r++)
	{
		for (int c = 0; c < refMatrix->get_Cols(); c++)
		{
			double value_1 = sqrt(refMatrix->get_At_Position(r, c) / ref_size);
			double value_2 = sqrt(sceneMatrix->get_At_Position(r, c) / scene_size);
			distance += value_1 * value_2;
		}
	}

	return distance;
}

GeometricHistgoram::GeometricHistgoram()
{
	// TODO Auto-generated constructor stub

}

GeometricHistgoram::~GeometricHistgoram()
{
	// TODO Auto-generated destructor stub
}

ShapeHistogram GeometricHistgoram::gm_Histogram(vector<Line> lines,
	AngleAccuracy angleAcc, int cols)
{
	ShapeHistogram shapeHist;
	vector<LocalHistogram> listLocalHists = shapeHist.construct_PGH(lines);
	shapeHist.construct_PGH_Matrix(listLocalHists, angleAcc, cols);
	return shapeHist;
}

ShapeHistogram GeometricHistgoram::geom_Histogram(vector<Line> lines,AngleAccuracy angleAcc,
	int cols)
{
	return gm_Histogram(lines, angleAcc, cols);
}

double GeometricHistgoram::bhattacharyya_Distance(vector<Line> sceneLines, vector<Line> refLines,
	AngleAccuracy angleAcc, int cols)
{
	ShapeHistogram refHistogram = gm_Histogram(refLines, angleAcc,
		cols);
	ShapeHistogram sceneHistogram = gm_Histogram(sceneLines, angleAcc, cols);
	return bhattacharyyaMetric(refHistogram, sceneHistogram);
}
