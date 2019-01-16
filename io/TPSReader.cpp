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


#include "TPSReader.h"

std::vector<Point> read_TPS(const char* filename)
{
	ifstream openFile(filename);
	vector<Point> mLandmarks;
	string lineText;
	if (openFile.is_open())
	{
		getline(openFile, lineText);
		size_t poseq = lineText.find_first_of("=");
		string nop = lineText.substr(poseq + 1, lineText.size() - 1 - poseq);

		int nPoints = atoi(nop.c_str());
		//cout<<"\n number points: "<<nPoints;
		int i = 0;
		while (i < nPoints)
		{
			getline(openFile, lineText);
			int pos = lineText.find(" ");
			string l1 = lineText.substr(0, pos);
			string l2 = lineText.substr(pos + 1, lineText.length() - pos - 1);
			mLandmarks.push_back(Point(atoi(l1.c_str()), atoi(l2.c_str())));
			i++;
		}
	}
	else
	{
		cout << endl << "Cannot open this file !";
	}
	openFile.close();
	return mLandmarks;
}
std::vector<Point> read_TPS_With_Difference(const char* filename,
	string &difference)
{
	ifstream openFile(filename);
	vector<Point> mLandmarks;
	string lineText;
	if (openFile.is_open())
	{
		getline(openFile, lineText);
		size_t poseq = lineText.find_first_of("=");
		string nop = lineText.substr(poseq + 1, lineText.size() - 1 - poseq);

		int nPoints = atoi(nop.c_str());
		int i = 0;
		while (i < nPoints)
		{
			getline(openFile, lineText);
			int pos = lineText.find(" ");
			string l1 = lineText.substr(0, pos);
			string l2 = lineText.substr(pos + 1, lineText.length() - pos - 1);
			mLandmarks.push_back(Point(atoi(l1.c_str()), atoi(l2.c_str())));
			i++;
		}
		getline(openFile, difference);
	}
	else
	{
		cout << endl << "Cannot open this file !";
	}
	openFile.close();
	return mLandmarks;
}
