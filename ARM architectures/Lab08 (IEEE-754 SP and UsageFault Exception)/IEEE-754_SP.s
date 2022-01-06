; +---------------------------------------------+
; | LAB08:              PART I                  |
; |              FLOATING-POINT NUMBERS         |
; |        IEEE-754 SP STANDARD IMPLEMENTATION  |
; +---------------------------------------------+
; IEEE-754 SP standard expresses floating point numbers in 32 bits as follows:
; 	- bit[31]:    sign bit. 0 -> positive, 1 -> negative
;	- bit[23:30]: exponent bits ("E" with bias 127)
; 	- bit[0:22]:  mantissa bits (fractional part of 1 <= "X" < 2)
; ---------------------------------------------------------------------------------
; There is not any native implementation of floating point numbers in Cortex-M3: we
; want to implement it in software.

integer_part	RN r0
fractional_part RN r1									
exponent        RN r2
mantissa        RN r3

		; +------------------------------------------+
		; | DEFINITION OF THE NUMBER TO BE CONVERTED |
		; +------------------------------------------+
		; Number to be expressed in IEEE-754 SP -> 1998.142578125
		; If Rn contains the integer part and Rm the fractional part, 
		; write in Rd the floating point number according to 
		; IEEE-754 SP standard.
		; ---------------------------------------------------------------
		LDR integer_part,    =0x000007CE		; 0x000007CE = 1998 
		LDR fractional_part, =0x087F91CD		; 0x087F91CD = 142578125

		; +------------------+
		; | COMPUTE EXPONENT |
		; +------------------+
		; The exponent is equal to the position of the first bit set to 1 in the binary 
		; representation of the integer part.
		; As an example, 1998 = 2_11111001110, where the position of the first bit is 10.
		; After adding the 127 bias, the exponent becomes 10 + 127 = 137 = 2_10001001
		; --------------------------------------------------------------------------------
		; The computation is implemented by exploiting the Carry Flag in the Logical Shift
		; Left (LSL). When the carry flag is set to 1, it means that the first bit of the 
		; integer part has been reached. A counter keeps track of the position in a loop.
		MOV r2, #31
		PUSH {integer_part}		; Preserving value
		
		; DO...WHILE Loop
exponentLoop	LSLS integer_part, integer_part, #1		; Logical Shift Left 
		MRS r3, APSR					; Accessing APSR ...
		AND r3, r3, #0x20000000				; ... in order to access Carry Flag (PSR[29])
		CMP r3, #0x20000000				; ALternatively, use BC (Branch with Carry)
		BEQ firstBit
		
		; Decreasing counter
		SUB r2, r2, #1
		
		; Test condition
		CMP r2, #0
		BNE exponentLoop
		
		; Position of first bit is stored in r2
firstBit	MOV mantissa, integer_part
		POP{integer_part}
		ADD exponent, exponent, #127			; Adding BIAS = 127
		
		; +------------------+
		; | COMPUTE MANTISSA |
		; +------------------+
		; The first bits of the mantissa are taken from the binary representation of the 
		; integer part, after removing the initial '1'.
		; As an example, 1998 = 2_11111001110, where the first 10 bits of the mantissa are 
		;      1111001110
		; ---------------------------------------------------------------------------------
		; The remaining N bits of the mantissa are obtained by converting the fractional 
		; part. In the conversion, the fractional part is interpreted as an integer number.
		; The algorithm is the following:
		; 
		; LET X = 142578125
		; LET P = 1000000000 (the lowest power of 10 which is higher than X)
		; 
		; REPEAT N TIMES:                       // N: remaining bits of the mantissa
		;     X = 2*X (X is doubled)
		;     if (X > P) {
		;         the next bit of the mantissa is 1
		;         the new value of X is X = X – P
		;     } else {
		;         the next bit of the mantissa is 0
		;     }
		; ---------------------------------------------------------------------------------
N		RN r4
P               RN r5
remaining_mant  RN r7           ; Remaining mantissa is filled at every iteration from the right
		
		; COMPUTING N
		; From the exponent, it is possible to know how many bits left in the mantissa we 
		; had, doing:
		;                 N = 23 - (exponent - 127)
		; ---------------------------------------------------------------------------------
		MOV N, #0x17                       ; 0x17 = 23
		SUB r6, exponent, #0x7F            ; 0x7F = 127
		SUB N, N, r6
		
		; COMPUTING P
		; Let P start from 1 and be multiplied by a factor 10 every iteration of the loop
		; until P is greater than X (fractional_part).
		; ---------------------------------------------------------------------------------
		MOV P, #0x01                       
		MOV r6, #0xA                       ; factor of mutiplication
computeP        MUL P, P, r6
		CMP P, fractional_part
		BLE computeP 
		
		; MANTISSA LOOP
		; ---------------------------------------------------------------------------------
		PUSH {fractional_part}             ; Preserving fractional_part
		MOV r6, #0x02                      ; Multiplication factor stored in r6
		MOV remaining_mant, #0x00    
		
mantissaLoop    MUL fractional_part, fractional_part, r6
		CMP fractional_part, P
		BLT set_zero
		
		; ELSE: set_one
		; 
		;  +---+---+-...-+---+---+
		;  |   |   |     |   | 1 |
		;  +---+---+-...-+---+---+
		;                      ^_____ insert a 1 with a ORR operation
		;
		;  +---+---+-...-+---+---+
		;  |   |   |     | 1 |   | <- shift left to free the position
		;  +---+---+-...-+---+---+
		; ---------------------------------------------------------------------------------
		ORR remaining_mant, remaining_mant, #0x00000001 
		LSL remaining_mant, remaining_mant, #1
		SUB fractional_part, fractional_part, P
		
		; Decreasing counter and test condition
		; -------------------------------------
		SUB N, N, #1
		CMP N, #0
		BNE mantissaLoop
		B   done

set_zero        ORR remaining_mant, remaining_mant, #0x00000000 
		LSL remaining_mant, remaining_mant, #1
		
		; Decreasing counter and test condition
		; -------------------------------------
		SUB N, N, #1
		CMP N, #0
		BNE mantissaLoop

		; Aligning the mantissa (which was previously stored in the 
		; left-most bits of the register).
done            LSR remaining_mant, remaining_mant, #1
		LSR mantissa, mantissa, #9         		
		ADD mantissa, mantissa, remaining_mant
		LSL exponend, exponent, #23
               
                ; r12 storing the result
                ; ------------------------------------
		ADD r12, exponent, mantissa

		
		; Restoring fractional_part
		POP {fractional_part}
