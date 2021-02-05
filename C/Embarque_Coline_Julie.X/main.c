#include <xc.h>
#include <libpic30.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "Robot.h"
#include "ToolBox.h"
#include "adc.h"
#include "main.h"
#include "UART.h"
#include "CB_TX1.h"
#include "CB_RX1.h"
#include "UART_Protocol.h"

int main(void) {
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();

    /****************************************************************************************************/
    // Configuration des entrées sorties
    /****************************************************************************************************/
    InitIO();

    InitTimer23();
    InitTimer1();
    InitTimer4();
    
    InitPWM();
    
        LED_BLANCHE = 1;
        LED_BLEUE = 1;
        LED_ORANGE = 1;

    InitADC1();
    
    InitUART();
    //void __attribute__((interrupt, no_auto_psv)) _T3Interrupt();
    //robotState.vitesseDroiteConsigne = 30;

    InitQEI1();
    InitQEI2() ;
    
    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    while (1) {
        //LED_BLANCHE=!LED_BLANCHE ;
        //LED_ORANGE=!LED_ORANGE;
        
        //On gère les éventuels byte stockés dans la queue de réception
        if(CB_RX1_IsDataAvailable() > 0)
        {
            char c = CB_RX1_Get();
            UartDecodeMessage(c);
        }

        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [0]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [1]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [2]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;

            if (robotState.distanceTelemetreDroit <= 20) {
                LED_ORANGE = 1;
            } 
            else 
            {
                LED_ORANGE = 0;
            }

            if (robotState.distanceTelemetreCentre <= 20) {
                LED_BLEUE = 1; 
            } else {
                LED_BLEUE =0;
            }

            if (robotState.distanceTelemetreGauche <= 20) {
                LED_BLANCHE = 1;
            } else {
                LED_BLANCHE = 0;
            }            
            unsigned char msgPayload1[] = {robotState.distanceTelemetreGauche, robotState.distanceTelemetreCentre, robotState.distanceTelemetreDroit};
            UartEncodeAndSendMessage(0x0030, 3, msgPayload1);  
                        
            //unsigned char msgPayload[] = {'B', 'o', 'n', 'j', 'o', 'u', 'r',};
            //UartEncodeAndSendMessage(0x0080, 7, msgPayload);
        }
    }    
}// fin main
    

unsigned char stateRobot;
unsigned char automaticMode=1;

void SetRobotState(unsigned char state)
{
    stateRobot=state;
}

void SetRobotAutoControlState(unsigned char controle)
{    
     if(controle==0x31)
        {
         automaticMode = 1;
        }
     else
     {
         automaticMode = 0;
     }
} //le mode automatique/manuel marche a la perfection (youhou)


void OperatingSystemLoop(void) {
    
    if(JACK == 0)
    {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            SendRobotState();
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
            break;
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 500)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            SendRobotState();
            
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            if(automaticMode)
                SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            SendRobotState();
            
            PWMSetSpeedConsigne(20, MOTEUR_DROIT); 
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            if(automaticMode)
                SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            SendRobotState();
            
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            if(automaticMode)
                SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            SendRobotState();            
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-20, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            if(automaticMode)
                SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            SendRobotState();            
            PWMSetSpeedConsigne(-20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS: 
            if(automaticMode)
                SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_RECULE:
            SendRobotState();
            
            PWMSetSpeedConsigne(-20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-20, MOTEUR_GAUCHE);
            stateRobot = STATE_RECULE_EN_COURS;
            break;
        case STATE_RECULE_EN_COURS:
            if(automaticMode)
                SetNextRobotStateInAutomaticMode();
            break;
        
        case STATE_ARRET:
            SendRobotState();
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ARRET_EN_COURS;
            break;
        case STATE_ARRET_EN_COURS:
            if(automaticMode)
                SetNextRobotStateInAutomaticMode();
            break;
        default:
            stateRobot = STATE_ATTENTE;
            break;
            
       
        }
    }       

    } 
//}

void SendRobotState()
{
    unsigned char payload[5];
    payload[0] = stateRobot;
    payload[1] = (unsigned char)(timestamp>>24);
    payload[2] = (unsigned char)(timestamp>>16);
    payload[3] = (unsigned char)(timestamp>>8);
    payload[4] = (unsigned char)(timestamp>>0);
    
    UartEncodeAndSendMessage(0x0050, 5, payload);
}


unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;

    //Détermination de la position des obstacles en fonction des télémètres
    if (robotState.distanceTelemetreDroit < 20 &&  
            robotState.distanceTelemetreCentre < 20 &&  
            robotState.distanceTelemetreGauche < 20)
        positionObstacle = OBSTACLE_PARTOUT;    
    else if (robotState.distanceTelemetreCentre < 20 &&  
            robotState.distanceTelemetreGauche < 20 &&
            robotState.distanceTelemetreDroit >20)
        positionObstacle = OBSTACLE_A_GAUCHE_EN_FACE;
    else if (robotState.distanceTelemetreDroit < 20 &&  
            robotState.distanceTelemetreCentre < 20 &&
            robotState.distanceTelemetreGauche > 20)
        positionObstacle = OBSTACLE_A_DROITE_EN_FACE;    
    else if (robotState.distanceTelemetreDroit < 20 &&  //obstacle à droite
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 20)
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreGauche < 20 && //Obstacle à gauche 
            robotState.distanceTelemetreDroit < 20 &&  
            robotState.distanceTelemetreCentre < 20)
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 20 && //Obstacle en face 
            robotState.distanceTelemetreDroit > 20 &&  
            robotState.distanceTelemetreGauche > 20)
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroit > 20 &&  
            robotState.distanceTelemetreCentre > 20 &&  
            robotState.distanceTelemetreGauche > 20) //pas d?obstacle  
        positionObstacle = PAS_D_OBSTACLE;




    //Détermination de l?état à venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_PARTOUT)
        nextStateRobot = STATE_RECULE;
    else if (positionObstacle == OBSTACLE_A_DROITE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_RECULE;

//    if (positionObstacle == PAS_D_OBSTACLE)
//        nextStateRobot = STATE_AVANCE;
//    if (positionObstacle == OBSTACLE_A_DROITE)
//        nextStateRobot = STATE_TOURNE_GAUCHE;
//    if (positionObstacle == OBSTACLE_A_GAUCHE)
//        nextStateRobot = STATE_TOURNE_DROITE;
//    if (positionObstacle == OBSTACLE_EN_FACE)
//        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    
    
    //Si l?on n?est pas dans la transition de l?étape en cours
    if (nextStateRobot != stateRobot - 1) {
        stateRobot = nextStateRobot;
    }

    
    
} 

