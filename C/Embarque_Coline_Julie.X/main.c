#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "Robot.h"
#include "ToolBox.h"
#include "adc.h"
#include "main.h"

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

    //void __attribute__((interrupt, no_auto_psv)) _T3Interrupt();
    //robotState.vitesseDroiteConsigne = 30;



    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    while (1) {
        //LED_BLANCHE=!LED_BLANCHE ;
        //LED_ORANGE=!LED_ORANGE;

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
        }

    }


}// fin main
    

unsigned char stateRobot;



void OperatingSystemLoop(void) {
    
    if(JACK == 0)
    {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
            break;
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 500)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(20, MOTEUR_DROIT); 
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;
            
       
        }
    }       

    } 
//}




unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;

    //Détermination de la position des obstacles en fonction des télémètres
    if (robotState.distanceTelemetreDroit < 20 && //20
            robotState.distanceTelemetreCentre > 10 && //10
            robotState.distanceTelemetreGauche > 20) //Obstacle à droite  //20
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreDroit > 20 && //20
            robotState.distanceTelemetreCentre > 10 &&  //10
            robotState.distanceTelemetreGauche < 20) //Obstacle à gauche  //20
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 10) //Obstacle en face  //10
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroit > 20 &&  //20
            robotState.distanceTelemetreCentre > 10 &&  //10
            robotState.distanceTelemetreGauche > 20) //pas d?obstacle  //20
        positionObstacle = PAS_D_OBSTACLE;
    //-10 a chaque distance penser a les remettre apres

    //Détermination de l?état à venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if ((positionObstacle == OBSTACLE_A_DROITE)&&(positionObstacle == OBSTACLE_EN_FACE)&&(positionObstacle == OBSTACLE_A_GAUCHE))
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if ((positionObstacle == OBSTACLE_A_DROITE)&&(positionObstacle == OBSTACLE_EN_FACE))
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if ((positionObstacle == OBSTACLE_EN_FACE)&&(positionObstacle == OBSTACLE_A_GAUCHE))
        nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    
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