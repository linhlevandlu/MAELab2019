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

#ifndef PHOUGHTRANSFORM_H_
#define PHOUGHTRANSFORM_H_
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
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"

#include "PHTEntry.h"

bool closetLine(Line line1, Line line2);
class PHoughTransform {
private:
	Point refPoint;
	vector<PHTEntry> phtEntries;
	//ptr_IntMatrix accumulator;
public:
	PHoughTransform();
	virtual ~PHoughTransform();

	void set_Ref_Point(Point rpoint);
	Point get_Ref_Point();
	vector<PHTEntry> get_PHT_Entries();
	vector<PHTEntry> construct_PHT_Table(vector<Line> lines);
};

#endif /* PHOUGHTRANSFORM_H_ */
