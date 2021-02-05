/* 
 * File:   Utilities.h
 * Author: GEII Robot
 *
 * Created on 5 février 2021, 13:39
 */

#ifndef UTILITIES_H
#define	UTILITIES_H

# define PI 3.141592653589793

double Abs1(double value);
double Max1(double value, double value2);
double Min1(double value,double value2);
double LimitToInterval1(double value, double min, double max);
double Modulo2PIAngleRadian(double angleRadian);

float getFloat(unsigned char *p, int index);
double getDouble(unsigned char *p, int index);

void getBytesFromFloat(unsigned char *p, int index, float f);
void getBytesFromDouble(unsigned char *p, int index, double d);
void getBytesFromInt32(unsigned char *p, int index, long in);

#endif	//UTILITIES_H

