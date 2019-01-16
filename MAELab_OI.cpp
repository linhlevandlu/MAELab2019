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

#include <iostream>
#include <math.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <dirent.h>

using namespace std;

#include "imageModel/Point.h"
#include "imageModel/Line.h"
#include "imageModel/Edge.h"
#include "imageModel/Matrix.h"
#include "imageModel/Image.h"

#include "io/Reader.h"
#include "MAELab.h"

int main(int argc, char* argv[])
{
	cout << endl << "MAELab without graphic user interface !!!";
	string modeljpg, modeltps, scenejpg, scenetps, method, savejpg, savetps;
	if (argc == 1)
	{
		cout<<endl<<"The program is running without arguments. The default values are used!"<<endl;
		modeljpg = "data/model.JPG";
		modeltps = "data/model.TPS";
		scenejpg = "data/scene1.jpg";
	}
	else
	{
		cout<<endl<<"The program is running with arguments!"<<endl;
		modeljpg = argv[1];
		modeltps = argv[2];
		scenejpg = argv[3];
	}

	Image model(modeljpg);
	Image scene(scenejpg);
	
	vector<Point> estLandmarks = mae_PCAI_Method(model,modeltps,scene);
	cout<<"\nNumber of estimated landmarks: "<<estLandmarks.size();
	for (size_t i = 0; i < estLandmarks.size(); i++)
	{
		Point pi = estLandmarks.at(i);
		cout<<"\nLandmark "<<i+1<<": "<<pi.get_X()<<"\t"<<pi.get_Y();
	}
	cout << endl;
	return 1;

}