	thumb
	area  moncode, code, readonly
	export CalculDFT
	import TabSin
	import TabCos
	
CalculDFT		proc
	push {lr,r0,r1,r4,r5}					;on met dans la pile
	

	ldr r2,=TabCos
	bl CalculDFT_phase1					;recuperation Re(k) dans r0 car le C n'as accés qu'a r0
	
	smull r4,r5,r0,r0					;on fais Re(k)^2 dans r4 et r5
	pop {r0,r1}						;on reprend dans la pile TabSig et k vu qu'ils sont en attributs de la fonction et deja push

	
	ldr r2,=TabSin					
	bl CalculDFT_phase1					;on recupere -Im(k) dans r0
	
	smlal r4,r5,r0,r0					;Re(k)^2 est déja dans r4 et r5 et on accumule avec (-Im(k))^2
	
	
	mov r0,r5						;Comme nous somme sur deux registres on veut que la partie de poid fort
	pop {lr,r4,r5}
	bx lr
	endp
	
CalculDFT_phase1	proc
	push {r4-r7}

	
	mov r4,r0 					;tab signal
	mov r5,r2 					;tab cos ou sin
	mov r6,r1 					;k est dans r6
	
	mov r7,#0					;dans r7 on cree ik qu'on incrémentera de k a chaques fois jusqua la fin de la boucle à 64
	
	mov r3,#0 					;met i à 0 dans r3
	mov r0,#0 					;resultat de la somme dans r0
boucle	ldrh r1, [r4, r3 ,LSL #1] 			;on charge x(i)
	ldrsh r2,[r5,r7,LSL #1] 			;on charge cos(ik) dans r2

	mla r0, r1,r2,r0 					;multiplication x(i)*cos(ik) dans r1
	 					;ajout dans la somme
	
	add r7,r7,r6 					;on ajoute k
	and r7,r7,#63					;on fais un modulo 64 de ik

	add r3,#1 					;on incremente i
	cmp r3,#64					;if r0=64 
	
	bne boucle
	beq sortie 					;b equal = il vas ou si c =63
	
	
sortie 
	
	pop  {r4-r7}
	bx lr
	endp
	end
		