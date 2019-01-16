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

#ifndef GEOMETRICHISTGORAM_H_
#define GEOMETRICHISTGORAM_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Edge.h"
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"

#include "../histograms/ShapeHistogram.h"

#include "Treatments.h"

double bhattacharyyaMetric(ShapeHistogram refHist, ShapeHistogram sceneHist);

class GeometricHistgoram: public Treatments
{

public:
	GeometricHistgoram();
	virtual ~GeometricHistgoram();
	//ShapeHistogram gm_Histogram(Image image, AngleAccuracy angleAcc, int cols);
	ShapeHistogram gm_Histogram(vector<Line> lines, AngleAccuracy angleAcc, int cols);
	ShapeHistogram geom_Histogram(vector<Line> lines, AngleAccuracy angleAcc, int cols);
	double bhattacharyya_Distance(vector<Line> sceneLines, vector<Line> refLines,
	AngleAccuracy angleAcc, int cols);
};

#endif /* GEOMETRICHISTGORAM_H_ */
