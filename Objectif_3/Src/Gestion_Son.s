; ce programme est pour l'assembleur RealView (Keil)
	thumb
	area  moncode, code, readonly
	export son_callback
	import etat
	import LongueurSonSilence 
	import PeriodeSonMicroSecSilence 
	import Silence 
		
;typedef struct {
;int position;		// 0
;int taille;		// 4
;void * son;		// 8
;int resolution;	// 12
;int periode_ticks;	// 16
;} type_etat;


son_callback	proc
	
;/*
E_POS	equ	0
E_TAI	equ	4
E_SON	equ	8
E_RES	equ	12
E_PER	equ	16
		end
;*/

TIM3_CCR3	equ	0x4000043C	; adresse registre PWM
		cmp E_POS, E_TAI
		bcs higherorsame
return

higherorsame 
		
		
		
	
	end
		
		