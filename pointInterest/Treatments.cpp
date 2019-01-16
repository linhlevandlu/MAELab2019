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


#include "Treatments.h"

Treatments::Treatments()
{
	// TODO Auto-generated constructor stub

}
Treatments::Treatments(Image img)
{
	refImage = img;
}
Treatments::~Treatments()
{
	// TODO Auto-generated destructor stub
}
void Treatments::set_Ref_Image(Image image)
{
	refImage = image;
}
Image Treatments::get_Ref_Image()
{
	return refImage;
}
