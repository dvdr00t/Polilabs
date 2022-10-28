		; +-----------+
		; | DATA AREA |
		; +-----------+
		; Allocating 26 bytes into a data area without initialization
		; ------------------------------------------------------------
		AREA 	myData, DATA, READWRITE
data1	        SPACE	26	

                AREA    |.text|, CODE, READONLY

exe02_solution  PROC

                ; +--------------------+
                ; | FIBONACCI SEQUENCE |
                ; +--------------------+
                ; r0 = 1
                ; r1 = 1
                ; r2 = r1 + r0
                ; r3 = r2 + r1
                ; --------------------------------------- 				
		MOV r0, #0x01
		MOV r1, #0x01
		ADD r2, r1, r0
		ADD r3, r2, r1
		ADD r4, r3, r2
		ADD r5, r4, r3
		ADD r6, r5, r4
		ADD r7, r6, r5
		ADD r8, r7, r6
		ADD r9, r8, r7
		ADD r10, r9, r8
		ADD r11, r10, r9
		ADD r12, r11, r10
				
		; Loading address of data space previously created 
		LDR r14, =data1
		
		; +------------------------+
		; | PRE-INDEXED ADDRESSING |
		; +------------------------+
		STR  r0, [r14]
		STR  r1, [r14, #1] !
		STR  r2, [r14, #1] !
		STR  r3, [r14, #1] !
		STR  r4, [r14, #1] !
		STR  r5, [r14, #1] !
		STR  r6, [r14, #1] !
		STR  r7, [r14, #1] !
		STR  r8, [r14, #1] !
		STR  r9, [r14, #1] !
		STR r10, [r14, #1] !
		STR r11, [r14, #1] !
		STR r12, [r14, #1] !
				
		; +-------------------------+
		; | POST-INDEXED ADDRESSING |
		; +-------------------------+
		ADD r14, #1
		STR r12, [r14], #1
		STR r11, [r14], #1
		STR r10, [r14], #1
		STR  r9, [r14], #1
		STR  r8, [r14], #1
		STR  r7, [r14], #1
		STR  r6, [r14], #1
		STR  r5, [r14], #1
		STR  r4, [r14], #1
		STR  r3, [r14], #1
		STR  r2, [r14], #1
		STR  r1, [r14], #1
		STR  r0, [r14], #1
				
stop		B stop
                ENDP
