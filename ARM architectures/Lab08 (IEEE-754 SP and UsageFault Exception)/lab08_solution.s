;/**************************************************************************//**
; * @file     startup_LPC17xx.s
; * @brief    CMSIS Cortex-M3 Core Device Startup File for
; *           NXP LPC17xx Device Series
; * @version  V1.10
; * @date     06. April 2011
; *
; * @note
; * Copyright (C) 2009-2011 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/

; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     WDT_IRQHandler            ; 16: Watchdog Timer
                DCD     TIMER0_IRQHandler         ; 17: Timer0
                DCD     TIMER1_IRQHandler         ; 18: Timer1
                DCD     TIMER2_IRQHandler         ; 19: Timer2
                DCD     TIMER3_IRQHandler         ; 20: Timer3
                DCD     UART0_IRQHandler          ; 21: UART0
                DCD     UART1_IRQHandler          ; 22: UART1
                DCD     UART2_IRQHandler          ; 23: UART2
                DCD     UART3_IRQHandler          ; 24: UART3
                DCD     PWM1_IRQHandler           ; 25: PWM1
                DCD     I2C0_IRQHandler           ; 26: I2C0
                DCD     I2C1_IRQHandler           ; 27: I2C1
                DCD     I2C2_IRQHandler           ; 28: I2C2
                DCD     SPI_IRQHandler            ; 29: SPI
                DCD     SSP0_IRQHandler           ; 30: SSP0
                DCD     SSP1_IRQHandler           ; 31: SSP1
                DCD     PLL0_IRQHandler           ; 32: PLL0 Lock (Main PLL)
                DCD     RTC_IRQHandler            ; 33: Real Time Clock
                DCD     EINT0_IRQHandler          ; 34: External Interrupt 0
                DCD     EINT1_IRQHandler          ; 35: External Interrupt 1
                DCD     EINT2_IRQHandler          ; 36: External Interrupt 2
                DCD     EINT3_IRQHandler          ; 37: External Interrupt 3
                DCD     ADC_IRQHandler            ; 38: A/D Converter
                DCD     BOD_IRQHandler            ; 39: Brown-Out Detect
                DCD     USB_IRQHandler            ; 40: USB
                DCD     CAN_IRQHandler            ; 41: CAN
                DCD     DMA_IRQHandler            ; 42: General Purpose DMA
                DCD     I2S_IRQHandler            ; 43: I2S
                DCD     ENET_IRQHandler           ; 44: Ethernet
                DCD     RIT_IRQHandler            ; 45: Repetitive Interrupt Timer
                DCD     MCPWM_IRQHandler          ; 46: Motor Control PWM
                DCD     QEI_IRQHandler            ; 47: Quadrature Encoder Interface
                DCD     PLL1_IRQHandler           ; 48: PLL1 Lock (USB PLL)
                DCD     USBActivity_IRQHandler    ; 49: USB Activity interrupt to wakeup
                DCD     CANActivity_IRQHandler    ; 50: CAN Activity interrupt to wakeup


                IF      :LNOT::DEF:NO_CRP
                AREA    |.ARM.__at_0x02FC|, CODE, READONLY
CRP_Key         DCD     0xFFFFFFFF
                ENDIF


                AREA    |.text|, CODE, READONLY
			
; The System Handler Control and State Register is a word register (R/W) specified 
; in the Nested Interrupt Vectored Controller (NVIC) at the address 0xE000ED24. It stores
; several different information for each bits. In particular:
; 	- bit[18] -> Usage Fault Handler is enable
; 	- bit[17] -> Bus Fault Handler is enable
; 	- bit[16] -> Memory Management Fault Handler is enable
; 	- etc...
SystemHandlerControlandStateRegister EQU 0xE000ED24

; The Usage Fault Status Register is a half-word register (R/W) specified 
; in the Nested Interrupt Vectored Controller (NVIC) at the address 0xE000ED2A. It stores
; several different information for each bits. In particular:
; 	- bit[9] -> Division by zero and DIV_0_TRP is set
; 	- bit[8] -> Unaligned memory access attempted and UNALIGN_TRP is set
; 	- bit[3] -> Attempt to execute a coprocessor instruction
;	- bit[2] -> ...
; 	- bit[1] -> Branch target address to PC with LSB equals 0
; 	- bit[0] -> Use of not supported (undefined) instruction
UsageFaultStatusRegister EQU 0xE000ED2A

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]

; +---------------------------------------------+
; | LAB08:              PART II                 |
; |           COPROCESSOR FAULT MANAGEMENT      |
; +---------------------------------------------+
; It is possible to call a coprocessor as follows:
;    CDP proc, imm, dest, op1, op2, sign
; where:
;    - CDP: instruction to call a coprocessor.
;    - proc: called coprocessor. We call p0.
;    - imm: operation executed by coprocessor (imm = 1 for conversion to the IEEE-754 SP).
;    - dest: register containing result.
;    - op1: register containing integer part.
;    - op2: register containing fractional part.
;    - sign: 0 if positive, 1 if negative.
; -----------------------------------------------------------------------------
; Cortex-M3 has not any coprocessor therefore CDP raises a usage fault.
; NOTE THAT: usage fault MUST BE ENABLED, otherwise a hard fault is raised.
; The conversion to the IEEE-754 SP format is done in the exception handler.
; -----------------------------------------------------------------------------
; OPERATIONS TO BE PERFORMED:
; 1) Check the proper bit in the Usage Fault Status Register. If the exception is due to a 
;    coprocessor instruction, branch to the corresponding piece of code. Otherwise, write a 
;    dummy implementation of other usage faults (e.g., B .).
; 2) Before entering the exception handler, PC is saved in the stack (MSP or PSP) with offset 24
;    (use of MSP or PSP is determined by reading LR). Retrieve the instruction and undertantd
;     if it is required to perform the IEEE-754 SP conversion.
; 3) Usage faults return to the same instructions that triggered the fault. Since we do 
;    not want to execute CDP again after the handler, the return address must be updated to 
;    the next instruction. Update the value of PC saved in the stack (with offset 24) with the 
;    new value PC + 4.
; -----------------------------------------------------------------------------
				; Loading the System Handler Control and State Register
				LDR r0, =SystemHandlerControlandStateRegister
				LDR r1, [r0]
				
				; Enabling the usage fault by setting bit[18] to 1
				; bin: 0000 0000 0000 0100 0000 0000 0000 0000
				; 0x :    0    0    0    4    0    0    0    0
				ORR r1, r1, #0x00040000
				
				; Storing back the result in the System Handler Control and State Register
				; Check in the NVIC when debugging!
				STR r1, [r0] 

				; RAISING THE EXCEPTION
				; Values are passed in the registers c0 (r4) and c1 (r5), while the result is 
				; obtained in the register c7 (r11)
				; ---------------------------------------------------------------------------------
integer_part	RN r4
fractional_part RN r5
				LDR integer_part,    =0x000007CE		; 0x000007CE = 1998 
				LDR fractional_part, =0x087F91CD		; 0x087F91CD = 142578125
				
				CDP p0, #1, c7, c0, c1, #0
				
stop			B	stop
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
					
; The Usage Fault Handler has been enabled by setting the 18th bit in the System Handler Control and State Register
; to 1. The advantages of having the Usage Fault Handler enabled is that, if the code call the handler, we already know
; what caused the exception.
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
					
				; +---------------------+
				; | 1) Check proper bit |
				; +---------------------+
					
				; Loading the Usage Fault Status Register
				; NOTE THAT: this register is a HALF-WORD register, therefore LDRH is required to
				; load a half-word value.
				LDR  r2, =UsageFaultStatusRegister
				LDRH r3, [r2]
				
				; Testing if the bit[3] of the register has been set to 1 (meaning that it has been
				; attempted to execute a coprocessor instruction
				TST r3, #0x00000008 ; bin: 0000 0000 0000 0000 0000 0000 0000 1000
				BNE raisedCoprocessor
				B   handleEverythingElse
					
				; All other faults end in an infinite loop

raisedCoprocessor
                ; +--------------------------------------+
				; | 2) Recognizing offending instruction |
				; +--------------------------------------+
				; Load 4 bytes (1 word) from the address of PC. Due to little endianness, the 
				; two halfwords must be switched (e.g., by rotating 16 positions).
				; ---------------------------------------------------------------------------------
				; It is necessary to understand in which stack (MSP or PSP?) the PSR was stored.
				; Checking if the EXEC_RETURN address has bit[2] set to 0 or 1:
				; 	- bit[2] == 1 then PSP was being used before the call
				; 	- bit[2] == 0 then MSP was being used before the call
				TST LR, #0x00000004 	; bin: 0000 0000 0000 0000 0000 0000 0000 0100 
				ITE EQ					; if (bit[2] == 1) {
				MRSEQ r0, MSP			; 		r0 = [MSP]
				MRSNE r0, PSP			; } else {r0 = [PSP]}
				
				LDR r1, [r0, #24]       ; Restoring PC before the exception was raised
				LDR r1, [r1]            ; Retrieving the OPCODE of the instruction pointed by PC
				ROR r1, r1, #16         ; (Little endian! switching halfwords)
				PUSH {r1}               ; We will need r1 later to extract source registers
				AND r1, #0x00100000     ; If the instruction is CDP p0, #1, ... 
				                        ; execute code for IEEE-754 SP conversion.
										; bits are [xxxx 1110 0001 xxxx xxxx 0000 xxxx xxxx]
				CMP r1, #0x00100000
				BEQ handleConversion
				
				; else
				; +----------------------------+
				; | 3) Changing return address |
				; +----------------------------+
chg_ret_addr	LDR r7, [r0, #24]
				ADD r7, r7, #4
				MOV PC, r7
				BX LR

handleConversion			

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
									
exponent        RN r2
mantissa        RN r3

				; +----------------------------------+
				; | RESTORING VALUES TO BE CONVERTED |
				; +----------------------------------+
				POP {r1}
				PUSH {r4, r5, r6, r7, r8, r9, r10, r11}
				PUSH {r1}
				PUSH {r1}
				PUSH {r1}
				MOV r2, #4                  ; Offset for retrieving the values indexed
				AND r1, #0x000F0000         ; Masking-out source index
				LSR r1, r1, #16             ; Address of the first operand (integer_part)
				MUL r1, r1, r2              ; index*4 points to the value
				ADD r1, r1, #12             ; actually it is (index*4)+12 due to the pushes{r1}
				LDR r4, [SP, r1]            ; r4 = value[Source_0]
				POP {r1}                    ; Restoring r1
				AND r1, #0x0000000F         ; Address of the second operand (fractional_part)
				MUL r1, r1, r2              ; index*4 points to the value
				ADD r1, r1, #8              ; actually it is (index*4)+4 due to the pushes{r1}
				LDR r5, [SP, r1]            ; r1 = value[Source_1]
				POP {r1}                    ; Restoring r1
				AND r1, #0x00000020         ; Bit[5] indicates the sign
				ROR r10, r1, #6
				
				
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
				MRS r3, APSR							; Accessing APSR ...
				AND r3, r3, #0x20000000					; ... in order to access Carry Flag (PSR[29])
				CMP r3, #0x20000000						; ALternatively, use BC (Branch with Carry)
				BEQ firstBit
				
				; Decreasing counter
				SUB r2, r2, #1
				
				; Test condition
				CMP r2, #0
				BNE exponentLoop
				
				; Position of first bit is stored in r2
firstBit		MOV mantissa, integer_part
				POP{integer_part}
				ADD exponent, exponent, #127					; Adding BIAS = 127
				
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
N				RN r6
P               RN r7
remaining_mant  RN r9               ; Remaining mantissa is filled at every iteration from the right
				
				; COMPUTING N
				; From the exponent, it is possible to know how many bits left in the mantissa we 
				; had, doing:
				;                 N = 23 - (exponent - 127)
				; ---------------------------------------------------------------------------------
				MOV N, #0x17                       ; 0x17 = 23
				SUB r8, exponent, #0x7F            ; 0x7F = 127
				SUB N, N, r8
				
				; COMPUTING P
				; Let P start from 1 and be multiplied by a factor 10 every iteration of the loop
				; until P is greater than X (fractional_part).
				; ---------------------------------------------------------------------------------
				MOV P, #0x01                       
				MOV r8, #0xA                       ; factor of mutiplication
computeP        MUL P, P, r8
				CMP P, fractional_part
				BLE computeP 
				
				; MANTISSA LOOP
				; ---------------------------------------------------------------------------------
				PUSH {fractional_part}             ; Preserving fractional_part
				MOV r8, #0x02                      ; Multiplication factor stored in r6
				MOV remaining_mant, #0x00    
				
mantissaLoop    MUL fractional_part, fractional_part, r8
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
				ADD r10, r10, mantissa
				LSL exponent, exponent, #23
				ADD r10, r10, exponent
				
				; Restoring fractional_part
				POP {fractional_part}
				
				; STORING BACK THE RESULT
				; The destination register will contains the IEEE-754 SP representation, 
				; since the corresponding entry in the stack was updated before.
				; ----------------------------------------------------------------------------
				POP {r1}
				MOV r2, #4                  ; Offset for retrieving the values indexed
				AND r1, #0x0000F000         ; Masking-out source index
				LSR r1, r1, #12             ; Address of the first operand (integer_part)
				MUL r1, r1, r2              ; index*4 points to the value
				STR r10, [SP, r1]
				
				; Returning to main routine
				POP {r4, r5, r6, r7, r8, r9, r10, r11}
				B chg_ret_addr
				
handleEverythingElse			
                B       .
                ENDP
				
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WDT_IRQHandler            [WEAK]
                EXPORT  TIMER0_IRQHandler         [WEAK]
                EXPORT  TIMER1_IRQHandler         [WEAK]
                EXPORT  TIMER2_IRQHandler         [WEAK]
                EXPORT  TIMER3_IRQHandler         [WEAK]
                EXPORT  UART0_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  UART2_IRQHandler          [WEAK]
                EXPORT  UART3_IRQHandler          [WEAK]
                EXPORT  PWM1_IRQHandler           [WEAK]
                EXPORT  I2C0_IRQHandler           [WEAK]
                EXPORT  I2C1_IRQHandler           [WEAK]
                EXPORT  I2C2_IRQHandler           [WEAK]
                EXPORT  SPI_IRQHandler            [WEAK]
                EXPORT  SSP0_IRQHandler           [WEAK]
                EXPORT  SSP1_IRQHandler           [WEAK]
                EXPORT  PLL0_IRQHandler           [WEAK]
                EXPORT  RTC_IRQHandler            [WEAK]
                EXPORT  EINT0_IRQHandler          [WEAK]
                EXPORT  EINT1_IRQHandler          [WEAK]
                EXPORT  EINT2_IRQHandler          [WEAK]
                EXPORT  EINT3_IRQHandler          [WEAK]
                EXPORT  ADC_IRQHandler            [WEAK]
                EXPORT  BOD_IRQHandler            [WEAK]
                EXPORT  USB_IRQHandler            [WEAK]
                EXPORT  CAN_IRQHandler            [WEAK]
                EXPORT  DMA_IRQHandler            [WEAK]
                EXPORT  I2S_IRQHandler            [WEAK]
                EXPORT  ENET_IRQHandler           [WEAK]
                EXPORT  RIT_IRQHandler            [WEAK]
                EXPORT  MCPWM_IRQHandler          [WEAK]
                EXPORT  QEI_IRQHandler            [WEAK]
                EXPORT  PLL1_IRQHandler           [WEAK]
                EXPORT  USBActivity_IRQHandler    [WEAK]
                EXPORT  CANActivity_IRQHandler    [WEAK]

WDT_IRQHandler
TIMER0_IRQHandler
TIMER1_IRQHandler
TIMER2_IRQHandler
TIMER3_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
PWM1_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
I2C2_IRQHandler
SPI_IRQHandler
SSP0_IRQHandler
SSP1_IRQHandler
PLL0_IRQHandler
RTC_IRQHandler
EINT0_IRQHandler
EINT1_IRQHandler
EINT2_IRQHandler
EINT3_IRQHandler
ADC_IRQHandler
BOD_IRQHandler
USB_IRQHandler
CAN_IRQHandler
DMA_IRQHandler
I2S_IRQHandler
ENET_IRQHandler
RIT_IRQHandler
MCPWM_IRQHandler
QEI_IRQHandler
PLL1_IRQHandler
USBActivity_IRQHandler
CANActivity_IRQHandler

                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
