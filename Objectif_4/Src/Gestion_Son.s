; ce programme est pour l'assembleur RealView (Keil)
	thumb
	area  moncode, code, readonly
	export son_callback
	import etat
	import LongueurSon
	import PeriodeSonMicroSec
	import Son 
        
;typedef struct {
;int position;        // 0
;int taille;        // 4
;void * son;        // 8
;int resolution;    // 12
;int periode_ticks;    // 16
;} type_etat;


son_callback	proc
    
;/*
E_POS    equ    0
E_TAI    equ    4
E_SON    equ    8
E_RES    equ    12
E_PER    equ    16
        
;*/
BORNE_SUP	equ	32768 
TIM3_CCR3	equ	0x4000043C    ; adresse registre PWM
    
	push     {r4}
    
	ldr	r1, =etat		; On charge l'adresse de la structure dans r1
	ldr     r2, [r1,#E_POS]		; On charge l'adresse de etat.position
	ldr	r3, [r1,#E_TAI]		; On charge l'adresse de etat.taille
    
	cmp	r2, r3			; On compare la position et la taille
	beq	silence			; Si c'est égal on va à silence
	bne	son			; Sinon à son
    
son	ldr	r4, [r1, #E_RES]	; On charge l'adresse de etat.résolution
	ldr	r3, [r1, #E_SON]	; On charge l'adresse du tableau de son
	ldrsh	r3, [r3, r2, lsl #1]	; On charge un half word (16 bits) on decale r3 de r2 pour avoir notre position et on incrémente la position 
    
	add	r3, r3, #BORNE_SUP	; On ajoute la composante continue
	mul	r3, r3, r4		; On multiplie par la facteur d'échelle
	lsr	r3, #16			; On divise par 2^16
					; Dorénavant nos échantillons sont entre 0 et 2^16
	ldr	r4, =TIM3_CCR3		; On charge l'adresse de TIM3
	str	r3, [r4]		; Puis on ajoute notre échantillon
    
	add    r2, #1			; On incrémente notre position à nouveau car nous sommes en 16 bits
	str    r2, [r1, #E_POS]		; On met à jour la valeur de la position

silence	pop{r4}
	bx	lr
	endp
	end