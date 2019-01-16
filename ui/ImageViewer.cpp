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


#include "ImageViewer.h"

void ImageViewer::createFileMenu()
{
	openAct = new QAction(QIcon("./ui/resources/ico/open.png"), tr("&Open..."),
			this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAct = new QAction(QIcon("./ui/resources/ico/save.png"), tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	saveAct->setEnabled(false);
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAct = new QAction(tr("Save &As..."), this);
	saveAsAct->setShortcuts(QKeySequence::SaveAs);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));
	saveAsAct->setEnabled(false);
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	closeAct = new QAction(tr("&Close"), this);
	closeAct->setShortcut(tr("Ctrl+W"));
	closeAct->setStatusTip(tr("Close this window"));
	connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

}
void ImageViewer::createViewMenu()
{
	zoomInAct = new QAction(QIcon("./ui/resources/ico/1uparrow.png"),
			tr("Zoom &In (25%)"), this);
	zoomInAct->setShortcut(tr("Ctrl++"));
	zoomInAct->setEnabled(false);
	connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

	zoomOutAct = new QAction(QIcon("./ui/resources/ico/1downarrow.png"),
			tr("Zoom &Out (25%)"), this);
	zoomOutAct->setShortcut(tr("Ctrl+-"));
	zoomOutAct->setEnabled(false);
	connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

	normalSizeAct = new QAction(tr("&Normal Size"), this);
	normalSizeAct->setShortcut(tr("Ctrl+N"));
	normalSizeAct->setEnabled(false);
	connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

	fitToWindowAct = new QAction(tr("&Fit to Window"), this);
	fitToWindowAct->setEnabled(false);
	fitToWindowAct->setCheckable(true);
	fitToWindowAct->setShortcut(tr("Ctrl+J"));
	connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

	gHistogramAct = new QAction(tr("&Gray-scale Histogram"), this);
	gHistogramAct->setEnabled(false);
	connect(gHistogramAct, SIGNAL(triggered()), this, SLOT(gScaleHistogram()));

	rgbHistogramAct = new QAction(tr("&RGB Histogram"), this);
	rgbHistogramAct->setEnabled(false);
	connect(rgbHistogramAct, SIGNAL(triggered()), this,
			SLOT(rgbHistogramCalc()));

	displayMLandmarksAct = new QAction(tr("&Display manual landmarks"), this);
	displayMLandmarksAct->setEnabled(false);
	displayMLandmarksAct->setCheckable(true);
	displayMLandmarksAct->setChecked(false);
	connect(displayMLandmarksAct, SIGNAL(triggered()), this,
			SLOT(displayManualLandmarks()));

	displayALandmarksAct = new QAction(tr("&Display estimated landmarks"),
			this);
	displayALandmarksAct->setEnabled(false);
	displayALandmarksAct->setCheckable(true);
	connect(displayALandmarksAct, SIGNAL(triggered()), this,
			SLOT(displayAutoLandmarks()));

}
void ImageViewer::viewMenuUpdateActions()
{
	zoomInAct->setEnabled(!fitToWindowAct->isChecked());
	zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
	normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor)
{
	Q_ASSERT(imageLabel->pixmap());
	scaleFactor *= factor;
	imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

	adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
	adjustScrollBar(scrollArea->verticalScrollBar(), factor);

	zoomInAct->setEnabled(scaleFactor < 3.0);
	zoomOutAct->setEnabled(scaleFactor > 0.333);
}
void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
	scrollBar->setValue(
			int(
					factor * scrollBar->value()
							+ ((factor - 1) * scrollBar->pageStep() / 2)));
}

void ImageViewer::createHelpMenu()
{
	aboutAct = new QAction(tr("&About MAELab"), this);
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	/*testAct = new QAction(tr("&Test a function"), this);
	testAct->setEnabled(false);
	connect(testAct, SIGNAL(triggered()), this, SLOT(testMethod()));*/
}
void ImageViewer::createSegmentationMenu()
{
	binaryThresholdAct = new QAction(tr("&Binary threshold"), this);
	binaryThresholdAct->setEnabled(false);
	connect(binaryThresholdAct, SIGNAL(triggered()), this,
			SLOT(binThreshold()));

	cannyAct = new QAction(tr("&Canny result"), this);
	cannyAct->setEnabled(false);
	connect(cannyAct, SIGNAL(triggered()), this, SLOT(cannyAlgorithm()));

	suzukiAct = new QAction(tr("&Suzuki result"), this);
	suzukiAct->setEnabled(false);
	connect(suzukiAct, SIGNAL(triggered()), this, SLOT(suzukiAlgorithm()));

	lineSegmentationAct = new QAction(tr("&View segmentation result"), this);
	lineSegmentationAct->setEnabled(false);
	connect(lineSegmentationAct, SIGNAL(triggered()), this,
			SLOT(lineSegmentation()));
}

void ImageViewer::createFilterMenu()
{
	gauAct = new QAction(tr("&Gaussian filter"), this);
	gauAct->setEnabled(false);
	connect(gauAct, SIGNAL(triggered()), this, SLOT(gauFilter()));

	robertAct = new QAction(tr("&Robert filter"), this);
	robertAct->setEnabled(false);
	connect(robertAct, SIGNAL(triggered()), this, SLOT(robertFilter()));

	sobelAct = new QAction(tr("&Sobel filter"), this);
	sobelAct->setEnabled(false);
	connect(sobelAct, SIGNAL(triggered()), this, SLOT(sobelFilter()));

	erosionAct = new QAction(tr("&Erosion"), this);
	erosionAct->setEnabled(false);
	connect(erosionAct, SIGNAL(triggered()), this, SLOT(erosionOperation()));

	dilationAct = new QAction(tr("&Dilation"), this);
	dilationAct->setEnabled(false);
	connect(dilationAct, SIGNAL(triggered()), this, SLOT(dilationOperation()));

	openBinaryAct = new QAction(tr("&Open"), this);
	openBinaryAct->setEnabled(false);
	connect(openBinaryAct, SIGNAL(triggered()), this, SLOT(openOperation()));

	closeBinaryAct = new QAction(tr("&Close"), this);
	closeBinaryAct->setEnabled(false);
	connect(closeBinaryAct, SIGNAL(triggered()), this, SLOT(closeOperation()));
}
void ImageViewer::createLandmarksMenu()
{
	/*phtAct = new QAction(tr("&Probabilistic Hough Transform"), this);
	 phtAct->setEnabled(false);
	 phtAct->setShortcut(tr("Ctrl+P"));
	 connect(phtAct, SIGNAL(triggered()), this, SLOT(pHoughTransform()));*/

	/*phtPointsAct = new QAction(tr("&Generalizing Hough Transform"), this);
	phtPointsAct->setEnabled(false);
	phtPointsAct->setShortcut(tr("Ctrl+G"));
	connect(phtPointsAct, SIGNAL(triggered()), this, SLOT(gHoughTransform()));
	*/
	autoLandmarksAct = new QAction(tr("&Probabilistic Hough Transform"), this);
	autoLandmarksAct->setEnabled(false);
	autoLandmarksAct->setShortcut(tr("Ctrl+L"));
	connect(autoLandmarksAct, SIGNAL(triggered()), this,
			SLOT(extractLandmarks()));

	pcaiAct = new QAction(tr("PCAI method"), this);
	pcaiAct->setEnabled(false);
	pcaiAct->setShortcut(tr("Ctrl+I"));
	connect(pcaiAct, SIGNAL(triggered()), this, SLOT(pcaiMethodViewer()));

	sobelAndSIFTAct = new QAction(tr("Sobel and SIFT method"), this);
	sobelAndSIFTAct->setEnabled(false);
	//sobelAndSIFTAct->setShortcut(tr("Ctrl+I"));
	connect(sobelAndSIFTAct, SIGNAL(triggered()), this, SLOT(sobelAndSIFT()));

	cannyAndSIFTAct = new QAction(tr("Canny and SIFT method"), this);
	cannyAndSIFTAct->setEnabled(false);
	//sobelAndSIFTAct->setShortcut(tr("Ctrl+I"));
	connect(cannyAndSIFTAct, SIGNAL(triggered()), this, SLOT(cannyAndSIFT()));

	/*measureMBaryAct = new QAction(tr("&Measure manual centroid"), this);
	measureMBaryAct->setEnabled(false);
	connect(measureMBaryAct, SIGNAL(triggered()), this, SLOT(measureMBary()));

	measureEBaryAct = new QAction(tr("&Measure estimated centroid"), this);
	measureEBaryAct->setEnabled(false);
	connect(measureEBaryAct, SIGNAL(triggered()), this, SLOT(measureEBary()));*/

	/*dirAutoLandmarksAct = new QAction(
			tr("Compute automatic landmarks on folder"), this);
	dirAutoLandmarksAct->setEnabled(false);
	connect(dirAutoLandmarksAct, SIGNAL(triggered()), this,
			SLOT(dirAutoLandmarks()));

	dirCentroidMeasureAct = new QAction(tr("Measure centroid on folder"), this);
	dirCentroidMeasureAct->setEnabled(false);
	connect(dirCentroidMeasureAct, SIGNAL(triggered()), this,
			SLOT(dirCentroidMeasure()));

	dirGenerateDataAct = new QAction(tr("Random generated the data"), this);
	dirGenerateDataAct->setEnabled(false);
	dirGenerateDataAct->setShortcut(tr("Ctrl+D"));
	connect(dirGenerateDataAct, SIGNAL(triggered()), this,
			SLOT(dirGenerateData()));*/
}
/*void ImageViewer::createRegistrationMenu()
 {
 icpAct = new QAction(tr("PCAI method"), this);
 icpAct->setEnabled(false);
 icpAct->setShortcut(tr("Ctrl+I"));
 connect(icpAct, SIGNAL(triggered()), this, SLOT(icpMethodViewer()));
 }*/

void ImageViewer::createActions()
{
	createFileMenu();
	createViewMenu();
	createHelpMenu();
	createSegmentationMenu();
	createLandmarksMenu();
	createFilterMenu();
}
void ImageViewer::createMenus()
{
	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(saveAsAct);
	fileMenu->addSeparator();
	fileMenu->addAction(closeAct);
	fileMenu->addAction(exitAct);

	viewMenu = new QMenu(tr("&View"), this);
	viewMenu->addAction(zoomInAct);
	viewMenu->addAction(zoomOutAct);
	viewMenu->addSeparator();
	viewMenu->addAction(normalSizeAct);
	viewMenu->addAction(fitToWindowAct);
	viewMenu->addSeparator();
	viewMenu->addAction(gHistogramAct);
	viewMenu->addAction(rgbHistogramAct);
	viewMenu->addAction(displayMLandmarksAct);
	viewMenu->addAction(displayALandmarksAct);

	helpMenu = new QMenu(tr("&Helps"), this);
	helpMenu->addAction(aboutAct);
	//helpMenu->addAction(testAct);

	segmentationMenu = new QMenu(tr("&Segmentation"), this);
	segmentationMenu->addAction(binaryThresholdAct);
	segmentationMenu->addAction(cannyAct);
	segmentationMenu->addAction(suzukiAct);
	segmentationMenu->addAction(lineSegmentationAct);

	processMenu = new QMenu(tr("&Process"), this);
	QMenu* filterMenu = processMenu->addMenu(tr("Filter"));
	filterMenu->addAction(gauAct);
	filterMenu->addAction(robertAct);
	filterMenu->addAction(sobelAct);
	QMenu* binaryMenu = processMenu->addMenu(tr("Binary"));
	binaryMenu->addAction(erosionAct);
	binaryMenu->addAction(dilationAct);
	binaryMenu->addAction(openBinaryAct);
	binaryMenu->addAction(closeBinaryAct);

	dominantPointMenu = new QMenu(tr("&Landmarks"), this);
	//dominantPointMenu->addAction(phtAct);
	//dominantPointMenu->addAction(phtPointsAct);
	dominantPointMenu->addAction(autoLandmarksAct);
	dominantPointMenu->addAction(pcaiAct);
	dominantPointMenu->addAction(sobelAndSIFTAct);
	dominantPointMenu->addAction(cannyAndSIFTAct);
	//dominantPointMenu->addSeparator();
	//dominantPointMenu->addAction(measureMBaryAct);
	//dominantPointMenu->addAction(measureEBaryAct);
	//dominantPointMenu->addSeparator();
	//QMenu* menuDirectory = dominantPointMenu->addMenu(
	//		tr("Working on directory"));
	//menuDirectory->addAction(dirAutoLandmarksAct);
	//menuDirectory->addAction(dirCentroidMeasureAct);
	//menuDirectory->addAction(dirGenerateDataAct);

	//helpMenu = new QMenu(tr("&Filters"), this);
	//helpMenu->addAction(Gaussian filter);
	//helpMenu->addAction(Median_filter"");

	//registrationMenu = new QMenu(tr("&Registration"), this);
	//registrationMenu->addAction(icpAct);

	// add menus to GUI
	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(viewMenu);
	menuBar()->addMenu(segmentationMenu);
	menuBar()->addMenu(processMenu);
	menuBar()->addMenu(dominantPointMenu);
	//menuBar()->addMenu(registrationMenu);
	menuBar()->addMenu(helpMenu);
}
void ImageViewer::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(saveAct);

	viewToolBar = addToolBar(tr("View"));
	viewToolBar->addAction(zoomInAct);
	viewToolBar->addAction(zoomOutAct);
}
void ImageViewer::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}
void ImageViewer::activeFunction()
{
	openAct->setEnabled(true);
	saveAct->setEnabled(true);
	saveAsAct->setEnabled(true);

	zoomInAct->setEnabled(true);
	zoomOutAct->setEnabled(true);
	fitToWindowAct->setEnabled(true);
	normalSizeAct->setEnabled(true);
	gHistogramAct->setEnabled(true);
	rgbHistogramAct->setEnabled(true);
	displayMLandmarksAct->setEnabled(true);
	displayALandmarksAct->setEnabled(true);
	//if (matImage->get_List_Of_Auto_Landmarks().size() > 0)
		

	binaryThresholdAct->setEnabled(true);
	cannyAct->setEnabled(true);
	suzukiAct->setEnabled(true);
	lineSegmentationAct->setEnabled(true);

	gauAct->setEnabled(true);
	robertAct->setEnabled(true);
	sobelAct->setEnabled(true);
	erosionAct->setEnabled(true);
	dilationAct->setEnabled(true);
	openBinaryAct->setEnabled(true);
	closeBinaryAct->setEnabled(true);

	//phtAct->setEnabled(true);
	//phtPointsAct->setEnabled(true);
	autoLandmarksAct->setEnabled(true);
	//if (matImage->get_List_Of_Manual_Landmarks().size() > 0)
	//	measureMBaryAct->setEnabled(true);
	//if (matImage->get_List_Of_Auto_Landmarks().size() > 0)
	//	measureEBaryAct->setEnabled(true);
	//dirAutoLandmarksAct->setEnabled(true);
	//dirCentroidMeasureAct->setEnabled(true);
	//dirGenerateDataAct->setEnabled(true);

	pcaiAct->setEnabled(true);
	sobelAndSIFTAct->setEnabled(true);
	cannyAndSIFTAct->setEnabled(true);

	//testAct->setEnabled(true);

	viewMenuUpdateActions();
	if (!fitToWindowAct->isChecked())
		imageLabel->adjustSize();
}

ImageViewer::ImageViewer()
{
	imageLabel = new QLabel;
	imageLabel->setBackgroundRole(QPalette::Base);
	imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	imageLabel->setScaledContents(true);

	scrollArea = new QScrollArea;
	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(imageLabel);
	setCentralWidget(scrollArea);

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	setWindowTitle(tr(".: MAELab :."));
	resize(900, 700);

	setWindowIcon(QIcon("./ui/resources/ico/logo.ico"));
	//parameterPanel = NULL;

}

ImageViewer::~ImageViewer()
{
}
void ImageViewer::closeEvent(QCloseEvent *event)
{
	QMessageBox::StandardButtons ret;
	ret = QMessageBox::warning(this, tr("Warning"),
			tr("Do you want exit the program?"),
			QMessageBox::Yes | QMessageBox::No);
	bool cl = true;
	if (ret == QMessageBox::Yes)
		cl = true;
	else if (ret == QMessageBox::No)
		cl = false;
	if (cl)
		event->accept();
	else
		event->ignore();
}
void ImageViewer::loadImage(QString fn)
{

	matImage = new Image(fn.toStdString());
	qImage.load(fn);

	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	scaleFactor = 1.0;

	saveAsAct->setEnabled(true);
	activeFunction();

	this->fileName = fn;
	setWindowTitle(tr("Image Viewer - ") + fileName);
	statusBar()->showMessage(tr("File loaded"), 2000);
}
void ImageViewer::loadImage(Image *_matImage, QImage _qImage, QString tt)
{
	matImage = _matImage;
	qImage = _qImage;
	imageLabel->setPixmap(QPixmap::fromImage(qImage));
	scaleFactor = 1.0;

	saveAct->setEnabled(true);
	activeFunction();

	setWindowTitle(tr("Image Viewer - ") + tt);
	statusBar()->showMessage(tr("Finished"), 2000);
}

void ImageViewer::displayLandmarks(Image *image, vector<Point> lms, RGB color)
{
	Point lm;
	Matrix<RGB> imgWithLM = image->get_RGB_Matrix();
	for (size_t i = 0; i < lms.size(); i++)
	{
		lm = lms.at(i);
		cout << "\nManual landmark: " << lm.get_X() << "\t" << lm.get_Y();
		imgWithLM = fill_Circle(imgWithLM, lm, 3, color);
	}
	image->set_RGB_Matrix(imgWithLM);
}
// =========================== Slots =======================================
void ImageViewer::about()
{
	QMessageBox::about(this, tr("About MAELab"),
			tr(
					"<p><b>MAELab</b> is a software in Computer Vision. It provides the functions for "
							"segmentation and landmarks detection on beetle's anatomical images.</p>"));
}

void ImageViewer::open()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
			QDir::currentPath());
	cout << fileName.toStdString() << endl;
	if (!fileName.isEmpty())
	{
		QImage image(fileName);
		if (image.isNull())
		{
			QMessageBox::information(this, tr("MAELab"),
					tr("Cannot load %1.").arg(fileName));
			return;
		}
		if (!this->fileName.isEmpty())
		{
			ImageViewer* other = new ImageViewer;
			other->loadImage(fileName);
			other->move(x() + 40, y() + 40);
			other->show();
		}
		else
		{
			this->loadImage(fileName);
		}
	}
}
void ImageViewer::save()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save image"), ".",
			tr("Image Files (*.jpg)"));
	if (fileName.isEmpty())
	{
		cout << "\nCan not save the image !!";
		return;
	}
	QApplication::setOverrideCursor(Qt::WaitCursor);
	qImage.save(fileName, "JPEG");

	QApplication::restoreOverrideCursor();

	this->fileName = fileName;
	setWindowTitle(tr("MAELab - ") + fileName);

	saveAct->setEnabled(false);
	saveAsAct->setEnabled(true);

	statusBar()->showMessage(tr("File saved"), 2000);
	return;
}
void ImageViewer::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save as image"),
			".", tr("Image Files (*.jpg)"));
	if (fileName.isEmpty())
	{
		cout << "\nCan not save the image !!";
		return;
	}
	QApplication::setOverrideCursor(Qt::WaitCursor);
	qImage.save(fileName, "JPEG");
	QApplication::restoreOverrideCursor();

	saveAct->setEnabled(true);

	statusBar()->showMessage(tr("File saved"), 2000);
	return;
}
void ImageViewer::zoomIn()
{
	scaleImage(1.25);
}
void ImageViewer::zoomOut()
{
	scaleImage(0.8);
}
void ImageViewer::normalSize()
{
	imageLabel->adjustSize();
	scaleFactor = 1.0;
}
void ImageViewer::fitToWindow()
{

	bool fitToWindow = fitToWindowAct->isChecked();
	scrollArea->setWidgetResizable(fitToWindow);
	if (!fitToWindow)
	{
		normalSize();
	}
	viewMenuUpdateActions();
}


void ImageViewer::gScaleHistogram()
{
	cout << "\n Gray scale histogram." << endl;
	ptr_IntMatrix histogram = mae_get_Gray_Histogram(matImage);
	int max = -1;
	for (int c = 0; c < histogram->get_Cols(); c++)
	{
		if (histogram->get_At_Position(0, c) > max)
			max = histogram->get_At_Position(0, c);
	}

	RGB color;
	ptr_RGBMatrix hDisplay = new Matrix<RGB>(240, 300, color);
	Point pbegin, pend;
	color = 255;
	for (int c = 0; c < histogram->get_Cols(); c++)
	{
		pbegin.set_X(c);
		pbegin.set_Y(239);
		
		double newR = 0
				+ (histogram->get_At_Position(0, c) - 0) * (239 - 0) / (max - 0);
		pend.set_X(c);
		pend.set_Y(239 - newR);

		*hDisplay = drawing_Line(*hDisplay, Line(pbegin, pend), color);
	}
	delete histogram;

	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptr_RGB_To_QImage(*hDisplay), "Histogram result");
	other->move(x() - 40, y() - 40);
	other->show();

	delete hDisplay;

	cout << "\nFinished." << endl;
}

void ImageViewer::rgbHistogramCalc()
{
	cout << "\n RGB histogram." << endl;

	ptr_RGBMatrix histogram = mae_get_RGB_Histogram(matImage);
	double maxR = -1, maxG = -1, maxB = -1;
	for (int c = 0; c < histogram->get_Cols(); c++)
	{
		RGB color = histogram->get_At_Position(0, c);
		if (color.get_R() > maxR)
			maxR = color.get_R();
		if (color.get_G() > maxG)
			maxG = color.get_G();
		if (color.get_B() > maxB)
			maxB = color.get_B();
	}

	RGB color;
	ptr_RGBMatrix redDisplay = new Matrix<RGB>(240, 300, color);
	ptr_RGBMatrix greenDisplay = new Matrix<RGB>(240, 300, color);
	ptr_RGBMatrix blueDisplay = new Matrix<RGB>(240, 300, color);

	Point pbegin, pend;
	for (int c = 0; c < histogram->get_Cols(); c++)
	{
		RGB cvalue = histogram->get_At_Position(0, c);
		pbegin.set_X(c);
		pbegin.set_Y(239);
		pend.set_X(c);
		pend.set_Y(239 - ((double) ((cvalue.get_R() * 239) / maxR)));
		color= 0;
		color.set_R(255);
		
		*redDisplay = drawing_Line(*redDisplay, Line(pbegin, pend), color);

		pend.set_X(c);
		pend.set_Y(239 - ((double) ((cvalue.get_G() * 239) / maxG)));
		color= 0;
		color.set_G(255);
		
		*greenDisplay = drawing_Line(*greenDisplay, Line(pbegin, pend), color);

		pend.set_X(c);
		pend.set_Y(239 - ((double) ((cvalue.get_B() * 239) / maxB)));
		color= 0;
		color.set_B(255);
		
		*blueDisplay = drawing_Line(*blueDisplay, Line(pbegin, pend), color);
	}
	delete histogram;

	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptr_RGB_To_QImage(*redDisplay),
			"RGB Histogram result - Red channel");
	other->move(x() - 40, y() - 40);
	other->show();

	ImageViewer *other2 = new ImageViewer;
	other2->loadImage(matImage, ptr_RGB_To_QImage(*greenDisplay),
			"RGB Histogram result - Green channel");
	other2->move(x() - 60, y() - 60);
	other2->show();

	ImageViewer *other3 = new ImageViewer;
	other3->loadImage(matImage, ptr_RGB_To_QImage(*blueDisplay),
			"RGB Histogram result - Blue Channel");
	other3->move(x() - 80, y() - 80);
	other3->show();

	delete redDisplay;
	delete greenDisplay;
	delete blueDisplay;

	cout << "\nFinished." << endl;
}

void ImageViewer::displayManualLandmarks()
{
	cout << "\n Display the manual landmarks.\n";

	bool currentState = displayMLandmarksAct->isChecked();
	if (currentState)
	{
		if (matImage->get_List_Of_Manual_Landmarks().size() <= 0)
		{
			QMessageBox msgbox;
			msgbox.setText("Select the manual landmarks file.");
			msgbox.exec();

			QString reflmPath = QFileDialog::getOpenFileName(this);
			matImage->read_Manual_Landmarks(reflmPath.toStdString());

		}
		vector<Point> mLandmarks = matImage->get_List_Of_Manual_Landmarks();
		RGB color(255,0,0);
		displayLandmarks(matImage, mLandmarks, color);
		displayMLandmarksAct->setChecked(true);
		//measureMBaryAct->setEnabled(true);
	}
	else
	{
		Image *img = new Image(fileName.toStdString());
		matImage->set_RGB_Matrix(img->get_RGB_Matrix());
		displayMLandmarksAct->setChecked(false);
	}

	if (displayALandmarksAct->isChecked())
	{
		vector<Point> aLM = matImage->get_List_Of_Auto_Landmarks();
		if (aLM.size() > 0)
		{
			RGB color;
			color.set_R(255);
			color.set_G(255);
			displayLandmarks(matImage, aLM, color);
			displayALandmarksAct->setChecked(true);
		}
	}

	this->loadImage(matImage, ptr_RGB_To_QImage(matImage->get_RGB_Matrix()),
			"Display manual landmarks.");
	this->show();
	cout << "\nFinish.\n";
}

void ImageViewer::displayAutoLandmarks()
{
	vector<Point> autoLM = matImage->get_List_Of_Auto_Landmarks();
	QMessageBox message;
	if (autoLM.size() <= 0)
	{
		message.setText(
				"Automatic landmarks do not exists. You need to compute them.");
		message.exec();
		displayALandmarksAct->setChecked(false);
	}
	else
	{
		bool currentState = displayALandmarksAct->isChecked();
		if (currentState)
		{
			RGB color(255,255,0);
			displayLandmarks(matImage, autoLM, color);
			displayALandmarksAct->setChecked(true);
		}
		else
		{
			Image *img = new Image(fileName.toStdString());
			matImage->set_RGB_Matrix(img->get_RGB_Matrix());
			displayALandmarksAct->setChecked(false);
		}

		if (displayMLandmarksAct->isChecked())
		{
			vector<Point> mLM = matImage->get_List_Of_Manual_Landmarks();
			if (mLM.size() > 0)
			{
				RGB color(255,0,0);
				displayLandmarks(matImage, mLM, color);
				displayMLandmarksAct->setChecked(true);
			}
		}
		this->loadImage(matImage, ptr_RGB_To_QImage(matImage->get_RGB_Matrix()),
				"Display estimated landmarks.");
	}

}

void ImageViewer::binThreshold()
{
	cout << "\nBinary thresholding...\n";
	ptr_IntMatrix rsMatrix = mae_Binary_Threshold(matImage);
	//rsMatrix = postProcess(rsMatrix, 255);

	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptr_Int_To_QImage(*rsMatrix),
			"Thresholding result");
	other->show();

}

void ImageViewer::cannyAlgorithm()
{
	cout << "\nCanny Algorithm...\n";

	vector<Point> cPoints = mae_Canny_Algorithm(matImage);

	RGB color(255,0,0);
	Point pi;
	cout << endl << "Number of points: " << cPoints.size() << endl;
	Matrix<RGB> sImage = matImage->get_RGB_Matrix();
	for (size_t i = 0; i < cPoints.size(); i++)
	{
		pi = cPoints.at(i);
		sImage.set_At_Position(pi.get_Y(), pi.get_X(), color);
	}

	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptr_RGB_To_QImage(sImage), "Canny result");
	other->move(x() - 40, y() - 40);
	other->show();
}

void ImageViewer::suzukiAlgorithm()
{
	cout << "\nSuzuki Algorithm...\n";
	vector<Edge> edges = mae_Suzuki_Algorithm(matImage);
	
	RGB color(255,0,0);
	Edge edgei;
	Point pi;
	
	Matrix<RGB> sImage = matImage->get_RGB_Matrix();
	for (size_t i = 0; i < edges.size(); i++)
	{
		edgei = edges.at(i);
		for (size_t k = 0; k < edgei.get_Points().size(); k++)
		{
			pi = edgei.get_Points().at(k);
			sImage.set_At_Position(pi.get_Y(), pi.get_X(), color);
		}
	}
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptr_RGB_To_QImage(sImage), "Suzuki result");
	other->move(x() - 40, y() - 40);
	other->show();
}

void ImageViewer::lineSegmentation()
{
	cout << "\nDisplay the list of lines." << endl;
	vector<Line> listOfLines = mae_Line_Segment(matImage);
	RGB color(255, 0, 0);
	Line linei;
	cout << "\nBegin draw the lines...\n";
	Matrix<RGB> sImage = matImage->get_RGB_Matrix();
	for (size_t i = 0; i < listOfLines.size(); i++)
	{
		linei = listOfLines.at(i);
		sImage = drawing_Line(sImage, linei, color);
	}

	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptr_RGB_To_QImage(sImage),
			"Line segmentation result");
	other->move(x() - 40, y() - 40);
	other->show();
}

// ==================================================== Filter menu =========================================
void ImageViewer::gauFilter()
{
	cout << "\n Gaussian filter." << endl;
	Matrix<double> kernel = get_Gaussian_Kernel(3, 1);
	Matrix<RGB> gsResult = mae_Gaussian_Filter(matImage, kernel);
	
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptr_RGB_To_QImage(gsResult),
			"Gaussian filter result");
	other->move(x() - 40, y() - 40);
	other->show();
}

void ImageViewer::robertFilter()
{
	cout << "\n Robert filter." << endl;
	
	Matrix<int> rbResult = mae_Robert_Filter(matImage);
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptr_Int_To_QImage(rbResult),
			"Robert filter result");
	other->move(x() - 40, y() - 40);
	other->show();
}

void ImageViewer::sobelFilter()
{
	cout << "\n Sobel filter." << endl;
	
	Matrix<double> sbResult = mae_Sobel_Filter(matImage);
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptr_Double_To_QImage(sbResult), "Sobel filter result");
	other->move(x() - 40, y() - 40);
	other->show();
}

void ImageViewer::erosionOperation()
{
	cout << "\n Erosion operation." << endl;
	
	Matrix<int> erResult = mae_Erode(matImage);
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptr_Int_To_QImage(erResult),
			"Erosion operation result");
	other->move(x() - 40, y() - 40);
	other->show();
}

void ImageViewer::dilationOperation()
{
	cout << "\n Dilation operation." << endl;
	
	Matrix<int> dlResult = mae_Dilate(matImage);

	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptr_Int_To_QImage(dlResult),
			"Dilation operation result");
	other->move(x() - 40, y() - 40);
	other->show();
}
void ImageViewer::openOperation()
{
	cout << "\n Open operation." << endl;
	
	Matrix<int> dlResult = mae_Open_Binary(matImage);
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptr_Int_To_QImage(dlResult),
			"Open operation result");
	other->move(x() - 40, y() - 40);
	other->show();
}

void ImageViewer::closeOperation()
{
	cout << "\n Close operation." << endl;
	
	Matrix<int> clResult = mae_Close_Binary(matImage);
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, ptr_Int_To_QImage(clResult),
			"Close operation result");
	other->move(x() - 40, y() - 40);
	other->show();
}

// ======================================================= Landmarks points ===============================
/*void ImageViewer::gHoughTransform()
{
	cout << "\n Generalizing hough transform." << endl;
	QMessageBox msgbox;

	// extract list of points of model image by using canny
	msgbox.setText("Select the model image.");
	msgbox.exec();
	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	cout << endl << fileName2.toStdString() << endl;
	Image *modelImage = new Image(fileName2.toStdString());

	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();
	QString reflmPath = QFileDialog::getOpenFileName(this);
	modelImage->read_Manual_Landmarks(reflmPath.toStdString());
	
	ProHoughTransform tr;
	tr.set_Ref_Image(*modelImage);

	Point ePoint, mPoint;
	//double angleDiff;
	//==================================================================
	LandmarkDetection lmDetect;
	lmDetect.set_Ref_Image(*modelImage);
	//vector<Point> estLandmarks = lmDetect.landmarksAutoDectect2(*matImage, 9, 36);
	vector<Point> estLandmarks = lmDetect.landmarksWithSIFT(*matImage, 9, 36);
	cout << "\nNumber of the landmarks: " << estLandmarks.size() << endl;
	// drawing...
	Point lm;
	QPainter qpainter(&qImage);
	qpainter.setPen(Qt::yellow);
	qpainter.setBrush(Qt::yellow);
	qpainter.setFont(QFont("Arial", 15));
	for (size_t i = 0; i < estLandmarks.size(); i++)
	{
		lm = estLandmarks.at(i);
		cout << "Landmarks " << i + 1 << ":\t" << lm.get_X() << "\t" << lm.get_Y()
				<< endl;		
		if (lm.get_X() >= 0 && lm.get_X() < matImage->get_RGB_Matrix().get_Cols()
				&& lm.get_Y() >= 0
				&& lm.get_Y() < matImage->get_RGB_Matrix().get_Rows())
		{
			qpainter.drawEllipse(lm.get_X(), lm.get_Y(), 4, 4);
			qpainter.drawText(lm.get_X() + 6, lm.get_Y(),
					QString::number((int) i));
		}
	}
	qpainter.end();
	Point ebary;
	double mCentroid = measure_Centroid_Point(estLandmarks, ebary);
	msgbox.setText(
			"<p>Coordinate of bary point: (" + QString::number(ebary.get_X())
					+ ", " + QString::number(ebary.get_Y()) + ")</p>"
							"<p>Centroid value: " + QString::number(mCentroid)
					+ "</p");
	msgbox.exec();

	this->loadImage(matImage, qImage, "Landmarks result");
	this->show();
	msgbox.setText("Finish.");
	msgbox.exec();

}*/

void ImageViewer::extractLandmarks()
{
	cout << "\n Automatic extraction the landmarks.\n";
	QMessageBox msgbox;
	msgbox.setText("Select the model image.");
	msgbox.exec();

	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	cout << endl << fileName2.toStdString() << endl;
	Image *modelImage = new Image(fileName2.toStdString());

	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();
	QString reflmPath = QFileDialog::getOpenFileName(this);
	modelImage->read_Manual_Landmarks(reflmPath.toStdString());

	Point ePoint;
	double angleDiff;
	vector<Point> lms = mae_PHT_Method(*modelImage,reflmPath.toStdString(),*matImage,Degree,500, 400, 500, ePoint, angleDiff);



	/*LandmarkDetection tr;
	tr.set_Ref_Image(*modelImage);
	vector<Point> lms = tr.landmarksAutoDectect(*matImage, Degree, 500, 400,
			500, ePoint, angleDiff);
	cout << "\nNumber of the landmarks: " << lms.size() << endl;
	RGB color(255, 255, 0);
	Point lm;*/
	matImage->rotate(ePoint, angleDiff, 1);
	/*Matrix<RGB> rgbResult;
	for (size_t i = 0; i < lms.size(); i++)
	{
		lm = lms.at(i);
		rgbResult = drawing_Circle(matImage->get_RGB_Matrix(), lm, 5, color);
	}*/
	Point lm;
	qImage = ptr_RGB_To_QImage(matImage->get_RGB_Matrix());
	QPainter qpainter(&qImage);
	qpainter.setPen(Qt::red);
	qpainter.setBrush(Qt::red);
	qpainter.setFont(QFont("Arial", 10));
	for (size_t i = 0; i < lms.size(); i++)
	{
		lm = lms.at(i);
		cout << "Landmarks " << i + 1 << ":\t" << lm.get_X() << "\t" << lm.get_Y()
				<< endl;
		//fill_Circle(matImage->get_RGB_Matrix(), lm, 3, color);
		qpainter.drawEllipse(lm.get_X(), lm.get_Y(), 5, 5);
		qpainter.drawText(lm.get_X() + 6, lm.get_Y(), QString::number((int) i));
	}
	qpainter.end();


	matImage->set_Auto_Landmarks(lms);
	displayALandmarksAct->setEnabled(true);
	displayALandmarksAct->setChecked(true);
	//this->loadImage(matImage, ptr_RGB_To_QImage(matImage->get_RGB_Matrix()),
	//		"Landmarks result");
	//this->show();
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, qImage,"Palaniswamy method");
	other->move(x() - 40, y() - 40);
	other->show();

	msgbox.setText("Finish");
	msgbox.exec();
}
/*
void ImageViewer::measureMBary()
{
	QMessageBox qmessage;
	vector<Point> mLandmarks = matImage->getListOfManualLandmarks();
	if (mLandmarks.size() > 0)
	{
		Point ebary(0, 0);
		double mCentroid = measureCentroidPoint(mLandmarks, ebary);

		qmessage.setText(
				"<p>Coordinate of bary point: (" + QString::number(ebary.get_X())
						+ ", " + QString::number(ebary.get_Y()) + ")</p>"
								"<p>Centroid value: "
						+ QString::number(mCentroid) + "</p");
	}
	else
	{
		qmessage.setText("The image has not the manual landmarks.");
	}
	qmessage.exec();
}
void ImageViewer::measureEBary()
{
	QMessageBox qmessage;
	vector<Point> mLandmarks = matImage->getListOfAutoLandmarks();
	if (mLandmarks.size() > 0)
	{
		Point ebary(0, 0);
		double mCentroid = measureCentroidPoint(mLandmarks, ebary);

		qmessage.setText(
				"<p>Coordinate of bary point: (" + QString::number(ebary.get_X())
						+ ", " + QString::number(ebary.get_Y()) + ")</p>"
								"<p>Centroid value: "
						+ QString::number(mCentroid) + "</p");
	}
	else
	{
		qmessage.setText("The image has not the automatic landmarks.");
	}
	qmessage.exec();
}
void ImageViewer::dirAutoLandmarks()
{
	cout << "\n Automatic estimated landmarks on directory." << endl;
	QMessageBox msgbox;

	msgbox.setText("Select the model's landmarks file");
	msgbox.exec();
	QString lpath = QFileDialog::getOpenFileName(this);
	matImage->readManualLandmarks(lpath.toStdString());

	msgbox.setText("Selecte the scene images folder.");
	msgbox.exec();
	QString folder = QFileDialog::getExistingDirectory(this);

	msgbox.setText("Selecte the saving folder.");
	msgbox.exec();
	QString savefolder = QFileDialog::getExistingDirectory(this);

	vector < string > fileNames = readDirectory(folder.toStdString().c_str());
	Point pk;
	vector<Point> esLandmarks;
	Point ePoint(0, 0);
	double angleDiff = 0;
	LandmarkDetection tr;
	tr.setRefImage(*matImage);

	string fileName;
	for (size_t i = 0; i < fileNames.size(); i++)
	{
		fileName = folder.toStdString() + "/" + fileNames.at(i);
		cout << "\n" << fileName << endl;
		Image sceneimage(fileName);

		esLandmarks = tr.landmarksAutoDectect(sceneimage, Degree, 500, 400, 500,
				ePoint, angleDiff);
		if (savefolder != NULL || savefolder != "")
		{
			string saveFile = savefolder.toStdString() + "/" + fileNames.at(i)
					+ ".TPS";
			ofstream inFile(saveFile.c_str());
			inFile << "LM=" << esLandmarks.size() << "\n";
			for (size_t k = 0; k < esLandmarks.size(); k++)
			{
				pk = esLandmarks.at(k);
				inFile << pk.get_X() << "\t" << pk.get_Y() << "\n";
			}
			inFile << "IMAGE=" << saveFile << "\n";
			inFile.close();
		}
	}

	msgbox.setText("Finish");
	msgbox.exec();

}
void ImageViewer::dirCentroidMeasure()
{
	cout << "\n Compute centroid on directory." << endl;

	QMessageBox qmessage;
	qmessage.setText("Select the landmarks folder.");
	qmessage.exec();

	QString lmfolder = QFileDialog::getExistingDirectory(this);

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), ".",
			tr("Measure file (*.txt)"));

	vector < string > fileNames = readDirectory(lmfolder.toStdString().c_str());
	string saveFile;
	ofstream inFile;
	if (fileName != NULL || fileName != "")
	{
		string saveFile = fileName.toStdString();
		inFile.open(saveFile.c_str(), std::ofstream::out);
	}
	for (size_t i = 0; i < fileNames.size(); i++)
	{
		string filePath = lmfolder.toStdString() + "/" + fileNames.at(i);
		//vector<Point> mLandmarks = readTPSFile(filePath.c_str());

		//matImage->setMLandmarks(filePath);
		vector<Point> mLandmarks = matImage->readManualLandmarks(filePath);
		if (mLandmarks.size() > 0)
		{
			Point ebary(0, 0);
			double mCentroid = 0;
			mCentroid = measureCentroidPoint(mLandmarks, ebary);

			if (fileName != NULL || fileName != "")
			{
				inFile << fileNames.at(i) << "\t" << ebary.get_X() << "\t"
						<< ebary.get_Y() << "\t" << mCentroid << "\n";

			}
		}
		else
		{
			if (fileName != NULL || fileName != "")
			{
				inFile << fileNames.at(i) << "\t" << 0 << "\t" << 0 << "\t" << 0
						<< "\n";

			}
		}
		mLandmarks.clear();
	}
	inFile.close();

	qmessage.setText("Finish.");
	qmessage.exec();
}

// GHT with points
void ImageViewer::dirGenerateData()
{
	cout << "\n Automatic generate data on directory." << endl;
	QMessageBox msgbox;

	string imageFolder = "/home/linh/Desktop/editedImages/md_images/size1";
	//string imageFolder = "/home/linh/Desktop/rotatedImages/mg";
	string lmFolder = "/home/linh/Desktop/editedImages/md_landmarks/size1";
	string saveFolder = "/home/linh/Desktop/results/2017/md/6mars";
	vector < string > images = readDirectory(imageFolder.c_str());
	vector < string > lms = readDirectory(lmFolder.c_str());
	int nrandom = 0;
	string model;
	string sceneName;
	string lmFile;

	Point pk;
	vector<Point> esLandmarks;
	Point ePoint(0, 0);
	LandmarkDetection tr;
	//for (int i = 0; i < 21; i++)
	//{ // run on 20 images
	//nrandom = random(0, (int) images.size());
	nrandom = 41; // use Md 028 as ref
	string modelName = images.at(nrandom);
	cout << "\n Random and model: " << nrandom << "\t" << modelName << endl;
	model = imageFolder + "/" + images.at(nrandom);
	lmFile = lmFolder + "/" + lms.at(nrandom);
	matImage = new Image(model);
	matImage->readManualLandmarks(lmFile);
	tr.setRefImage(*matImage);
	//tr.landmarksOnDir2(modelName, imageFolder, images, saveFolder);
	tr.landmarksOnDir4(modelName, imageFolder, images, saveFolder, lmFolder,
			lms);
	//}

	msgbox.setText("Finish");
	msgbox.exec();
}
*/
// ================================================ Registration ========================================
void ImageViewer::pcaiMethodViewer()
{
	cout << "\nLandmarks with image registration" << endl;
	QMessageBox msgbox;

	// extract list of points of model image by using canny
	msgbox.setText("Select the model image.");
	msgbox.exec();
	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	cout << endl << fileName2.toStdString() << endl;
	Image *modelImage = new Image(fileName2.toStdString());
	//string lmfile = "/media/linh/Data/PhD/Dataset/mg/landmarks/Mg 025.TPS";
	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();
	QString reflmPath = QFileDialog::getOpenFileName(this);
	//modelImage->read_Manual_Landmarks(reflmPath.toStdString());
	//vector<Point> estLandmarks = PCAI(*modelImage, *matImage,
	//		modelImage->get_List_Of_Manual_Landmarks());
	vector<Point> estLandmarks = mae_PCAI_Method(*modelImage, 
			reflmPath.toStdString(),*matImage);
	cout << "\nNumber of the landmarks: " << estLandmarks.size() << endl;

	//RGB color(255,0,0);
	
	// display the result
	Point lm;
	QPainter qpainter(&qImage);
	qpainter.setPen(Qt::red);
	qpainter.setBrush(Qt::red);
	qpainter.setFont(QFont("Arial", 10));
	for (size_t i = 0; i < estLandmarks.size(); i++)
	{
		lm = estLandmarks.at(i);
		cout << "Landmarks " << i + 1 << ":\t" << lm.get_X() << "\t" << lm.get_Y()
				<< endl;
		//fill_Circle(matImage->get_RGB_Matrix(), lm, 3, color);
		qpainter.drawEllipse(lm.get_X(), lm.get_Y(), 5, 5);
		qpainter.drawText(lm.get_X() + 6, lm.get_Y(), QString::number((int) i));
	}
	qpainter.end();
	Point ebary;
	double mCentroid = measure_Centroid_Point(estLandmarks, ebary);
	msgbox.setText(
			"<p>Coordinate of bary point: (" + QString::number(ebary.get_X())
					+ ", " + QString::number(ebary.get_Y()) + ")</p>"
							"<p>Centroid value: " + QString::number(mCentroid)
					+ "</p");
	msgbox.exec();

	// working on folder

	//Matrix<RGB> saveData = matImage->get_RGB_Matrix();
	//save_RGB("result.jpg", &saveData);
	//this->loadImage(matImage, qImage, "PCAI result");
	//this->show();
	//delete newScene;
	delete modelImage;
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, qImage,"PCAI result");
	other->move(x() - 40, y() - 40);
	other->show();
	//msgbox.setText("Finish.");
	//msgbox.exec();
}

void ImageViewer::sobelAndSIFT()
{
	cout << "\nLandmarks with contours (Sobel) and SIFT" << endl;
	QMessageBox msgbox;

	// extract list of points of model image by using canny
	msgbox.setText("Select the model image.");
	msgbox.exec();
	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	cout << endl << fileName2.toStdString() << endl;
	Image *modelImage = new Image(fileName2.toStdString());
	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();
	QString reflmPath = QFileDialog::getOpenFileName(this);
	//modelImage->read_Manual_Landmarks(reflmPath.toStdString());

	// get the contours in the scene image
	/*Matrix<double> kernel = get_Gaussian_Kernel(5, 2);
	Matrix<int> gsResult = gaussian_Blur(matImage->get_Gray_Matrix(), kernel);
	Matrix<int> sbResult = Sobel_Operation(&gsResult);
	int tValue = 6; //thresholdOtsu(sbResult);
	cout << "\n Otsu threshold: " << tValue << endl;
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
	cout << "\nManual landmarks: "
			<< modelImage->get_List_Of_Manual_Landmarks().size() << endl;
	Matrix<int> modelData = modelImage->get_Gray_Matrix();
	Matrix<int> matData = matImage->get_Gray_Matrix();
	vector<Point> estLandmarks = verify_Descriptors_3(&modelData, &matData,
			contours, modelImage->get_List_Of_Manual_Landmarks(), 9);*/
	vector<Point> estLandmarks = mae_Sobel_SIFT_Method(*modelImage, reflmPath.toStdString(),*matImage,9);
	cout << "\nNumber of estimated landmarks: " << estLandmarks.size() << endl;

	Point lm;
	QPainter qpainter(&qImage);
	qpainter.setPen(Qt::blue);
	qpainter.setBrush(Qt::red);
	qpainter.setFont(QFont("Arial", 10));
	for (size_t i = 0; i < estLandmarks.size(); i++)
	{
		lm = estLandmarks.at(i);
		cout << "Landmarks " << i + 1 << ":\t" << lm.get_X() << "\t" << lm.get_Y()
				<< endl;
		qpainter.drawEllipse(lm.get_X(), lm.get_Y(), 7, 7);
		qpainter.drawText(lm.get_X() + 6, lm.get_Y(), QString::number((int) i));
	}
	qpainter.end();

	delete modelImage;
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, qImage, "Sobel and SIFT");
	other->move(x() - 40, y() - 40);
	other->show();
}

void ImageViewer::cannyAndSIFT()
{
	cout << "\nLandmarks with contours (Sobel) and SIFT" << endl;
	QMessageBox msgbox;

	// extract list of points of model image by using canny
	msgbox.setText("Select the model image.");
	msgbox.exec();
	QString fileName2 = QFileDialog::getOpenFileName(this);
	if (fileName2.isEmpty())
		return;
	cout << endl << fileName2.toStdString() << endl;
	Image *modelImage = new Image(fileName2.toStdString());
	msgbox.setText("Select the landmark file of model image.");
	msgbox.exec();
	QString reflmPath = QFileDialog::getOpenFileName(this);

	/*modelImage->read_Manual_Landmarks(reflmPath.toStdString());
	vector<Point> cPoints = mae_Canny_Algorithm(matImage);
	cout << "\nNumber of points on contours: " << cPoints.size() << endl;
	Matrix<int> modelGrayData = modelImage->get_Gray_Matrix();
	Matrix<int> sceneGrayData = matImage->get_Gray_Matrix();
	vector<Point> estLandmarks = verify_Descriptors_4(&modelGrayData,
			&sceneGrayData, cPoints, modelImage->get_List_Of_Manual_Landmarks(),
			63);*/
	vector<Point> estLandmarks = mae_Canny_SIFT_Method(*modelImage,reflmPath.toStdString(),*matImage,63);
	cout << "\nNumber of estimated landmarks: " << estLandmarks.size() << endl;
	/*RGB color(255,0,0);
	Point lm;
	for (size_t i = 0; i < estLandmarks.size(); i++)
	{
		lm = estLandmarks.at(i);
		matImage->set_RGB_Matrix(fill_Circle(matImage->get_RGB_Matrix(), lm, 7, color));
	}*/

	Point lm;
	QPainter qpainter(&qImage);
	qpainter.setPen(Qt::blue);
	qpainter.setBrush(Qt::red);
	qpainter.setFont(QFont("Arial", 10));
	for (size_t i = 0; i < estLandmarks.size(); i++)
	{
		lm = estLandmarks.at(i);
		cout << "Landmarks " << i + 1 << ":\t" << lm.get_X() << "\t" << lm.get_Y()
				<< endl;
		qpainter.drawEllipse(lm.get_X(), lm.get_Y(), 7, 7);
		qpainter.drawText(lm.get_X() + 6, lm.get_Y(), QString::number((int) i));
	}
	qpainter.end();

	delete modelImage;
	ImageViewer *other = new ImageViewer;
	other->loadImage(matImage, qImage, "Canny and SIFT");
	other->move(x() - 40, y() - 40);
	other->show();
}
