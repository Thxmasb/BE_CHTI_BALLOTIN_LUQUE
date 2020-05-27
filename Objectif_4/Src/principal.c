#include "gassp72.h"

extern void timer_callback(void);
extern short TabSig[];
extern int CalculDFT(unsigned short *,int );

int M2[6];
unsigned short int dma_buf[64];
int cmpt[6]={0,0,0,0,0,0};
int score[6]={0,0,0,0,0,0};
int M2TIR=3936256/4;
int SYSTICK_PER=360000;
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

void sys_callback(){
	// Démarrage DMA pour 64 points
	Start_DMA1(64);
	Wait_On_End_Of_DMA1();
	Stop_DMA1;
	
	M2[0]=CalculDFT(dma_buf,17);
	M2[1]=CalculDFT(dma_buf,18);
	M2[2]=CalculDFT(dma_buf,19);
	M2[3]=CalculDFT(dma_buf,20);
	M2[4]=CalculDFT(dma_buf,23);
	M2[5]=CalculDFT(dma_buf,24);
	
	for(int k=0;k<=5;k++){
		if(M2[k]>M2TIR){
			cmpt[k]++;
		}
		else{
			cmpt[k]=0;
		}
		if(cmpt[k]>0x13){
			score[k]=score[k]+1;
			etat.position=0;
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
		}

	}
	
}
	
int main(void)
{	
	// activation de la PLL qui multiplie la fréquence du quartz par 9
	CLOCK_Configure();
	
	//Pour une fréquence du PWM supérieure à 20kHz mais multiple exact de la fréquence 
	//d'échantillonnage audio (= 10,9 kHz) on peut prendre 21,8 kHz et en période exprimée en Tick ça donne 30 000
	GPIO_Configure(GPIOB,0,OUTPUT,ALT_PPULL);
	// PA2 (ADC voie 2) = entrée analog
	GPIO_Configure(GPIOA, 2, INPUT, ANALOG);
	// PB1 = sortie pour profilage à l'oscillo
	GPIO_Configure(GPIOB, 1, OUTPUT, OUTPUT_PPULL);
	// PB14 = sortie pour LED
	GPIO_Configure(GPIOB, 14, OUTPUT, OUTPUT_PPULL);	
	// config TIM3-CH3 en mode PWM
	etat.resolution = PWM_Init_ff( TIM3, 3, Periode_PWM_en_Tck );
	etat.taille = LongueurSon;
	etat.son = Son;
	etat.Tech_en_Tck = PeriodeSonMicroSec*72;

	// activation ADC, sampling time 1us
	Init_TimingADC_ActiveADC_ff( ADC1, 51); //0x33=51; 0x52=82; 0x3E=62; 0x3C=60; 0x31=49
	Single_Channel_ADC( ADC1, 2 );
	// Déclenchement ADC par timer2, periode (72MHz/320kHz)ticks
	Init_Conversion_On_Trig_Timer_ff( ADC1, TIM2_CC2, 225 );
	// Config DMA pour utilisation du buffer dma_buf (a créér)
	Init_ADC1_DMA1( 0, dma_buf );

	// Config Timer, période exprimée en périodes horloge CPU (72 MHz)
	Systick_Period_ff( SYSTICK_PER );
	// enregistrement de la fonction de traitement de l'interruption timer
	// ici le 3 est la priorité, sys_callback est l'adresse de cette fonction, a créér en C
	Systick_Prio_IT( 3, sys_callback );
	SysTick_On;
	SysTick_Enable_IT;
	
	while	(1)
		{

		}
}
