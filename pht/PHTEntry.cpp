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


#include "PHTEntry.h"

//============================== Method of class ==========================
PHTEntry::PHTEntry()
{
	// TODO Auto-generated constructor stub

}

PHTEntry::~PHTEntry()
{
	// TODO Auto-generated destructor stub
}
vector<HoughSpace> PHTEntry::get_List_Hough_Space()
{
	return listHoughSpace;
}

void PHTEntry::set_List_Hough_Space(vector<HoughSpace> listHoughSpace)
{
	this->listHoughSpace = listHoughSpace;
}

Line PHTEntry::get_Obj_Line()
{
	return oLine;
}

void PHTEntry::set_Obj_Line(Line line)
{
	oLine = line;
}

Line PHTEntry::get_Ref_Line()
{
	return rLine;
}

void PHTEntry::set_Ref_Line(Line line)
{
	rLine = line;
}
void PHTEntry::add_Hough_Space(HoughSpace hSpace)
{
	listHoughSpace.push_back(hSpace);
}
