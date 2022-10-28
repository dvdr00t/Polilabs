exe01_solution  PROC
					
		; +-----------------------+
                ; | RE-DEFINING REGISTERS |
                ; +-----------------------+
single_value	RN r1
double_value	RN r2
triple_value	RN r3
quadruple_value	RN r4
quintuple_value	RN r5

                ; +----------------+
		; | FIRST SOLUTION |
		; +----------------+
		; Performing the multiplications just by using ADD operators
		; -----------------------------------------------------------
		MOV single_value, #0x02	                             ; single_value = 2 
		ADD double_value, single_value, single_value         
		ADD triple_value, double_value, single_value
		ADD quadruple_value, triple_value, single_value
		ADD quintuple_value, quadruple_value, single_value
				
                ; +----------------------------+
		; | SECOND SOLUTION (SUGGESTED)|
		; +----------------------------+
		; Performing the multiplication by exploiting the INLINE
		; BARREL SHIFTER with the MOV operator. 
		; -----------------------------------------------------------
		MOV single_value, #0x02                              ; single_value = 2
		MOV double_value, single_value, LSL #1               ; double_value = 2*2 = 4
		ADD triple_value, double_value, single_value         ; triple_value = 4+2 = 6
		MOV quadruple_value, single_value, LSL #2            ; quadruple_value = 2*4 = 8
		ADD quintuple_value, quadruple_value, single_value   ; quintuple_value = 8+2 = 10

stop	        B stop
                ENDP
