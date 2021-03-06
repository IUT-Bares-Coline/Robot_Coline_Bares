#ifndef ROBOT_H
#define ROBOT_H

typedef struct robotStateBITS {
    
union {
    struct {
        unsigned char taskEnCours ;

        float vitesseGaucheConsigne ;
        float vitesseGaucheCommandeCourante ;
        float vitesseDroiteConsigne ;
        float vitesseDroiteCommandeCourante ;
        float distanceTelemetreDroit;
        float distanceTelemetreCentre;
        float distanceTelemetreGauche;
        
        double vitesseDroitFromOdometry;
        double vitesseGaucheFromOdometry;
        double vitesseLineaireFromOdometry;
        double vitesseAngulaireFromOdometry;

        double angleRadianFromOdometry_1;
        double angleRadianFromOdometry;
        double xPosFromOdometry;
        double yPosFromOdometry;
        double xPosFromOdometry_1;
        double yPosFromOdometry_1;
        
        double vitesseLineaireConsigne;
        double vitesseAngulaireConsigne;
        
            } ;
    } ; 
} 
ROBOT_STATE_BITS;
extern volatile ROBOT_STATE_BITS robotState ;

#endif /* ROBOT_H */
