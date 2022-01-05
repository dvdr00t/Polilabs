exe06_solution  PROC

                ; +------------------------------+
				; | MY SMUAD/SMUSD DOCUMENTATION |
				; +------------------------------+
				; Cortex-M4 instruction set contains the following instruction:
				;      SMUAD <Rd>, <Rn>, <Rm>
                ;      SMUSD <Rd>, <Rn>, <Rm>
				; Both instructions multiply the lower halfword of Rn times the lower 
				; halfword of Rm, and the higher halfword of Rn times the higher halfword of Rm.
	            ; Halfwords are considered in two's complement.
				; SMUAD sums the two products and saves the result in Rd.
                ; SMUSD subtracts the product of the higher halfwords from the product of 
				; the lower halfwords, storing the result in Rd.
				; 
				; For example:
				;    
				;       Rn = 0x 7A30 458D
				;       Rm = 0x C315 9EAA
                ;
				; The sign of halfwords has to be extended before multiplication.  
				; 
                ; 		0x458D  *  0x9EAA  = 0xE58E35A2
				;       (17805) * (-24918) = (-443664990)     <-- 2's complement
				;       (17805) *  (40618) =  (723203490)     <-- Pure binary
				; 
				;       0x7A30 * 0xC315 = 0xE2EC95F0
				;
				; With SMUAD, Rd = 0xC87ACB92       
				; With SMUSD, Rd = 0x02A19FB2
				; --------------------------------------------------------------------------

                ; Defining the data
				; --------------------------------------------------------------------------
				LDR r0, =0x7A30458D
				LDR r1, =0xC3159EAA
				
				; Calling procedures
				; --------------------------------------------------------------------------
				BL SMUAD_routine
				BL SMUSD_routine
				
stop			B stop
                ENDP
			
			    ; +----------------------+
				; |	SMUAD IMPLEMENTATION |
				; +----------------------+
				; Parameters are passed with registers. In particular:
				; r0: first parameter
				; r1: second parameter
				; r4: result
SMUAD_routine	PROC
				PUSH {LR}
				
				; Initialization
				; --------------------------------------------------------------------------
				BL preparation
				
				; Summing the two results obtained
				; --------------------------------------------------------------------------
				ADD r4, r2, r3
				
				POP  {PC}
				ENDP
					
				; +----------------------+
				; |	SMUSD IMPLEMENTATION |
				; +----------------------+
				; Parameters are passed with registers. In particular:
				; r0: first parameter
				; r1: second parameter
				; r4: result
SMUSD_routine	PROC
				PUSH {LR}
				
				; Initialization
				; --------------------------------------------------------------------------
				BL preparation
				
				; Summing the two results obtained
				; --------------------------------------------------------------------------
				SUB r4, r3, r2
				
				POP  {PC}
				ENDP
				
				; +---------------------+
				; |	PREPARATION ROUTINE |
				; +---------------------+
				; Parameters are passed with registers. In particular:
				; r0: first parameter
				; r1: second parameter
				; r2: stores the product of the highest bytes
				; r3: stores the product of the lowest bytes
preparation     PROC
				PUSH {LR}
				
				; Operations on the highest bytes
				; --------------------------------------------------------------------------
				MOV r8, r0, ASR #16         ; ASR performs Arithmetic Shift Right 
				MOV r9, r1, ASR #16         ; in order to obtain sign extension
				MUL r2, r8, r9
				
				; Operations on the lowest bytes
				; --------------------------------------------------------------------------
				MOV r8, r0, LSL #16         ; LSL #16 allows to load the lowest half of the 
				ASR r8, #16                 ; register in the upper-most 16 bits of the 
				MOV r9, r1, LSL #16         ; destination register, in order to perform the
				ASR r9, #16                 ; sign extension with the ASR Arithmetic shift
				MUL r3, r8, r9
				
				POP  {PC}
	            ENDP