 /*
 * File:   QEI.c
 * Author: GEII Robot
 *
 * Created on 29 janvier 2021, 16:24
 */
#include "xc.h"
#include "robot.h"
#include <math.h>
#include "timer.h"
#include "Utilities.h"
#include "UART_Protocol.h"
#define DISTROUES 281.2
#define FREQ_ECH_QEI 250
#define PI 3.141592653589793
#define POSITION_DATA 0x0061


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

float QeiDroitPosition_T_1 , QeiDroitPosition, QeiGauchePosition_T_1, QeiGauchePosition, delta_d, delta_g, delta_theta, dx ;

void QEIUpdateData()
{
//On s au ve g a rde l e s a n ci e n n e s v al e u r s
QeiDroitPosition_T_1 = QeiDroitPosition ;
QeiGauchePosition_T_1 = QeiGauchePosition ;

//On é r a c t u a l i s e l e s v a l e u r s de s p o s i t i o n s
long QEI1RawValue = POS1CNTL;
QEI1RawValue += ( ( long )POS1HLD<<16);
long QEI2RawValue = POS2CNTL;
QEI2RawValue += ( ( long )POS2HLD<<16);

// C onve r si on en mm ( r \ ? e g l \ ? e pour l a t a i l l e de s r o u e s c o d e u s e s )
QeiDroitPosition = 0.01620 * QEI1RawValue ;
QeiGauchePosition = -0.01620 * QEI2RawValue ;

// C al c ul de s d e l t a s de p o s i t i o n
delta_d = QeiDroitPosition - QeiDroitPosition_T_1 ;
delta_g = QeiGauchePosition - QeiGauchePosition_T_1 ;
// d el t a_ t h e t a = atan ( ( delta_d ? del ta_g ) / DISTROUES) ;
delta_theta = ( delta_d - delta_g ) / DISTROUES;
dx = ( delta_d + delta_g ) / 2 ;

// Calcul des vitesses
// attention à remultiplier par la éfrquence dé?chantillonnage
robotState.vitesseDroitFromOdometry = delta_d *FREQ_ECH_QEI;
robotState.vitesseGaucheFromOdometry = delta_g*FREQ_ECH_QEI;
robotState.vitesseLineaireFromOdometry =
(robotState.vitesseDroitFromOdometry + robotState.vitesseGaucheFromOdometry ) / 2 ;
robotState.vitesseAngulaireFromOdometry = delta_theta*FREQ_ECH_QEI;

//Mise à jour du p ositionnment terrain à t?1
robotState.xPosFromOdometry_1 = robotState.xPosFromOdometry ;
robotState.yPosFromOdometry_1 = robotState.yPosFromOdometry ;
robotState.angleRadianFromOdometry_1 = robotState.angleRadianFromOdometry ;

// Calcul des positions dans le  referentiel du terrain
robotState.xPosFromOdometry = robotState.xPosFromOdometry_1 + dx * cos(delta_theta); //????
robotState.yPosFromOdometry = robotState.yPosFromOdometry_1 + dx * sin(delta_theta);//????
robotState.angleRadianFromOdometry = robotState.angleRadianFromOdometry_1 + delta_theta;//????
if(robotState.angleRadianFromOdometry > PI )
    robotState.angleRadianFromOdometry -= 2*PI ;
if(robotState.angleRadianFromOdometry < -PI )
    robotState.angleRadianFromOdometry += 2*PI ;
}


void SendPositionData ()
{
    unsigned char positionPayload[24] ;
    getBytesFromInt32 (positionPayload, 0, timestamp) ;
    getBytesFromFloat (positionPayload, 4, (float) (robotState.xPosFromOdometry)) ;
    getBytesFromFloat (positionPayload, 8, (float) (robotState.yPosFromOdometry)) ;
    getBytesFromFloat (positionPayload, 12, (float) (robotState.angleRadianFromOdometry)) ;
    getBytesFromFloat (positionPayload, 16, (float) (robotState.vitesseLineaireFromOdometry)) ;
    getBytesFromFloat (positionPayload, 20, (float) (robotState.vitesseAngulaireFromOdometry)) ;
    UartEncodeAndSendMessage (POSITION_DATA, 24, positionPayload) ;
}