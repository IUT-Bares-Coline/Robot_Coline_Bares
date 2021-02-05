//Partie PWM
#include <xc.h> // library xc.h inclut tous les uC
#include "IO.h"
#include "PWM.h"
#include "Robot.h"
#include "ToolBox.h"


#define PWMPER 40.0
unsigned char acceleration = 1;

void InitPWM(void) {
    PTCON2bits.PCLKDIV = 0b000; //Divide by 1
    PTPER = 100 * PWMPER; //Période en pourcentage

    //Réglage PWM moteur 1 sur hacheur 1
    IOCON1bits.POLH = 1; //High = 1 and active on low =0
    IOCON1bits.POLL = 1; //High = 1
    IOCON1bits.PMOD = 0b01; //Set PWM Mode to Redundant
    FCLCON1 = 0x0003; //Désactive la gestion des faults

    //Reglage PWM moteur 2 sur hacheur 6
    IOCON6bits.POLH = 1; //High = 1
    IOCON6bits.POLL = 1; //High = 1
    IOCON6bits.PMOD = 0b01; //Set PWM Mode to Redundant
    FCLCON6 = 0x0003; //Désactive la gestion des faults

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
//            MOTEUR_DROIT_INL = 1; //Mise à 1 de la pin
//            MOTEUR_DROIT_ENH = 1; //Pilotage de la pin en mode PWM
//            MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommandeCourante * PWMPER);
//        } else {
//            MOTEUR_DROIT_ENL = 1; //Pilotage de la pin en mode IO
//            MOTEUR_DROIT_INH = 1; //Mise à 1 de la pin
//            MOTEUR_DROIT_ENH = 0; //Pilotage de la pin en mode PWM
//            MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommandeCourante * PWMPER);
//        }
//    }
//    if (motor == MOTEUR_GAUCHE) {
//        if (robotState.vitesseGaucheCommandeCourante > 0) {
//            MOTEUR_GAUCHE_ENL = 1; //Pilotage de la pin en mode IO
//            MOTEUR_GAUCHE_INL = 1; //Mise à 1 de la pin
//            MOTEUR_GAUCHE_ENH = 0; //Pilotage de la pin en mode PWM
//            MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante * PWMPER);
//        } else {
//            MOTEUR_GAUCHE_ENL = 0; //Pilotage de la pin en mode IO
//            MOTEUR_GAUCHE_INH = 1; //Mise à 1 de la pin
//            MOTEUR_GAUCHE_ENH = 1; //Pilotage de la pin en mode PWM
//            MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante * PWMPER);
//        }
//    }
//}

void PWMUpdateSpeed() {
    // Cette fonction est appelée sur timer et permet de suivre des rampes d?accélération
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
        MOTEUR_DROIT_INL = 1; //Mise à 1 de la pin
        MOTEUR_DROIT_ENH = 1; //Pilotage de la pin en mode PWM
    } else {
        MOTEUR_DROIT_ENH = 0; //pilotage de la pin en mode IO
        MOTEUR_DROIT_INH = 1; //Mise à 1 de la pin
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
        MOTEUR_GAUCHE_INL = 1; //Mise à 1 de la pin
        MOTEUR_GAUCHE_ENH = 1; //Pilotage de la pin en mode PWM
    } else {
        MOTEUR_GAUCHE_ENH = 0; //pilotage de la pin en mode IO
        MOTEUR_GAUCHE_INH = 1; //Mise à 1 de la pin
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

/*
#define COEFF_VITESSE_LINEAIRE_PERCENT 1/25.
#define COEFF_VITESSE_ANGULAIRE_PERCENT 1/50.
void PWMSetSpeedConsignePolaire(){
// C o r r e c ti o n A n g ul ai r e
double erreurVitesseAngulaire = ...
double s o r t i e C o r r e c t e u r A n g u l a i r e = . . .
double c o r r e c t i o n V i t e s s e A n g u l a i r e = . . .
double c o r r e c ti o n Vi t e s s e A n g ul ai r e P o u r c e n t =
c o r r e c t i o n V i t e s s e A n g u l a i r e ? COEFF\_VITESSE\_ANGULAIRE\_PERCENT;
// C o r r e c ti o n éL i n a i r e
double e r r e u r V i t e s s e L i n e a i r e = . . .
double s o r t i e C o r r e c t e u r L i n e a i r e = . . .
double c o r r e c t i o n V i t e s s e L i n e a i r e = . . .
double c o r r e c t i o n V i t e s s e L i n e a i r e P o u r c e n t =
c o r r e c t i o n V i t e s s e L i n e a i r e ? COEFF\_VITESSE\_LINEAIRE\_PERCENT;
//ééGnration de s c o n si g n e s d r o i t e e t gauche
r o b o t S t a t e . vi t e s s e D r oi t e C o n si g n e = c o r r e c t i o n V i t e s s e L i n e a i r e P o u r c e n t
+ c o r r e c ti o n Vi t e s s e A n g ul ai r e P o u r c e n t ;
r o b o t S t a t e . vi t e s s e D r oi t e C o n si g n e = Limi t T o I n t e r v al (
r o b o t S t a t e . vi t e s s e D r oi t e C o n si g n e , ?100, 1 0 0 ) ;
r o b o t S t a t e . vi t e s s eG a u c h eC o n si g n e = c o r r e c t i o n V i t e s s e L i n e a i r e P o u r c e n t
? c o r r e c ti o n Vi t e s s e A n g ul ai r e P o u r c e n t ;
r o b o t S t a t e . vi t e s s eG a u c h eC o n si g n e = Limi t T o I n t e r v al (
r o b o t S t a t e . vi te s seG aucheC on si gne , ?100, 1 0 0 ) ;
}
*/