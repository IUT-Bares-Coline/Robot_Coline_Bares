//Partie PWM
#include <xc.h> // library xc.h inclut tous les uC
#include "IO.h"
#include "PWM.h"
#include "Robot.h"
#include "ToolBox.h"


#define PWMPER 40.0
unsigned char acceleration = 1000;

void InitPWM(void) {
    PTCON2bits.PCLKDIV = 0b000; //Divide by 1
    PTPER = 100 * PWMPER; //P�riode en pourcentage

    //R�glage PWM moteur 1 sur hacheur 1
    IOCON1bits.POLH = 1; //High = 1 and active on low =0
    IOCON1bits.POLL = 1; //High = 1
    IOCON1bits.PMOD = 0b01; //Set PWM Mode to Redundant
    FCLCON1 = 0x0003; //D�sactive la gestion des faults

    //Reglage PWM moteur 2 sur hacheur 6
    IOCON6bits.POLH = 1; //High = 1
    IOCON6bits.POLL = 1; //High = 1
    IOCON6bits.PMOD = 0b01; //Set PWM Mode to Redundant
    FCLCON6 = 0x0003; //D�sactive la gestion des faults

    /* Enable PWM Module */
    PTCONbits.PTEN = 1;
}

//void PWMSetSpeed(float vitesseEnPourcents, unsigned char motor) {
//
//    robotState.vitesseGaucheCommandeCourante = vitesseEnPourcents;
//    robotState.vitesseDroiteCommandeCourante = vitesseEnPourcents;
//
//
//    if (motor == MOTEUR_DROIT) {
//        if (robotState.vitesseDroiteCommandeCourante > 0) {
//            MOTEUR_DROIT_ENL = 0; //Pilotage de la pin en mode IO
//            MOTEUR_DROIT_INL = 1; //Mise � 1 de la pin
//            MOTEUR_DROIT_ENH = 1; //Pilotage de la pin en mode PWM
//            MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommandeCourante * PWMPER);
//        } else {
//            MOTEUR_DROIT_ENL = 1; //Pilotage de la pin en mode IO
//            MOTEUR_DROIT_INH = 1; //Mise � 1 de la pin
//            MOTEUR_DROIT_ENH = 0; //Pilotage de la pin en mode PWM
//            MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommandeCourante * PWMPER);
//        }
//    }
//    if (motor == MOTEUR_GAUCHE) {
//        if (robotState.vitesseGaucheCommandeCourante > 0) {
//            MOTEUR_GAUCHE_ENL = 1; //Pilotage de la pin en mode IO
//            MOTEUR_GAUCHE_INL = 1; //Mise � 1 de la pin
//            MOTEUR_GAUCHE_ENH = 0; //Pilotage de la pin en mode PWM
//            MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante * PWMPER);
//        } else {
//            MOTEUR_GAUCHE_ENL = 0; //Pilotage de la pin en mode IO
//            MOTEUR_GAUCHE_INH = 1; //Mise � 1 de la pin
//            MOTEUR_GAUCHE_ENH = 1; //Pilotage de la pin en mode PWM
//            MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante * PWMPER);
//        }
//    }
//}

void PWMUpdateSpeed() {
    // Cette fonction est appel�e sur timer et permet de suivre des rampes d?acc�l�ration
    if (robotState.vitesseDroiteCommandeCourante < robotState.vitesseDroiteConsigne)
        robotState.vitesseDroiteCommandeCourante = Min(
            robotState.vitesseDroiteCommandeCourante + acceleration,
            robotState.vitesseDroiteConsigne);
            //LED_BLEUE = !LED_BLEUE ;
    if (robotState.vitesseDroiteCommandeCourante > robotState.vitesseDroiteConsigne)
        robotState.vitesseDroiteCommandeCourante = Max(
            robotState.vitesseDroiteCommandeCourante - acceleration,
            robotState.vitesseDroiteConsigne);

    if (robotState.vitesseDroiteCommandeCourante > 0) {
        MOTEUR_DROIT_ENL = 0; //pilotage de la pin en mode IO
        MOTEUR_DROIT_INL = 1; //Mise � 1 de la pin
        MOTEUR_DROIT_ENH = 1; //Pilotage de la pin en mode PWM
    } else {
        MOTEUR_DROIT_ENH = 0; //pilotage de la pin en mode IO
        MOTEUR_DROIT_INH = 1; //Mise � 1 de la pin
        MOTEUR_DROIT_ENL = 1; //Pilotage de la pin en mode PWM
    }
    MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommandeCourante) * PWMPER;
    
    

    if (robotState.vitesseGaucheCommandeCourante < robotState.vitesseGaucheConsigne)
        robotState.vitesseGaucheCommandeCourante = Min(
            robotState.vitesseGaucheCommandeCourante + acceleration,
            robotState.vitesseGaucheConsigne);
    if (robotState.vitesseGaucheCommandeCourante > robotState.vitesseGaucheConsigne)
        robotState.vitesseGaucheCommandeCourante = Max(
            robotState.vitesseGaucheCommandeCourante - acceleration,
            robotState.vitesseGaucheConsigne);

    if (robotState.vitesseGaucheCommandeCourante > 0) {
        MOTEUR_GAUCHE_ENL = 0; //pilotage de la pin en mode IO
        MOTEUR_GAUCHE_INL = 1; //Mise � 1 de la pin
        MOTEUR_GAUCHE_ENH = 1; //Pilotage de la pin en mode PWM
    } else {
        MOTEUR_GAUCHE_ENH = 0; //pilotage de la pin en mode IO
        MOTEUR_GAUCHE_INH = 1; //Mise � 1 de la pin
        MOTEUR_GAUCHE_ENL = 1; //Pilotage de la pin en mode PWM
    }
    MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante) * PWMPER;
}

void PWMSetSpeedConsigne(float vitesseEnPourcents, char moteur) {

    if (moteur == MOTEUR_GAUCHE) {
        robotState.vitesseGaucheConsigne = vitesseEnPourcents;

    }
    if(moteur == MOTEUR_DROIT) {
        robotState.vitesseDroiteConsigne = -vitesseEnPourcents;

    }
}




double sortieCorrecteurAngulaireI = 0 ;
double sortieCorrecteurLineaireI = 0 ;


void PWMSetSpeedConsignePolaire(){  
    
    //robotState.vitesseLineaireConsigne = (robotState.vitesseDroiteConsigne + robotState.vitesseGaucheConsigne ) / 2 ;
    //robotState.vitesseAngulaireConsigne = RAYONROUE /robotState.vitesseLineaireConsigne ;
    robotState.vitesseAngulaireConsigne = 0 ;
    robotState.vitesseLineaireConsigne = 0 ;
    
    // Correction Angulaire
    double erreurVitesseAngulaire = robotState.vitesseAngulaireConsigne - robotState.vitesseAngulaireFromOdometry;
    double sortieCorrecteurAngulaireP = P_angulaire * erreurVitesseAngulaire;
    sortieCorrecteurAngulaireI = i_angulaire*(erreurVitesseAngulaire/FREQ_ECH_QEI) + sortieCorrecteurAngulaireI ;
    double sortieCorrecteurAngulaire = sortieCorrecteurAngulaireP + sortieCorrecteurAngulaireI ;
    double correctionVitesseAngulairePourcent = sortieCorrecteurAngulaire * COEFF_VITESSE_ANGULAIRE_PERCENT;//sortieCorrecteurAngulaire * DISTROUES/2 * COEFF_VITESSE_ANGULAIRE_PERCENT;

    // Correction Lin�aire
    double erreurVitesseLineaire = robotState.vitesseLineaireConsigne - robotState.vitesseLineaireFromOdometry;
    double sortieCorrecteurLineaireP = P_lineaire * erreurVitesseLineaire;
    sortieCorrecteurLineaireI = (i_lineaire*erreurVitesseLineaire)/FREQ_ECH_QEI + sortieCorrecteurLineaireI ;
    double sortieCorrecteurLineaire = sortieCorrecteurLineaireP + sortieCorrecteurLineaireI ;
    double correctionVitesseLineairePourcent = sortieCorrecteurLineaire * COEFF_VITESSE_LINEAIRE_PERCENT;//sortieCorrecteurLineaire * DISTROUES/2 * COEFF_VITESSE_LINEAIRE_PERCENT;
    
    
    //��Gnration des consignes droite et gauche
    robotState.vitesseDroiteConsigne = -(correctionVitesseLineairePourcent + correctionVitesseAngulairePourcent * DISTROUES/2);//-(correctionVitesseLineairePourcent + correctionVitesseAngulairePourcent) ; //"-" --> moteur iners� annette
    robotState.vitesseDroiteConsigne = LimitToInterval(robotState.vitesseDroiteConsigne, -100, 100 ) ;
    robotState.vitesseGaucheConsigne = correctionVitesseLineairePourcent - correctionVitesseAngulairePourcent * DISTROUES/2;
    robotState.vitesseGaucheConsigne = LimitToInterval(robotState.vitesseGaucheConsigne, -100, 100 ) ;
}