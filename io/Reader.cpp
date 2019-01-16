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

#include "Reader.h"

//====================================================== JPEG File ==================================================

Matrix<RGB> read_JPG_To_RGB(const char* filename,int &rows, int &cols)
{
	return decompress_JPEG(filename,rows,cols);
}
void save_RGB(const char* filename, ptr_RGBMatrix rgbMatrix)
{
	RGB_2_JPEG(filename, rgbMatrix);
}
void save_Gray_Scale(const char* filename, ptr_IntMatrix grayMatrix)
{
	Gray_2_JPEG(filename, grayMatrix);
}
vector<string> read_Directory(const char* dirPath)
{
	vector<string> result;
	DIR *pDir;
	struct dirent *entry;
	string filePath;
	pDir = opendir(dirPath);
	if (pDir == NULL)
	{
		cout << "\n Error when reading the folder";
		result.clear();
		return result;
	}
	while (entry = readdir(pDir))
	{
		if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			filePath = entry->d_name;
			result.push_back(filePath);
		}
	}
	closedir(pDir);
	std::sort(result.begin(),result.end());
	return result;
}
// ============================================================== TPS File =====================================================
vector<Point> read_TPS_File(const char* filename)
{
	return read_TPS(filename);
}

