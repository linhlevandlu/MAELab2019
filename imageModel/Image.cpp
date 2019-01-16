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



#include "Image.h"

const int BIN_SIZE = 256;
const int MEAN_GRAY = 120;
const int DECREASE_25 = 25;
const int DECREASE_5 = 5;

const double RED_COEFFICIENT = 0.299f;
const double GREEN_COEFFICIENT = 0.587f;
const double BLUE_COEFFICIENT = 0.114f;


/*
 * The equations to convert from RGB or BGR to Grayscale: Gvalue = 0.299*R + 0.587*G + 0.114*B
 *
 */
//================================================= Utils methods =================================================
Matrix<int> convert_RGB_To_Gray(Matrix<RGB> rgbMatrix)
{
	int width = rgbMatrix.get_Cols();
	int height = rgbMatrix.get_Rows();
	Matrix<int> grayMatrix(height,width);
	//grayMatrix = new Matrix<int>(height, width);
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{

			grayMatrix.set_At_Position(h, w,
				round(
					((double) rgbMatrix.get_At_Position(h, w).get_R() * RED_COEFFICIENT)
						+ ((double) rgbMatrix.get_At_Position(h, w).get_G() * GREEN_COEFFICIENT)
						+ ((double) rgbMatrix.get_At_Position(h, w).get_B() * BLUE_COEFFICIENT)));
		}
	}

	return grayMatrix;
}

// ================================================== End utils methods =============================================
//===================================================== Constructor =================================================
Image::Image()
{
	// TODO Auto-generated constructor stub
	medianHistogram = 0;
	meanHistogram = 0;
	thresholdValue = 0;
}
Image::Image(const Image &cpimage)
{
	fileName = cpimage.fileName;
	listOfLines = cpimage.listOfLines;
	manualLandmarks = cpimage.manualLandmarks;
	autoLandmarks = cpimage.autoLandmarks;
	imgMatrix = cpimage.imgMatrix;
	grayMatrix = cpimage.grayMatrix;
	grayHistogram = cpimage.grayHistogram;
	medianHistogram = cpimage.medianHistogram;
	meanHistogram = cpimage.meanHistogram;
	thresholdValue = cpimage.thresholdValue;
}
Image::~Image()
{
	/*listOfLines.clear();
	 manualLandmarks.clear();
	 autoLandmarks.clear();
	 delete grayMatrix;
	 delete imgMatrix;
	 delete grayHistogram;*/
}
;
Image::Image(std::string filePath)
{
	medianHistogram = 0;
	meanHistogram = 0;
	thresholdValue = 0;

	fileName = filePath;
	int rows = 0, cols = 0;
	imgMatrix = read_JPG_To_RGB(filePath.c_str(), rows, cols);

	grayMatrix = convert_RGB_To_Gray(imgMatrix);
	grayHistogram.set_Rows(1);
	grayHistogram.set_Cols(BIN_SIZE);
	grayHistogram.Init();

	rgbHistogram.set_Rows(1);
	rgbHistogram.set_Cols(BIN_SIZE);
	rgbHistogram.Init();

	calc_Histogram();

	calc_Threshold_Value();

	//cout << endl << "Threshold value: " << thresholdValue;
}

//===================================================== End constructor ================================================

//================================================ Public methods ======================================================
void Image::set_File_Name(std::string filePath)
{
	fileName = filePath;
}
std::string Image::get_File_Name()
{
	return fileName;
}
string Image::get_Name()
{
	size_t found1 = fileName.find_last_of("/");
	size_t found2 = fileName.find_last_of(".");
	string str = fileName.substr(found1 + 1, found2 - found1 - 1);
	size_t len = str.length();
	size_t i = 0;
	while (i < len)
	{
		char c = str.at(i);
		if (c == ' ')
		{
			str.replace(i, 1, "_");
		}
		i++;
	}
	return str+".JPG";
}
void Image::set_Manual_Landmarks(string tpsFile)
{
	manualLandmarks = read_Manual_Landmarks(tpsFile);
}
void Image::set_RGB_Matrix(Matrix<RGB> rgbMatrix)
{
	imgMatrix = rgbMatrix;
	grayMatrix = convert_RGB_To_Gray(imgMatrix);
}
void Image::set_Gray_Matrix(Matrix<int> graymatrix)
{
	grayMatrix = graymatrix;
}
void Image::set_Auto_Landmarks(vector<Point> eLandmarks)
{
	autoLandmarks = eLandmarks;
}
Matrix<int> Image::get_Gray_Matrix()
{
	return grayMatrix;
}
Matrix<RGB> Image::get_RGB_Matrix()
{
	return imgMatrix;
}
/*
vector<Line> Image::get_List_Of_Lines()
{
	if (listOfLines.size() <= 0)
		get_Approximate_Lines(3);
	return listOfLines;
}
*/
float Image::get_Median_Histogram()
{
	if (medianHistogram == 0)
		calc_Histogram();
	return medianHistogram;

}
Matrix<int> Image::get_Gray_Histogram()
{	
	return grayHistogram;
}
Matrix<RGB> Image::get_RGB_Histogram()
{
	return rgbHistogram;
}
float Image::get_Mean_Histogram()
{
	if (meanHistogram == 0)
		calc_Histogram();
	return meanHistogram;
}
float Image::get_Threshold_Value()
{
	if (thresholdValue == 0)
		calc_Threshold_Value();
	return thresholdValue;
}

/*
vector<Line> Image::get_Approximate_Lines(double minDistance)
{
	vector<Point> cPoints;
	vector<Edge> listOfEdges = canny_Algorithm(cPoints);
	vector<Line> totalLines;

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

	cout << "\n Min distance: " << minDistance;
	cout << "\n Total lines after segment the edge: " << totalLines.size()<<endl;
	listOfLines = totalLines;
	return totalLines;
}
*/

vector<Point> Image::read_Manual_Landmarks(string fileName)
{
	manualLandmarks.clear();
	vector<Point> mLandmarks = read_TPS_File(fileName.c_str());
	int rows = grayMatrix.get_Rows();
	Point temp;
	Point p;
	for (size_t t = 0; t < mLandmarks.size(); t++)
	{
		temp = mLandmarks.at(t);
		p.set_X(temp.get_X());
		p.set_Y(rows - temp.get_Y());
		manualLandmarks.push_back(p);
	}

	mLandmarks.clear();

	return manualLandmarks;
}
vector<Point> Image::get_List_Of_Manual_Landmarks()
{
	return manualLandmarks;
}
vector<Point> Image::get_List_Of_Auto_Landmarks()
{
	return autoLandmarks;
}
//================================================ End public methods ==================================================

//================================================ Private methods =====================================================
void Image::calc_Histogram()
{

	if (grayMatrix.get_Cols() != 0)
	{
		float total = 0;
		float pi = 0;
		int array[BIN_SIZE] =
		{ 0 };

		int redArray[BIN_SIZE] = {0};
		int greenArray[BIN_SIZE] = {0};
		int blueArray[BIN_SIZE] = {0};
		RGB color;
		for (int r = 0; r < grayMatrix.get_Rows(); r++)
		{
			for (int c = 0; c < grayMatrix.get_Cols(); c++)
			{
				int k = grayMatrix.get_At_Position(r, c);
				array[k]++;

				RGB ccolor = imgMatrix.get_At_Position(r,c);
				redArray[ccolor.get_R()]++;
				greenArray[ccolor.get_G()]++;
				blueArray[ccolor.get_B()]++;
			}
		}

		for (int k = 0; k < BIN_SIZE; k++)
		{
			// save and calculate the mean of gray histogram
			grayHistogram.set_At_Position(0, k, array[k]);
			total += array[k];
			pi += (k * array[k]);

			// save the rgb histogram
			color.set_R(redArray[k]);
			color.set_G(greenArray[k]);
			color.set_B(blueArray[k]);
			rgbHistogram.set_At_Position(0,k,color);
		}

		// calculate the mean of histogram
		meanHistogram = (pi / total);
		// calculate the median of histogram
		float avm = total / 2;
		float temp = 0;
		for (int m = 0; m < BIN_SIZE; m++)
		{
			temp += array[m];
			if (temp >= avm)
			{
				medianHistogram = m;
				break;
			}
		}
	}

}

void Image::calc_Threshold_Value()
{
	if (medianHistogram == 0 || meanHistogram == 0)
		calc_Histogram();
	int limit1 =
		meanHistogram > medianHistogram ? medianHistogram : meanHistogram;
	limit1 = (limit1 >= 120) ? (limit1 - DECREASE_25) : (limit1 - DECREASE_5);
	int imax1 = -1, max1 = -1;
	for (int index = 0; index < limit1; index++)
	{
		int temp = grayHistogram.get_At_Position(0, index);
		if (temp > max1)
		{
			max1 = temp;
			imax1 = index;
		}
	}
	int limit2 =
		meanHistogram > medianHistogram ? meanHistogram : medianHistogram;
	int imin = -1, min = max1;
	for (int k = imax1; k < limit2; k++)
	{
		int temp = grayHistogram.get_At_Position(0, k);
		if (temp < min)
		{
			min = temp;
			imin = k;
		}
	}
	int max2 = -1, imax2 = -1;
	for (int k = limit2; k < BIN_SIZE; k++)
	{
		int temp = grayHistogram.get_At_Position(0, k);
		if (temp > max2)
		{
			max2 = temp;
			imax2 = k;
		}
	}
	float mid1 = (imin + imax1) / 2;
	float mid2 = (imin + imax2) / 2;
	thresholdValue = (mid1 + mid2) / 2;
}

/*
vector<Edge> Image::canny_Algorithm(vector<Point> &cPoints)
{
	if (thresholdValue == 0)
		cal_Threshold_Value();

	ptr_IntMatrix binMatrix = binary_Threshold(&grayMatrix, (int) thresholdValue,
		MAX_GRAY_VALUE);
	ptr_IntMatrix cannyMatrix = canny_Process(binMatrix, (int) thresholdValue,
		3 * (int) thresholdValue, cPoints);

	vector<Edge> listOfEdges;
	listOfEdges = suzuki(cannyMatrix);

	delete cannyMatrix;
	delete binMatrix;

	return listOfEdges;
}
*/
// angle in degree
/*ptr_DoubleMatrix Image::getRotationMatrix2D(Point center, double angle,
 double scale)
 {
 cout << "\n"<<angle;
 //if (angle > 0)
 //	angle = -angle;

 double alpha = cos(angle * M_PI / 180) * scale;
 double beta = sin(angle * M_PI / 180) * scale;

 ptr_DoubleMatrix rotateM = new Matrix<double>(2, 3, 0);

 rotateM->set_At_Position(0, 0, alpha);
 rotateM->set_At_Position(0, 1, beta);
 rotateM->set_At_Position(0, 2,
 (1 - alpha) * center.get_X() - beta * center.get_Y());
 rotateM->set_At_Position(1, 0, -beta);
 rotateM->set_At_Position(1, 1, alpha);
 rotateM->set_At_Position(1, 2,
 beta * center.get_X() + (1 - alpha) * center.get_Y());

 return rotateM;
 }*/

void Image::rotate(Point center, double angle, double scale)
{
	/*ptr_DoubleMatrix rotationMatrix = getRotationMatrix2D(center, angle, scale);
	 ptr_IntMatrix source = grayMatrix;*/
	RGB color;
	grayMatrix.rotation(center, angle, scale, 0);
	imgMatrix.rotation(center, angle, scale, color);

	//*grayMatrix = gray;
	//*imgMatrix = rgb;

	/*int rows = source->get_Rows();
	 int cols = source->get_Cols();

	 double a00 = rotationMatrix->get_At_Position(0, 0);
	 double a01 = rotationMatrix->get_At_Position(0, 1);
	 double b00 = rotationMatrix->get_At_Position(0, 2);
	 double a10 = rotationMatrix->get_At_Position(1, 0);
	 double a11 = rotationMatrix->get_At_Position(1, 1);
	 double b10 = rotationMatrix->get_At_Position(1, 2);
	 ptr_RGBMatrix rgbSource = imgMatrix;
	 RGB color;
	 color.R = color.G = color.B = 0;
	 ptr_RGBMatrix rgbDest = new Matrix<RGB>(rows, cols, color);
	 ptr_IntMatrix result = new Matrix<int>(rows, cols, 0);
	 for (int row = 0; row < rows; row++)
	 {
	 for (int col = 0; col < cols; col++)
	 {
	 int value = source->get_At_Position(row, col);
	 color = rgbSource->get_At_Position(row, col);

	 int xnew = round(a00 * col + a01 * row + b00);
	 int ynew = round(a10 * col + a11 * row + b10);

	 if (xnew >= 0 && xnew < cols && ynew >= 0 && ynew < rows)
	 {
	 result->set_At_Position(ynew, xnew, value);
	 rgbDest->set_At_Position(ynew, xnew, color);
	 }
	 }
	 }
	 for (int row = 0; row < rows; row++)
	 {
	 for (int col = 1; col < cols - 1; col++)
	 {
	 int value = result->get_At_Position(row, col);
	 int leftValue = result->get_At_Position(row, col - 1);
	 int rightValue = result->get_At_Position(row, col + 1);
	 if (value == 0 && leftValue > 0 && rightValue >= 0)
	 {
	 result->set_At_Position(row, col, (leftValue + rightValue) / 2);
	 }

	 RGB color = rgbDest->get_At_Position(row, col);
	 RGB leftColor = rgbDest->get_At_Position(row, col - 1);
	 RGB rightColor = rgbDest->get_At_Position(row, col + 1);
	 if (color.R == 0 && color.G == 0 && color.B == 0
	 && (leftColor.R != 0 || leftColor.G != 0 || leftColor.B != 0)
	 && (rightColor.R != 0 || rightColor.G != 0 || rightColor.B != 0))
	 {
	 RGB newColor;
	 newColor.R = (leftColor.R + rightColor.R) / 2;
	 newColor.G = (leftColor.G + rightColor.G) / 2;
	 newColor.B = (leftColor.B + rightColor.B) / 2;
	 rgbDest->set_At_Position(row, col, newColor);
	 }
	 }
	 }
	 imgMatrix = rgbDest;
	 grayMatrix = result;*/

	//delete rotationMatrix;
	//delete source;
	//delete rgbSource;
	//delete rgbDest;
	//return result;
}
vector<Matrix<int> > Image::split_Channels()
{
	vector<Matrix<int> > channels;
	int rows = imgMatrix.get_Rows();
	int cols = imgMatrix.get_Cols();
	Matrix<int> red_Channel(rows,cols,0);
	Matrix<int> green_Channel(rows,cols,0);
	Matrix<int> blue_Channel(rows,cols,0);
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			RGB color = imgMatrix.get_At_Position(r,c);
			red_Channel.set_At_Position(r,c,color.get_R());
			green_Channel.set_At_Position(r,c,color.get_G());
			blue_Channel.set_At_Position(r,c,color.get_B());
		}
	}
	channels.push_back(red_Channel);
	channels.push_back(green_Channel);
	channels.push_back(blue_Channel);
	return channels;
}

// Merge 3 channels (R, G, B) into RGB matrix
Matrix<RGB> Image::merge_Channels(vector<Matrix<int> > channels)
{
	Matrix<RGB> mergeMatrix;
	if(channels.size() == 3) // 
	{
		int rows = channels.at(0).get_Rows();
		int cols = channels.at(0).get_Cols();
		mergeMatrix.set_Rows(rows);
		mergeMatrix.set_Cols(cols);
		mergeMatrix.Init();
		RGB color;
		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < cols; c++)
			{
				color.set_R(channels.at(0).get_At_Position(r,c));
				color.set_G(channels.at(1).get_At_Position(r,c));
				color.set_B(channels.at(2).get_At_Position(r,c));
				mergeMatrix.set_At_Position(r,c,color);
			}
		}
		imgMatrix = mergeMatrix;
	}
	return mergeMatrix;
}
//================================================ End private methods =====================================================

