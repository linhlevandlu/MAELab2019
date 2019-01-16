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

#include "LandmarkDetection.h"

LandmarkDetection::LandmarkDetection()
{
	// TODO Auto-generated constructor stub

}

LandmarkDetection::~LandmarkDetection()
{
	// TODO Auto-generated destructor stub
}
vector<Point> LandmarkDetection::refineLandmarks(vector<Point> estLandmarks,
	vector<Point> segmentation)
{
	vector<Point> result;
	Point pi;
	for (size_t i = 0; i < estLandmarks.size(); i++)
	{
		pi = estLandmarks.at(i);
		bool onCurve = check_Point_In_List(segmentation, pi);
		if (onCurve)
		{
			result.push_back(pi);
		}
		else
		{
			Point mPoint = nearest_Point(segmentation, pi);

			result.push_back(mPoint);
		}
	}
	return result;
}
vector<Point> LandmarkDetection::landmarksAutoDectect(Image sceneImage,
	AngleAccuracy acc, int cols, int templSize, int sceneSize, Point &ePoint,
	double &angleDiff)
{
	vector<Point> result;
	Image modelImage = Treatments::refImage;
	vector<Point> manualLMs = modelImage.get_List_Of_Manual_Landmarks();
	vector<Line> mLines = line_Segment(modelImage);// modelImage.get_List_Of_Lines();
	vector<Line> sLines = line_Segment(sceneImage) ;//sceneImage.get_List_Of_Lines();
	int width = modelImage.get_Gray_Matrix().get_Cols();
	int height = modelImage.get_Gray_Matrix().get_Rows();

	ShapeHistogram mHistogram;
	vector<LocalHistogram> mLocalHist = mHistogram.construct_PGH(mLines);
	mHistogram.construct_PGH_Matrix(mLocalHist, acc, cols);

	ShapeHistogram sHistogram;
	vector<LocalHistogram> sLocalHist = sHistogram.construct_PGH(sLines);
	sHistogram.construct_PGH_Matrix(sLocalHist, acc, cols);

	double bhatt = bhattacharyyaMetric(mHistogram, sHistogram);
	cout << "\nBhattacharrya: " << bhatt << endl;
	//if (bhatt > 0.9)
	//{
	PHoughTransform mpht;
	mpht.set_Ref_Point(Point(width / 2, height / 2));

	vector<PHTEntry> entriesTable = mpht.construct_PHT_Table(mLines);
	vector<Point> phtEsLM = phtLandmarks(entriesTable, mpht.get_Ref_Point(), sLines,
		width, height, manualLMs, angleDiff, ePoint);

	cout << "\n Number of landmarks (pht): " << phtEsLM.size();
	cout << "\nAngle difference: " << angleDiff << endl;
	if (phtEsLM.size() > 0)
	{
		result = verify_Landmarks(modelImage, sceneImage, manualLMs, phtEsLM,
			templSize, sceneSize, angleDiff, ePoint);
		//result = phtEsLM;

	}
	entriesTable.clear();
	phtEsLM.clear();
	//}

	return result;
}

vector<Point> LandmarkDetection::landmarksAutoDectect2(Image &sceneImage,
	int templSize, int sceneSize)
{
	Image modelImage = Treatments::refImage;
	vector<Point> manualLMs = modelImage.get_List_Of_Manual_Landmarks();
	ProHoughTransform proHT;
	proHT.set_Ref_Image(modelImage);
	double angle = 0;
	Point ePoint, mPoint;
	int rows = sceneImage.get_Gray_Matrix().get_Rows();
	int cols = sceneImage.get_Gray_Matrix().get_Cols();
	ptr_IntMatrix newScene = new Matrix<int>(rows, cols, 0);
	vector<Point> modelPoints, scenePoints, newScenePoints;
	Point translation, scaleMove;
	double scaleX = 0.0, scaleY = 0.0;
	vector<Point> phtEsLM = proHT.generalTransform(sceneImage, angle, ePoint,
		mPoint, newScene, translation, modelPoints, scenePoints, newScenePoints,
		scaleX, scaleY, scaleMove);
	ptr_IntMatrix modelSeg = new Matrix<int>(rows, cols, 0);
	ptr_IntMatrix sceneSeg = new Matrix<int>(rows, cols, 0);
	Point mi;
	for (size_t i = 0; i < modelPoints.size(); i++)
	{
		mi = modelPoints.at(i);
		if (mi.get_X() >= 0 && mi.get_Y() >= 0 && mi.get_Y() < rows
			&& mi.get_X() < cols)
		{
			modelSeg->set_At_Position(mi.get_Y(), mi.get_X(), 255);
		}
	}
	for (size_t i = 0; i < newScenePoints.size(); i++)
	{
		mi = newScenePoints.at(i);
		if (mi.get_X() >= 0 && mi.get_Y() >= 0 && mi.get_Y() < rows
			&& mi.get_X() < cols)
		{
			sceneSeg->set_At_Position(mi.get_Y(), mi.get_X(), 255);
		}
	}
	vector<Point> result;
	if (phtEsLM.size() > 0)
	{
		result = verify_Landmarks_2(modelSeg, sceneSeg, manualLMs, phtEsLM, templSize,
			sceneSize);
	}
	phtEsLM.clear();
	delete newScene;
	delete modelSeg;
	delete sceneSeg;

	// reverse the coordinate of estimated landmarks
	Point pi;
	int dx = translation.get_X();
	int dy = translation.get_Y();
	for (size_t i = 0; i < result.size(); i++)
	{
		pi = result.at(i);
		int xnew = pi.get_X(), ynew = pi.get_Y();
		rotate_A_Point(xnew, ynew, ePoint, angle, 1, xnew, ynew);
		result.at(i).set_X(xnew + dx);
		result.at(i).set_Y(ynew + dy);
	}
	return result;
}

vector<Point> LandmarkDetection::landmarksWithSIFT(Image &sceneImage,
	int templSize, int sceneSize)
{
	Image modelImage = Treatments::refImage;
	vector<Point> manualLMs = modelImage.get_List_Of_Manual_Landmarks();
	ProHoughTransform proHT;
	proHT.set_Ref_Image(modelImage);
	double angle = 0;
	Point ePoint, mPoint;
	int rows = sceneImage.get_Gray_Matrix().get_Rows();
	int cols = sceneImage.get_Gray_Matrix().get_Cols();
	ptr_IntMatrix newScene = new Matrix<int>(rows, cols, 0);
	vector<Point> modelPoints, scenePoints, newScenePoints;
	Point translation, scaleMove;
	double scaleX = 0, scaleY =0;
	vector<Point> phtEsLM = proHT.generalTransform(sceneImage, angle, ePoint,
		mPoint, newScene, translation, modelPoints, scenePoints, newScenePoints,
		scaleX, scaleY, scaleMove);
	ptr_IntMatrix modelSeg = new Matrix<int>(rows, cols, 0);
	ptr_IntMatrix sceneSeg = new Matrix<int>(rows, cols, 0);
	Point mi;
	for (size_t i = 0; i < modelPoints.size(); i++)
	{
		mi = modelPoints.at(i);
		if (mi.get_X() >= 0 && mi.get_Y() >= 0 && mi.get_Y() < rows
			&& mi.get_X() < cols)
		{
			modelSeg->set_At_Position(mi.get_Y(), mi.get_X(), 255);
		}
	}
	for (size_t i = 0; i < newScenePoints.size(); i++)
	{
		mi = newScenePoints.at(i);
		if (mi.get_X() >= 0 && mi.get_Y() >= 0 && mi.get_Y() < rows
			&& mi.get_X() < cols)
		{
			sceneSeg->set_At_Position(mi.get_Y(), mi.get_X(), 255);
		}
	}
	vector<Point> result;
	if (phtEsLM.size() > 0)
	{
		/*result = verifyDescriptors(modelImage.get_Gray_Matrix(), newScene,
		 manualLMs, phtEsLM, templSize, sceneSize);*/
		result = verify_Descriptors(modelSeg, sceneSeg, manualLMs, phtEsLM,
			templSize, sceneSize);
	}
	//result = phtEsLM;
	phtEsLM.clear();
	delete newScene;
	delete modelSeg;
	delete sceneSeg;
	for (size_t i = 0; i < result.size(); i++)
	{
		mi = result.at(i);
		int x = mi.get_X() - scaleMove.get_X();
		int y = mi.get_Y() - scaleMove.get_Y();
		result.at(i).set_X(x/ scaleX);
		result.at(i).set_Y(y/scaleY);
	}

	// reverse the coordinate of estimated landmarks
	Point pi;
	int dx = translation.get_X();
	int dy = translation.get_Y();
	for (size_t i = 0; i < result.size(); i++)
	{
		pi = result.at(i);
		int xnew = pi.get_X(), ynew = pi.get_Y();
		rotate_A_Point(xnew, ynew, ePoint, angle, 1, xnew, ynew);
		result.at(i).set_X(xnew + dx);
		result.at(i).set_Y(ynew + dy);
	}
	result = refineLandmarks(result, scenePoints);
	return result;
}
/*void LandmarkDetection::landmarksOnDir(string modelName, string folderScene,
	vector<string> sceneImages, AngleAccuracy acc, int cols, int templSize,
	int sceneSize, Point &ePoint, double &angleDiff, string saveFolder)
{

	Image modelImage = Treatments::refImage;
	vector<Point> manualLMs = modelImage.get_List_Of_Manual_Landmarks();
	vector<Line> mLines = line_Segment(modelImage) ;//modelImage.get_List_Of_Lines();
	int width = modelImage.get_Gray_Matrix().get_Cols();
	int height = modelImage.get_Gray_Matrix().get_Rows();

	ShapeHistogram mHistogram;
	vector<LocalHistogram> mLocalHist = mHistogram.construct_PGH(mLines);
	mHistogram.construct_PGH_Matrix(mLocalHist, acc, cols);

	ShapeHistogram sHistogram;
	vector<LocalHistogram> sLocalHist;
	Image *sceneImage;
	for (size_t i = 0; i < sceneImages.size(); i++)
	{
		string sceneName = sceneImages.at(i);
		cout << "\n==============================================" << sceneName;
		vector<Point> result;
		angleDiff = 0;
		sceneImage = new Image(folderScene + "/" + sceneName);
		vector<Line> sLines = line_Segment(*sceneImage) ;// sceneImage->get_List_Of_Lines();

		sLocalHist = sHistogram.construct_PGH(sLines);
		sHistogram.construct_PGH_Matrix(sLocalHist, acc, cols);

		double bhatt = bhattacharyyaMetric(mHistogram, sHistogram);
		cout << "\nBhattacharrya: " << bhatt << endl;
		PHoughTransform mpht;
		mpht.set_Ref_Point(Point(width / 2, height / 2));

		vector<PHTEntry> entriesTable = mpht.construct_PHT_Table(mLines);
		vector<Point> phtEsLM = phtLandmarks(entriesTable, mpht.get_Ref_Point(),
			sLines, width, height, manualLMs, angleDiff, ePoint);

		cout << "\n Number of landmarks (pht): " << phtEsLM.size();
		cout << "\nAngle difference: " << angleDiff << endl;

		if (phtEsLM.size() > 0)
		{
			result = verify_Landmarks(modelImage, *sceneImage, manualLMs, phtEsLM,
				templSize, sceneSize, angleDiff, ePoint);

		}
		entriesTable.clear();
		phtEsLM.clear();

		string saveFile = saveFolder + "/" + modelName + "_" + sceneName + ".TPS";
		ofstream inFile(saveFile.c_str());
		inFile << "LM=" << result.size() << "\n";
		Point pk;
		for (size_t k = 0; k < result.size(); k++)
		{
			pk = result.at(k);
			inFile << pk.get_X() << " " << pk.get_Y() << "\n";
		}
		inFile << "IMAGE=" << saveFile << "\n";
		inFile.close();
		delete sceneImage;
	}

}
void LandmarkDetection::landmarksOnDir2(string modelName, string folderScene,
	vector<string> sceneImages, string saveFolder)
{

	Image modelImage = Treatments::refImage;

	int rows = modelImage.get_Gray_Matrix().get_Rows();
	int cols = modelImage.get_Gray_Matrix().get_Cols();
	ptr_IntMatrix mgradirection = new Matrix<int>(rows, cols, -1);
	vector<Point> modelPoints;
	*mgradirection = *(get_Gradient_DMatrix(modelImage, modelPoints));
	vector<Point> mLandmarks = modelImage.get_List_Of_Manual_Landmarks();
	Point center(cols / 2, rows / 2);
	RTable rentries = rTable_Construct(mgradirection, center);
	Point mTemp;
	Line mLine = principal_Axis(mgradirection, mTemp);
	//sceneImages.size()
	for (size_t i = 0; i < 20; i++)
	{
		Image *sceneImage;
		string sceneName = sceneImages.at(i);
		cout << "\n==============================================" << sceneName;

		sceneImage = new Image(folderScene + "/" + sceneName);

		ptr_IntMatrix gradirection = new Matrix<int>(rows, cols, -1);
		vector<Point> scenePoints;
		*gradirection = *(get_Gradient_DMatrix(*sceneImage, scenePoints));

		Point mPoint, ePoint, translation, mtranslation;
		double angle;
		//vector<Point> eslm = generalizingHoughTransform(mgradirection, gradirection,
		//	mLandmarks, ePoint, mPoint, angle, translation);

		vector<Point> eslm = ght_With_Entries(rentries, mLine, mTemp, gradirection,
			mLandmarks, ePoint, mPoint, angle, translation, mtranslation);

		ptr_IntMatrix newScene = new Matrix<int>(rows, cols, 0);
		// move the model to the same and rotate the scene
		int dx = translation.get_X();
		int dy = translation.get_Y();
		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < cols; c++)
			{
				int value = sceneImage->get_Gray_Matrix().get_At_Position(r, c);
				int xnew = c + dx;
				int ynew = r + dy;
				rotate_A_Point(c + dx, r + dy, mPoint, angle, 1, xnew, ynew);
				if (xnew >= 0 && ynew >= 0 && ynew < rows && xnew < cols)
				{
					newScene->set_At_Position(ynew, xnew, value);
				}
			}
		}

		vector<Point> estLandmarks;

		if (eslm.size() > 0)
		{
			cout << "\nAngle difference: " << angle << endl;
			cout << "\n Number of landmarks (ght): " << eslm.size();
			Matrix<int> mgData = modelImage.get_Gray_Matrix();
			estLandmarks = verify_Landmarks_2(&mgData, newScene,
				mLandmarks, eslm, 100, 300);

		}

		cout << "\n Number of landmarks (matching): " << estLandmarks.size();

		// reverse the coordinate of estimated landmarks
		Point pi;
		int dx2 = ePoint.get_X() - mPoint.get_X();
		int dy2 = ePoint.get_Y() - mPoint.get_Y();
		for (size_t i = 0; i < estLandmarks.size(); i++)
		{
			pi = estLandmarks.at(i);
			int xnew = 0, ynew = 0;
			rotate_A_Point(pi.get_X(), pi.get_Y(), mPoint, -angle, 1, xnew, ynew);
			xnew += dx2;
			ynew += dy2;
			estLandmarks.at(i).set_X(xnew);
			estLandmarks.at(i).set_Y(ynew);
		}

		string saveFile = saveFolder + "/" + modelName + "_" + sceneName + ".TPS";
		ofstream inFile(saveFile.c_str());
		inFile << "LM=" << estLandmarks.size() << "\n";
		Point epk;
		RGB color;
		color.set_R(255);
		color.set_G(255);
		for (size_t k = 0; k < estLandmarks.size(); k++)
		{
			epk = estLandmarks.at(k);
			if (epk.get_X() >= 0 && epk.get_X() < cols && epk.get_Y() >= 0
				&& epk.get_Y() < rows)
			{
				fill_Circle(sceneImage->get_RGB_Matrix(), epk, 5, color);
			}

			inFile << epk.get_X() << " " << rows - epk.get_Y() << "\n";
		}
		inFile << "IMAGE=" << saveFile << "\n";
		inFile.close();

		string imgFile = saveFolder + "/" + modelName + "_" + sceneName;
		Matrix<RGB> sData = sceneImage->get_RGB_Matrix();
		save_RGB(imgFile.c_str(), &sData);

		estLandmarks.clear();
		eslm.clear();

		delete gradirection;
		delete sceneImage;
		delete newScene;
	}

}
void LandmarkDetection::landmarksOnDir3(string modelName, string folderScene,
	vector<string> sceneImages, string saveFolder)
{

	Image modelImage = Treatments::refImage;

	int rows = modelImage.get_Gray_Matrix().get_Rows();
	int cols = modelImage.get_Gray_Matrix().get_Cols();
	ptr_IntMatrix mgradirection = new Matrix<int>(rows, cols, -1);
	vector<Point> modelPoints;
	*mgradirection = *(get_Gradient_DMatrix(modelImage, modelPoints));
	vector<Point> mLandmarks = modelImage.get_List_Of_Manual_Landmarks();
	Point center(cols / 2, rows / 2);
	RTable rentries = rTable_Construct(mgradirection, center);
	Point mTemp;
	Line mLine = principal_Axis(mgradirection, mTemp);
	string saveFile = saveFolder + "/" + modelName + "_angle.TPS";
	ofstream inFile(saveFile.c_str());
//sceneImages.size()
	for (size_t i = 260; i < sceneImages.size(); i++)
	{
		Image *sceneImage;
		string sceneName = sceneImages.at(i);
		cout << "\n==============================================" << sceneName
			<< endl;

		sceneImage = new Image(folderScene + "/" + sceneName);

		ptr_IntMatrix gradirection = new Matrix<int>(rows, cols, -1);
		vector<Point> scenePoints;
		*gradirection = *(get_Gradient_DMatrix(*sceneImage, scenePoints));

		Point mPoint, ePoint, translation, mtranslation;
		double angle;
		//vector<Point> eslm = generalizingHoughTransform(mgradirection, gradirection,
		//	mLandmarks, ePoint, mPoint, angle, translation);

		vector<Point> eslm = ght_With_Entries(rentries, mLine, mTemp, gradirection,
			mLandmarks, ePoint, mPoint, angle, translation, mtranslation);
		inFile << sceneName << "\t" << angle << "\n";

		eslm.clear();
		delete gradirection;
		delete sceneImage;
	}
	delete mgradirection;
	inFile.close();
}
void LandmarkDetection::landmarksOnDir4(string modelName, string folderScene,
	vector<string> sceneImages, string saveFolder, string folderLandmarks,
	vector<string> landmarks)
{

	Image modelImage = Treatments::refImage;
	vector<Point> mLandmarks = modelImage.get_List_Of_Manual_Landmarks();
	int rows = modelImage.get_Gray_Matrix().get_Rows();
	int cols = modelImage.get_Gray_Matrix().get_Cols();
	for (size_t i = 20; i < 40; i++)
	{

		string sceneName = sceneImages.at(i);
		cout << "\n==============================================" << sceneName;
		Image sceneImage(folderScene + "/" + sceneName);
		string lmFile = folderLandmarks + "/" + landmarks.at(i);
		sceneImage.read_Manual_Landmarks(lmFile);
		vector<Point> mnLandmarks = sceneImage.get_List_Of_Manual_Landmarks();

		vector<Point> estLandmarks = landmarksAutoDectect2(sceneImage, 9, 36);
		cout << "\n Number of landmarks (matching): " << estLandmarks.size();

		string saveFile = saveFolder + "/" + modelName + "_" + sceneName + ".TPS";
		ofstream inFile(saveFile.c_str());
		inFile << "LM=" << estLandmarks.size() << "\n";
		Point epk;
		RGB color;
		color.set_R(255);
		color.set_G(255);
		
		for (size_t k = 0; k < estLandmarks.size(); k++)
		{
			epk = estLandmarks.at(k);
			if (epk.get_X() >= 0 && epk.get_X() < cols && epk.get_Y() >= 0
				&& epk.get_Y() < rows)
			{
				color.set_G(255);
				fill_Circle(sceneImage.get_RGB_Matrix(), epk, 3, color);
				color.set_G(0);
				fill_Circle(sceneImage.get_RGB_Matrix(), mnLandmarks.at(k), 3, color);
			}
			Line line(epk, mnLandmarks.at(k));

			inFile << epk.get_X() << " " << rows - epk.get_Y() << "\t"
				<< line.get_Length() << "\n";
		}
		inFile << "IMAGE=" << saveFile << "\n";
		inFile.close();

		string imgFile = saveFolder + "/" + modelName + "_" + sceneName;
		Matrix<RGB> saveData = sceneImage.get_RGB_Matrix();
		save_RGB(imgFile.c_str(), &saveData);

		estLandmarks.clear();

	}
}*/
