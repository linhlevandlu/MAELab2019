/*
 * ImageConvert.h
 *
 *  Created on: Dec 19, 2016
 *      Author: linh
 */

#ifndef IMAGECONVERT_H_
#define IMAGECONVERT_H_
#include <QtGui/qimage.h>
#include "../imageModel/Point.h"
#include "../imageModel/Matrix.h"

QImage ptr_Int_To_QImage(Matrix<int> inputMatrix);
QImage ptr_Double_To_QImage(Matrix<double> inputMatrix);
QImage ptr_RGB_To_QImage(Matrix<RGB> inputMatrix);
#endif /* IMAGECONVERT_H_ */
