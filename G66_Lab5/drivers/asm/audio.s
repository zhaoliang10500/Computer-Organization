		.text
		.equ FIFOSPACE, 0xFF203044
		.equ LEFT_DATA, 0xFF203048
		.equ RIGHT_DATA, 0xFF20304C
		.global write_audio_ASM

write_audio_ASM: 		
		PUSH {R1, R2}
		LDR R1, =FIFOSPACE	//POINTER
		LDRB R2, [R1,#2]
		//CLEAR = 0X0000FFFF
		TST R2, #0xFF//WLSC, LEFT IS FULL
		BEQ ROUTE
		LDRB R2,[R1, #3]
		TST R2, #0xFF	//WRSC, RIGHT IS FULL
		BEQ ROUTE
		LDR R1, =LEFT_DATA
		STR R0, [R1]
		LDR R1, =RIGHT_DATA
		STR R0, [R1]
		MOV R0, #1		//RETURN 1 IF DATA STORED
		POP {R1, R2}
		BX LR
		
		
ROUTE:			
		MOV R0, #0		//RETURN O IF NOT STORED
		POP {R1, R2}
		BX LR

		.end
