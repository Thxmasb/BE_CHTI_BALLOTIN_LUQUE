#include "gassp72.h"

extern void timer_callback(void);
int etat=0;
extern short TabSig[];
extern int CalculDFT(unsigned short *,int );

int M2[6];
unsigned short int dma_buf[64];
int cmpt[6]={0,0,0,0,0,0};
int score[6]={0,0,0,0,0,0};
int M2TIR=3936256/4;
int SYSTICK_PER=360000;

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
		}

	}
	
}
	
int main(void)
{	
	// activation de la PLL qui multiplie la fréquence du quartz par 9
	CLOCK_Configure();
	// PA2 (ADC voie 2) = entrée analog
	GPIO_Configure(GPIOA, 2, INPUT, ANALOG);
	// PB1 = sortie pour profilage à l'oscillo
	GPIO_Configure(GPIOB, 1, OUTPUT, OUTPUT_PPULL);
	// PB14 = sortie pour LED
	GPIO_Configure(GPIOB, 14, OUTPUT, OUTPUT_PPULL);

	// activation ADC, sampling time 1us
	Init_TimingADC_ActiveADC_ff( ADC1, 49); //0x33=51; 0x52=82; 0x3E=62; 0x3C=60; 0x31=49
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
