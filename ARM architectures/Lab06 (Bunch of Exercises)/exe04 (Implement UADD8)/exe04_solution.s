exe04_solution   PROC
				
                ; +------------------------+
		; | MY UADD8 DOCUMENTATION |
		; +------------------------+
                ; Cortex-M4 instruction set contains the following instruction:
		;      UADD8 <Rd>, <Rn>, <Rm>
		; UADD8 sums corresponding bytes of Rn and Rm, storing the result in Rd.
		; 
		; For example:
		;     Rn = 0x 7A 30 45 8D
		;     Rm = 0x C3 15 9E AA
		;     Rd = 0x 3D 45 E3 37
		; 
		; Please note the absence of carry between bytes in Rd.
		; --------------------------------------------------------------------------
		
		; Defining data
		LDR r0, =0x7A30458D
		LDR r1, =0xC3159EAA
		
		; Calling procedure
		BL UADD8_routine
				
stop		B stop
                ENDP
					
		; +----------------------+
		; |	UADD8 IMPLEMENTATION |
		; +----------------------+
		; Parameters are passed with registers. In particular:
		; r0: first parameter
		; r1: second parameter
		; r4: result
UADD8_routine	PROC
		PUSH {LR}
		
		; BYTE 0x FF 00 00 00
		MOV r8, r0
		AND r8, #0xFF000000
		MOV r9, r1
		AND r9, #0xFF000000
		ADD r10, r8, r9
		AND r10, #0xFF000000
		ADD r4, r4, r10
		
		; BYTE 0x 00 FF 00 00
		MOV r8, r0
		AND r8, #0x00FF0000
		MOV r9, r1
		AND r9, #0x00FF0000
		ADD r10, r8, r9
		AND r10, #0x00FF0000
		ADD r4, r4, r10
		
		; BYTE 0x 00 00 FF 00
		MOV r8, r0
		AND r8, #0x0000FF00
		MOV r9, r1
		AND r9, #0x0000FF00
		ADD r10, r8, r9
		AND r10, #0x0000FF00
		ADD r4, r4, r10
				
		; BYTE 0x 00 00 00 FF
		MOV r8, r0
		AND r8, #0x000000FF
		MOV r9, r1
		AND r9, #0x000000FF
		ADD r10, r8, r9
		AND r10, #0x000000FF
		ADD r4, r4, r10
		
		POP  {PC}
		ENDP
