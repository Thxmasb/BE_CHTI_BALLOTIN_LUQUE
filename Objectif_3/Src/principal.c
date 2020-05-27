#include "gassp72.h"

extern void son_callback(void);
extern short Son[];
extern int LongueurSon;
extern int PeriodeSonMicroSec;
int Periode_PWM_en_Tck=360;

typedef struct {
    int position;        // index courant dans le tableau d'echantillons
    int taille;          // nombre d'echantillons de l'enregistrement
    short int * son;     // adresse de base du tableau d'echantillons en ROM
    int resolution;      // pleine echelle du modulateur PWM
    int Tech_en_Tck;     // periode d'ech. audio en periodes d'horloge CPU
} type_etat;

type_etat etat;

int main(void)
{    //Pour une fréquence du PWM supérieure à 20kHz mais multiple exact de la fréquence 
    //d'échantillonnage audio (= 10,9 kHz) on peut prendre 21,8 kHz et en période exprimée en Tick ça donne 30 000
    // activation de la PLL qui multiplie la fréquence du quartz par 9
    CLOCK_Configure();
    GPIO_Configure(GPIOB,0,OUTPUT,ALT_PPULL);
    // config TIM3-CH3 en mode PWM
    etat.resolution = PWM_Init_ff( TIM3, 3, Periode_PWM_en_Tck );
    etat.taille = LongueurSon;
    etat.son = Son;
    etat.Tech_en_Tck = PeriodeSonMicroSec*72;
    // config port PB1 pour être utilisé en sortie
    // initialisation du timer 4
    // Periode_en_Tck doit fournir la durée entre interruptions,
    // exprimée en périodes Tck de l'horloge principale du STM32 (72 MHz)
    Timer_1234_Init_ff( TIM4, etat.Tech_en_Tck );
    // enregistrement de la fonction de traitement de l'interruption timer
    // ici le 2 est la priorité, timer_callback est l'adresse de cette fonction, a créér en asm,
    // cette fonction doit être conforme à l'AAPCS
    Active_IT_Debordement_Timer( TIM4, 2, son_callback );
    // lancement du timer
    Run_Timer( TIM4 );
    
    
    while    (1)
        {
        }
}