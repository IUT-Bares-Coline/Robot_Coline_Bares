#ifndef PWM_H
#define	PWM_H

#define MOTEUR_DROIT 1
#define MOTEUR_GAUCHE 0


#define COEFF_VITESSE_LINEAIRE_PERCENT 23.
#define COEFF_VITESSE_ANGULAIRE_PERCENT 23.
#define P_angulaire 5
#define i_angulaire 600//25*0.7
#define P_lineaire 10
#define i_lineaire 500//3
#define DISTROUES 281.2
#define RAYONROUE 0.02584
#define FREQ_ECH_QEI 250


void InitPWM(void);
//void PWMSetSpeed(float,unsigned char);
void PWMUpdateSpeed();
void PWMSetSpeedConsigne(float vitesseEnPourcents, char moteur);
void PWMSetSpeedConsignePolaire();

#endif