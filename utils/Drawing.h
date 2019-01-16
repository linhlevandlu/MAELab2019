/*
 * Drawing.h
 *
 *  Created on: Dec 18, 2016
 *      Author: linh
 */

#ifndef DRAWING_H_
#define DRAWING_H_
#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;
#include "../imageModel/Point.h"
#include "../imageModel/Line.h"
#include "../imageModel/Matrix.h"

vector<Point> detect_Line(Line line);

Matrix<RGB> drawing_Circle(Matrix<RGB> mat, Point center, int radius, RGB color);
Matrix<RGB> fill_Circle(Matrix<RGB> mat, Point center, int radius, RGB color);
Matrix<RGB> drawing_Line(Matrix<RGB> mat, Line line, RGB color);
#endif /* DRAWING_H_ */
