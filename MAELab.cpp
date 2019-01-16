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


#include "MAELab.h"

ptr_IntMatrix mae_get_Gray_Histogram(Image* inputImage)
{
	Matrix<int> grayHist = inputImage->get_Gray_Histogram();
	ptr_IntMatrix result = new Matrix<int>(grayHist.get_Rows(),
	                                       grayHist.get_Cols());
	result->set_Data(grayHist.get_Data());	
	return result;
}

ptr_RGBMatrix mae_get_RGB_Histogram(Image* inputImage)
{
	Matrix<RGB> rgbHistData = inputImage->get_RGB_Histogram();
	ptr_RGBMatrix rgbHist = &rgbHistData;
	ptr_RGBMatrix result = new Matrix<RGB>(rgbHist->get_Rows(),
	                                       rgbHist->get_Cols());
	for (int i = 0; i < rgbHist->get_Cols(); i++)
	{
		result->set_At_Position(0, i, rgbHist->get_At_Position(0, i));
	}
	return result;
}

ptr_IntMatrix mae_Binary_Threshold(Image* inputImage)
{
	Matrix<int> inputData = inputImage->get_Gray_Matrix();
	ptr_IntMatrix inputMatrix = &inputData;
	float thresh_value = inputImage->get_Threshold_Value();

	ptr_IntMatrix binaryMatrix = new Matrix<int>(inputMatrix->get_Rows(),
		inputMatrix->get_Cols(), MAX_GRAY_VALUE);
	*binaryMatrix = *(binary_Threshold(inputMatrix, thresh_value, MAX_GRAY_VALUE));
	ptr_IntMatrix result = new Matrix<int>(inputMatrix->get_Rows(),
		inputMatrix->get_Cols(), MAX_GRAY_VALUE);
	*result = *binaryMatrix;
	delete binaryMatrix;
	return result;
}


vector<Point> mae_Canny_Algorithm(Image* inputImage)
{

	float thresh_value = inputImage->get_Threshold_Value();
	Matrix<int> grayImage = inputImage->get_Gray_Matrix();
	ptr_IntMatrix binMatrix = binary_Threshold(&grayImage, (int) thresh_value,
		MAX_GRAY_VALUE);
	vector<Point> cPoints;
	ptr_IntMatrix cannyMatrix = cannyProcess(binMatrix, (int) thresh_value,
		3 * (int) thresh_value, cPoints);
	delete binMatrix;
	delete cannyMatrix;
	return cPoints;
}

vector<Edge> mae_Suzuki_Algorithm(Image* inputImage)
{
	float thresh_value = inputImage->get_Threshold_Value();
	Matrix<int> grayImage = inputImage->get_Gray_Matrix();
	ptr_IntMatrix binMatrix = binary_Threshold(&grayImage, (int) thresh_value,
		MAX_GRAY_VALUE);
	vector<Point> cPoints;
	ptr_IntMatrix cannyMatrix = cannyProcess(binMatrix, (int) thresh_value,
		3 * (int) thresh_value, cPoints);
	vector<Edge> listOfEdges;
	listOfEdges = suzuki(cannyMatrix);
	delete binMatrix;
	delete cannyMatrix;
	return listOfEdges;
}

vector<Line> mae_Line_Segment(Image* inputImage)
{
	vector<Line> totalLines = line_Segment(*inputImage);
	return totalLines;
}

Matrix<RGB> mae_Gaussian_Filter(Image* inputImage, Matrix<double> kernel)
{
	vector<Matrix<int> > channels = inputImage->split_Channels();
	for (size_t i = 0; i < channels.size(); i++)
	{
		Matrix<int> gau = gaussian_Blur(channels[i], kernel);
		channels[i] = gau;
	}
	return inputImage->merge_Channels(channels);
}

Matrix<int> mae_Robert_Filter(Image* inputImage)
{
	Matrix<int> grayData = inputImage->get_Gray_Matrix();
	Matrix<int> rbResult = Robert_Operation(&grayData);
	rbResult = post_Filter(rbResult);
	return rbResult;
}

Matrix<double> mae_Sobel_Filter(Image* inputImage)
{
	Matrix<int> grayData = inputImage->get_Gray_Matrix();
	//Matrix<int> sbResult = SobelOperation(&grayData);
	Matrix<double> sbResult = Sobel_Operation_Double(&grayData);
	//sbResult = postFilter(sbResult);
	return sbResult;
}

Matrix<int> mae_Erode(Image* inputImage)
{
	Matrix<int> sbResult = *(mae_Binary_Threshold(inputImage));
	ptr_IntMatrix erResult = erode(&sbResult, KERNEL_SIZE_DEFAULT);
	return *erResult;
}

Matrix<int> mae_Dilate(Image* inputImage)
{
	Matrix<int> sbResult = *(mae_Binary_Threshold(inputImage));
	ptr_IntMatrix dlResult = dilate(&sbResult, KERNEL_SIZE_DEFAULT);
	return *dlResult;
}

Matrix<int> mae_Open_Binary(Image* inputImage)
{
	Matrix<int> sbResult = *(mae_Binary_Threshold(inputImage));
	ptr_IntMatrix opResult = open_Binary(&sbResult, KERNEL_SIZE_DEFAULT);
	return *opResult;
}
Matrix<int> mae_Close_Binary(Image* inputImage)
{
	Matrix<int> sbResult = *(mae_Binary_Threshold(inputImage));
	ptr_IntMatrix clResult = close_Binary(&sbResult, KERNEL_SIZE_DEFAULT);
	return *clResult;
}

vector<Point> mae_PHT_Method(Image modelImage, string modelLandmarks, Image sceneImage, AngleAccuracy acc, int cols, int templSize, int sceneSize, Point &ePoint,
	double &angleDiff)
{
	modelImage.read_Manual_Landmarks(modelLandmarks);
	LandmarkDetection lmDetection;
	lmDetection.set_Ref_Image(modelImage);
	//Point ePoint;
	//double angleDiff;
	vector<Point> estLandmarks = lmDetection.landmarksAutoDectect(sceneImage,acc,cols,templSize,sceneSize, ePoint,angleDiff);
	return estLandmarks;
}
vector<Point> mae_PCAI_Method(Image modelImage,string modelLandmarks, Image sceneImage)
{
	// read manual landmarks of model image
	modelImage.read_Manual_Landmarks(modelLandmarks);
	vector<Point> estLandmarks = PCAI(modelImage,sceneImage,modelImage.get_List_Of_Manual_Landmarks());
	return estLandmarks;
}

vector<Point> mae_Sobel_SIFT_Method(Image modelImage, string modelLandmarks, Image sceneImage,int patchsize)
{
	Matrix<double> kernel = get_Gaussian_Kernel(5, 2);
	Matrix<int> gsResult = gaussian_Blur(sceneImage.get_Gray_Matrix(), kernel);
	Matrix<int> sbResult = Sobel_Operation(&gsResult);
	int tValue = 6; //Otsu threshold
	vector<Point> contours;
	for (int r = 0; r < sbResult.get_Rows(); r++)
	{
		for (int c = 0; c < sbResult.get_Cols(); c++)
		{
			int value = sbResult.get_At_Position(r, c);
			if (value == 0)
			{
				sbResult.set_At_Position(r, c, 0);
			}
			else
			{
				if (value >= tValue)
				{
					sbResult.set_At_Position(r, c, 255);
				}
				else
					sbResult.set_At_Position(r, c, 0);
			}
		}
	}
	ptr_IntMatrix dlResult = close_Binary(&sbResult, 3);
	for (int r = 0; r < dlResult->get_Rows(); r++)
	{
		for (int c = 0; c < dlResult->get_Cols(); c++)
		{
			if (dlResult->get_At_Position(r, c) == 255)
				contours.push_back(Point(c, r));
		}
	}
	modelImage.read_Manual_Landmarks(modelLandmarks);
	Matrix<int> modelData = modelImage.get_Gray_Matrix();
	Matrix<int> matData = sceneImage.get_Gray_Matrix();
	vector<Point> estLandmarks = verify_Descriptors_3(&modelData, &matData,
			contours, modelImage.get_List_Of_Manual_Landmarks(), patchsize);
	return estLandmarks;
}

vector<Point> mae_Canny_SIFT_Method(Image modelImage, string modelLandmarks, Image sceneImage,int patchsize)
{
	modelImage.read_Manual_Landmarks(modelLandmarks);
	vector<Point> cPoints = mae_Canny_Algorithm(&sceneImage);
	Matrix<int> modelGrayData = modelImage.get_Gray_Matrix();
	Matrix<int> sceneGrayData = sceneImage.get_Gray_Matrix();
	vector<Point> estLandmarks = verify_Descriptors_4(&modelGrayData,
			&sceneGrayData, cPoints, modelImage.get_List_Of_Manual_Landmarks(),
			patchsize);
	return estLandmarks;
}
/*
//=================================== old version ============================================
vector<Line> segment(ptr_Treatments treatment, Image image, int minDistance)
{

	Segmentation sg;
	treatment = &sg;
	treatment->setRefImage(image);

	vector<Line> lines = sg.segment();
	return lines;
}
*/
ShapeHistogram geomHistogram(ptr_Treatments treatment, Image image,
	AngleAccuracy angleAcc, int cols)
{
	GeometricHistgoram geoHistogram;
	treatment = &geoHistogram;
	treatment->set_Ref_Image(image);
	vector<Line> lines = mae_Line_Segment(&image);
	return geoHistogram.geom_Histogram(lines,angleAcc, cols);
}

/*
double bhattacharyyaDistance(ptr_Treatments treatment, Image sceneImage,
	AngleAccuracy angleAcc, int cols)
{
	GeometricHistgoram geoHistogram;
	treatment = &geoHistogram;
	return geoHistogram.bhattacharyyaDistance(sceneImage, angleAcc, cols);
}
PHoughTransform phtEntriesTable(ptr_Treatments treatment, Image image)
{
	ProHoughTransform pht;
	treatment = &pht;
	treatment->setRefImage(image);
	return pht.constructPHT();
}
vector<Point> estimatedLandmarks(ptr_Treatments treatment, Image sceneImage,
	AngleAccuracy acc, int cols, int templSize, int sceneSize, Point &ePoint,
	double angleDiff)
{
	LandmarkDetection lmd;
	lmd.setRefImage(treatment->getRefImage());
	//treatment = &lmd;
	return lmd.landmarksAutoDectect(sceneImage, acc, cols, templSize, sceneSize,
		ePoint, angleDiff);
}

*/
double measure_Centroid_Point(vector<Point> landmarks, Point &ebary)
{
	ebary.set_X(0);
	ebary.set_Y(0);

	int totalX = 0;
	int totalY = 0;
	size_t lmSize = landmarks.size();

	Point pi;
	for (size_t i = 0; i < lmSize; i++)
	{
		pi = landmarks.at(i);
		totalX += pi.get_X();
		totalY += pi.get_Y();
	}

	if (lmSize > 0)
	{
		ebary.set_X(totalX / lmSize);
		ebary.set_Y(totalY / lmSize);
	}
	double totalDistance = 0;
	Point lm;
	for (size_t j = 0; j < lmSize; j++)
	{
		lm = landmarks.at(j);
		//ptr_Line line = new Line(ebary,lm);
		double distance = distance_Points(ebary, landmarks.at(j));
		//cout<<"\nDistance: \t"<<distance;
		totalDistance += (distance * distance);

	}
	//cout<<"\n"<<sqrt(totalDistance);
	return sqrt(totalDistance);
}
