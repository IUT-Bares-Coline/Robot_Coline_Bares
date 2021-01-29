/*
 * File:   QEI.c
 * Author: GEII Robot
 *
 * Created on 29 janvier 2021, 16:24
 */
#include "xc.h"

void InitQEI1()
{
QEI1IOCbits.SWPAB = 1 ; //QEAx and QEBx are swapped
QEI1GECL = 0xFFFF;
QEI1GECH = 0xFFFF;
QEI1CONbits.QEIEN = 1 ; // Enable QEI Module
}
void InitQEI2() 
{
QEI2IOCbits.SWPAB = 1 ; //QEAx and QEBx are not swapped
QEI2GECL = 0xFFFF;
QEI2GECH = 0xFFFF;
QEI2CONbits.QEIEN = 1 ; // Enable QEI Module
}

