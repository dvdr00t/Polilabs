Reset_Handler   PROC

                ; +------------------------+
				; | MY USAD8 DOCUMENTATION |
				; +------------------------+
				; Cortex-M4 instruction set contains the following instruction:
				;      USAD8 <Rd>, <Rn>, <Rm>
				; Each byte of Rn and Rm is a pure binary number. USAD8 calculates the absolute 
				; value of the difference between each byte in Rn and Rm. After that, USAD8 sums 
				; the four absolute values, storing the result in Rd.
				; 
				; For example:
				;     Rn = 0x 7A 30 45 8D
				;     Rm = 0x C3 15 9E AA
				;     
				;     1. |0x8D - 0xAA| = 0x1D
				;     2. |0x45 - 0x9E| = 0x59
				;     3. |0x30 - 0x15| = 0x1B
				;     4. |0x7A – 0xC3| = 0x49
				; 
				;     Rd = 0x1D + 0x59 + 0x1B + 0x49 = 0xDA
				; 
				; Note: the value in Rd can be on more than 8 bit
				; --------------------------------------------------------------------------

                ; Defining data
				LDR r0, =0x7A30458D
				LDR r1, =0xC3159EAA
				
				; Calling procedure
				BL USAD8_routine
				
stop			B stop
                ENDP
					
			    ; +----------------------+
				; |	USAD8 IMPLEMENTATION |
				; +----------------------+
				; Parameters are passed with registers. In particular:
				; r0: first parameter
				; r1: second parameter
				; r2: is the accumulator register
				; r3: stores the minuend
				; r4: stores the subtrahend
				; r5: result
USAD8_routine	PROC
				PUSH {LR}
				
				; Initialization
				MOV r2, #0
				
				; r4 = r3 - r4
				MOV r3, r0
				AND r3, #0xFF000000
				LSR r3, 24
				MOV r4, r1
				AND r4, #0xFF000000
				LSR r4, 24
				SUB r4, r3, r4                 
				
				; Module: |r4|
				ASR r6, r4, #31 
				ADD r4, r4, r6 
				EOR r4, r4, r6
				
				; Accumulation
				ADD r2, r2, r4
				
				; r4 = r3 - r4
				MOV r3, r0
				AND r3, #0x00FF0000
				LSR r3, 16
				MOV r4, r1
				AND r4, #0x00FF0000
				LSR r4, 16
				SUB r4, r3, r4                 
				
				; Module: |r4|
				ASR r6, r4, #31 
				ADD r4, r4, r6 
				EOR r4, r4, r6
				
				; Accumulation
				ADD r2, r2, r4
				
				; r4 = r3 - r4
				MOV r3, r0
				AND r3, #0x0000FF00
				LSR r3, 8
				MOV r4, r1
				AND r4, #0x0000FF00
				LSR r4, 8
				SUB r4, r3, r4                 
				
				; Module: |r4|
				ASR r6, r4, #31 
				ADD r4, r4, r6 
				EOR r4, r4, r6
				
				; Accumulation
				ADD r2, r2, r4
				
				; r4 = r3 - r4
				MOV r3, r0
				AND r3, #0x000000FF
				MOV r4, r1
				AND r4, #0x000000FF
				SUB r4, r3, r4                 
				
				; Module: |r4|
				ASR r6, r4, #31 
				ADD r4, r4, r6 
				EOR r4, r4, r6
				
				; Accumulation
				ADD r2, r2, r4
				
				MOV r5, r2
				POP  {PC}
				ENDP