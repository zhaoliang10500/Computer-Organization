			.text
			.global _start

_start:
			LDR R9, =MAX
			LDR R10, =MIN
			 	 
			LDR R0, [R10, #4]
			LDR R1, [R10, #8]
			LDR R2, [R10, #12]
			LDR R3, [R10, #16]

			ADD R4, R0, R1
			ADD R5, R2, R3
			MUL R6, R4, R5
			MOV R4, #0
			MOV R5, #0

			ADD R4, R0, R2
			ADD R5, R1, R3
			MUL R7, R4, R5
			MOV R4, #0
			MOV R5, #0
						
			ADD R4, R0, R3
			ADD R5, R1, R2
			MUL R8, R4, R5
			MOV R4, #0
			MOV R5, #0

			CMP R6,R7
			MOVGT R5, R6
			MOVGT R4, R7
			MOVLT R5, R7
			MOVLT R4, R6			
			CMP R5, R8
			MOVLT R5, R8		
			CMP R4, R8
			MOVGT R4, R8
			STR R4, [R10]
			STR R5, [R9]
MAX:		.word	0
MIN:		.WORD	0
NUMBERS: 	.word	5,2,3,4
			
