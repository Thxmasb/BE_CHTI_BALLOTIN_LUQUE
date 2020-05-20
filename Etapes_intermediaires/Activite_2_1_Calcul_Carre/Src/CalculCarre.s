	thumb
	area  moncode, code, readonly
	export CalculCarre
	import TabSin
	import TabCos
	
CalculCarre	proc
	ldr r6,=TabSin 
	ldr r7,=TabCos
	ldrh r2,[r6, r0,LSL #1] ;r3 decalé de r0 * 2 octets
	ldrh r0,[r7, r0, LSL #1]
	
	mul r0,r0;
	mul r2,r2;multiplication accumulation
	
	add r0,r2
	
	bx lr
	
	
	endp
	end
		