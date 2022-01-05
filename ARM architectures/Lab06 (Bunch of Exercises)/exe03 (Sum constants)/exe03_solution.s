exe0e_solution  PROC
				
				; LOADING IN THE REGISTERS THE ADDRESSES OF DATA
				; ----------------------------------------------------------
				LDR r0, =myConstants
				LDR r3, =result	
				
				; ----------------------------------------------------------
				; |           METHOD 1: NO USE OF LOOPS                    |
				; ----------------------------------------------------------
				
				; PERFORMING ADDITION OF FIRST TWO HALF-WORDS
				; ----------------------------------------------------------
				LDRH r1, [r0], #2	; LDRH loads into r1 the value stored in memory at location [r0]
									; r1 <- [r0] i.e. loads the first half-word of myConstants: r1 = myConstants[0]. 
									; Then, r0 <- r0 + 2B i.e. add an offset of 16 bits to r0 (post-index)
				
				LDRH r2, [r0], #2	; r2 <- [r0]. Then r0 <- r0 + 16 bits (2 bytes = 1 halfword)
				ADD  r1, r1, r2		; r1 <- r1 + r2 (i.e. r1 = myConstant[0] + myConstant[1])
				
				; Storing the result back in memory
				STR  r1, [r3], #4	; STR stores into memory at the address stored in r3 (i.e. [r3]) the value
									; stored in r1. Then, it performs post-index offset computation.
									; mem[r3] <- r1. Then r3 <- r3 + 32 bits (4 bytes = 1 word)
									; results[0] = r1 = myConstant[0] + myConstant[1])
				
				; Repeat for all the others value
				
				; ----------------------------------------------------------
				; |           METHOD 1: EXPLOITING LOOPS                   |
				; ----------------------------------------------------------
				
				; Initializing counters
counter			RN r5
				MOV  counter, #0			; Counts how many iteration of the loop have been performed
				
				; Loop uses the same operations of the previous method
loop			LDRH r1, [r0], #2		
				LDRH r2, [r0], #2			
				ADD  r1, r1, r2
				STR  r1, [r3], #4
				
				; Incrementing counter and compare in order to branch
				ADD  counter, counter, #1	; counter = counter + 1 
				CMP  counter, #3			; 3 instead of 4 since the first iteration is performed
											; out of the loop
				BNE  loop
		
stop			B stop

; Defining halfwords (16 bits) constants
myConstants		DCW 	57721, 56649, 15328, 60606, 51209, 8240, 24310, 42159

                ENDP