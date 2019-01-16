/*
 * Converter.h
 *
 *  Created on: Jan 5, 2017
 *      Author: linh
 */

#ifndef CONVERTER_H_
#define CONVERTER_H_
#include <cstdlib>
#include <ctime>
#include <string>
#include <string.h>
#include <cmath>
#include <sstream>
using namespace std;

int random(int min,int max);
string get_Name(string fullPath,string split);
int round_To_Degree(double value);


string number_To_String(int number);
int string_To_Number(const string &text);
#endif /* CONVERTER_H_ */
