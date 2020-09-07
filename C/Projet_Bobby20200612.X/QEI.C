/*
 * File:   QEI.C
 * Author: colin
 *
 * Created on 12 juin 2020, 15:09
 */


#include "xc.h"
#include "QEI.h"

int main(void) {
    
    InitQEI1();
    void InitQEI2();
    
    return 0;
}


void InitQEI1()
{
QEI1IOCbits.SWPAB = 1 ; //QEAx and QEBx a r e swapped
QEI1GECL = 0xFFFF;
QEI1GECH = 0xFFFF;
QEI1CONbits.QEIEN = 1 ; // Enable QEI Module
}


void InitQEI2() 
{
QEI2IOCbits.SWPAB = 1 ; //QEAx and QEBx a r e not swapped
QEI2GECL = 0xFFFF;
QEI2GECH = 0xFFFF;
QEI2CONbits.QEIEN = 1 ; // Enable QEI Module
}