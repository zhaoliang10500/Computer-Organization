			.text
			.global _start

_start:
			LDR R7, =RANGE
			LDR R2, [R7,#4] //Hold number of elements in list
			ADD R3, R7, #8	//Points to the location of first number Loop counter
			LDR R0, [R3]
			MOV R1, #0
			
		
LOOP:		SUBS R2, R2, #1
			BEQ DONE
			LDR R4, [R3]
			ADD R3, R3, #4
			
CMPMIN:		CMP R0, R4
			BGT ISMIN
			B CMPMAX

ISMIN:		MOV R0, R4
			B CMPMAX			
			
CMPMAX:		CMP R1, R4
			BLT ISMAX
			B NOTHING
			
ISMAX: 		MOV R1, R4			
			B NOTHING

NOTHING:	SUB R6, R1,R0
			B LOOP
			
DONE: 		STR R6, [R7]

END: 		B END

RANGE:		.word	0
N: 			.word	7
NUMBERS: 	.word	90,50,60,10
			.word	20,40,70

