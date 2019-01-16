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

#ifndef IMAGEVIEWER_H_
#define IMAGEVIEWER_H_

#include "../imageModel/Point.h"
#include <QtGui/qlabel.h>
#include <QtGui/qscrollarea.h>
#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <float.h>
using namespace std;

#include <QtGui/QApplication>
#include <QtGui/QScrollBar>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QPrintDialog>
#include <QtGui/QAction>
#include <QtGui/QMenuBar>
#include <QtGui/QPainter>
#include <QtGui/QCloseEvent>
#include <QtCore/QSettings>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QMessageBox>
#include <QtCore/QDebug>
#include <QtGui/QIcon>
#include <QtGui/qwidget.h>

#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Edge.h"
#include "../imageModel/Matrix.h"
#include "../imageModel/Image.h"
#include "../io/Reader.h"

#include "../utils/ImageConvert.h"
#include "../utils/Drawing.h"
#include "../utils/Converter.h"

#include "../segmentation/Filters.h"
/*
#include "../segmentation/Thresholds.h"
#include "../segmentation/Canny.h"
#include "../segmentation/Suzuki.h"
#include "../segmentation/Projection.h"

*/
#include "../pht/GHTInPoint.h"
#include "../pht/PHTEntry.h"
#include "../pht/PHoughTransform.h"
#include "../pht/PCA.h"

#include "../histograms/ShapeHistogram.h"
//#include "../histograms/LBP.h"


#include "../pointInterest/Treatments.h"
//#include "../pointInterest/Segmentation.h"
#include "../pointInterest/ProHoughTransform.h"
#include "../pointInterest/LandmarkDetection.h"

//#include "../correlation/CrossCorrelation.h"
//#include "../correlation/DescriptorDistance.h"

#include "../MAELab.h"

class Image;

class ImageViewer: public QMainWindow
{
Q_OBJECT
public:
	ImageViewer();
	~ImageViewer();

	QString fileName;
	QImage qImage;
	Image *matImage;

	QAction *parameterAction;
	QDialog *parameterDialog;

	void loadImage(QString fn);
	void loadImage(Image *_matImage, QImage _qImage, QString tt);
protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void about();
	//void testMethod();

	void open();
	void save();
	void saveAs();

	void zoomIn();
	void zoomOut();
	void normalSize();
	void fitToWindow();
	void gScaleHistogram();
	void rgbHistogramCalc();
	void displayManualLandmarks();
	void displayAutoLandmarks();

	void binThreshold();
	void cannyAlgorithm();
	void suzukiAlgorithm();
	void lineSegmentation();

	void gauFilter();
	void robertFilter();
	void sobelFilter();
	void erosionOperation();
	void dilationOperation();
	void openOperation();
	void closeOperation();

	//void gHoughTransform();
	void extractLandmarks();
	//void measureMBary();
	//void measureEBary();
	//void dirAutoLandmarks();
	//void dirCentroidMeasure();
	//void dirGenerateData();
	void sobelAndSIFT();
	void cannyAndSIFT();

	void pcaiMethodViewer();

private:

	QLabel *imageLabel;
	QScrollArea *scrollArea;
	double scaleFactor;

	// menu
	QMenu *fileMenu;
	QMenu *viewMenu;
	QMenu *segmentationMenu;
	QMenu *processMenu;
	QMenu *dominantPointMenu;
	QMenu *helpMenu;

	// toolbar
	QToolBar *fileToolBar;
	QToolBar *viewToolBar;

	//menu action
	QAction *openAct;
	QAction *printAct;
	QAction *saveAct;
	QAction *saveAsAct;
	QAction *closeAct;
	QAction *exitAct;

	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *normalSizeAct;
	QAction *fitToWindowAct;
	QAction *gHistogramAct;
	QAction *rgbHistogramAct;
	QAction *displayMLandmarksAct;
	QAction *displayALandmarksAct;

	QAction *aboutAct;
	//QAction *testAct;

	QAction *binaryThresholdAct;
	QAction *cannyAct;
	QAction *suzukiAct;
	QAction *lineSegmentationAct;

	QAction *gauAct;
	QAction *robertAct;
	QAction *sobelAct;
	QAction *erosionAct;
	QAction *dilationAct;
	QAction *openBinaryAct;
	QAction *closeBinaryAct;

	//QAction *phtAct;
	//QAction *phtPointsAct;
	QAction *autoLandmarksAct;
	//QAction *measureMBaryAct;
	//QAction *measureEBaryAct;
	//QAction *dirAutoLandmarksAct;
	//QAction *dirCentroidMeasureAct;
	//QAction *dirGenerateDataAct;
	QAction *sobelAndSIFTAct;
	QAction *cannyAndSIFTAct;

	// Action registration
	QAction *pcaiAct;

	// private methods
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createFileMenu();
	void createViewMenu();
	void createHelpMenu();
	void createSegmentationMenu();
	void createLandmarksMenu();
	void createRegistrationMenu();
	void createFilterMenu();
	void activeFunction();
	void viewMenuUpdateActions();
	void scaleImage(double factor);
	void adjustScrollBar(QScrollBar *scrollBar, double factor);

	void displayLandmarks(Image *image, std::vector<Point> lms, RGB color);
};

#endif /* IMAGEVIEWER_H_ */
