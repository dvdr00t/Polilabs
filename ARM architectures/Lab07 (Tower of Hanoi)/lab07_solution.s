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

Stack_Size      EQU     0x00000300

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
					
; +----------------------+
; | STACK IMPLEMENTATION |
; +-----------------------
; Note that stacks are implemented in a FULL DESCENDING way, i.e. the stack pointer 
; points to a FULL cell of the stack (the last one inserted) and it moves from the last 
; position (TOS) to the base position (BOS).
; -------------------------------------------------------------------------------------

				AREA	stacks_area, DATA, READWRITE
base_pointer_A	SPACE	40
stack_pointer_A	
base_pointer_B	SPACE	40
stack_pointer_B
base_pointer_C	SPACE	40
stack_pointer_C

; +------------------------------------+
; | DATA FOR THE INITIAL CONFIGURATION |
; +------------------------------------+
; Note that the insertion ends when one of the following two conditions occurs:
; 	- the constant to be entered is 0
; 	- the constant to be inserted is greater than the last element on the stack.
; -------------------------------------------------------------------------------------

				AREA	initial_config, DATA, READONLY
initial_data	DCD	9, 6, 3, 2, 1, 8, 7, 0, 5, 4, 0

                AREA    |.text|, CODE, READONLY


; +--------------+
; | MAIN ROUTINE |
; +--------------+
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
					
in_data_ptr		RN  r0			; Pointer to the set of data
stack_A_ptr		RN  r10		    ; Stack pointer A
stack_B_ptr     RN  r11		    ; Stack pointer B
stack_C_ptr     RN  r12		    ; Stack pointer C

				; LOADING DATA ADDRESSES INTO REGISTERS
				; ------------------------------------------
				LDR in_data_ptr, =initial_data
				LDR stack_A_ptr, =stack_pointer_A
				LDR stack_B_ptr, =stack_pointer_B
				LDR stack_C_ptr, =stack_pointer_C
				
				; FIRST CALL TO THE fill_stack ROUTINE
				; ------------------------------------------
				PUSH {stack_A_ptr, in_data_ptr}
				BL fill_stack
				POP {stack_A_ptr, in_data_ptr}
				
				; SECOND CALL TO THE fill_stack ROUTINE
				; ------------------------------------------
				PUSH {stack_B_ptr, in_data_ptr}
				BL fill_stack
				POP {stack_B_ptr, in_data_ptr}
				
				; THIRD CALL TO THE fill_stack ROUTINE
				; ------------------------------------------
				PUSH {stack_C_ptr, in_data_ptr}
				BL fill_stack
				POP {stack_C_ptr, in_data_ptr}
				
				; +-----------------------------+
				; | EXECUTING A SINGLE MOVEMENT |
				; +-----------------------------+
				; It is possible to move a disc from one pole to another if one of 
				; two conditions occurs:
				;    - the new pole is empty
				;    - the upper disc in the new pole has a larger size than the
				;      disk to be moved.
				; ------------------------------------------------------------------------
return_value    RN r0

				; TESTING MOVE C --> B (4 on 7)
				; -------------------------------------
				; passing parameters
				PUSH {stack_C_ptr}
				PUSH {stack_B_ptr}
				PUSH {return_value}
				
				; calling the function
				BL  move1
				
				; retrieving results
				POP {return_value}
				POP {stack_B_ptr}
				POP {stack_C_ptr}
				
				
				; +------------------------------+
				; | EXECUTING MULTIPLE MOVEMENTS |
				; +------------------------------+
				; It is possible to move N disks from pole X to pole Y using a recursive 
				; procedure:
				;    - Move the first N-1 disks from X to Z
				;    - Move disk N from X to Y
				;    - Move N-1 disks from Z to Y
				; ------------------------------------------------------------------------
N			    RN r1

                ; TESTING 3 DISKS MOVEMENT A --> B (C is auxiliar)
				; -------------------------------------------------
				MOV N, #3
				
				; Passing parameters
				PUSH {stack_A_ptr}       ; stack_pointer_start
				PUSH {stack_B_ptr}       ; stack_pointer_end
				PUSH {stack_C_ptr}       ; stack_pointer_aux
				PUSH {N}                 ; N
				
				; calling the function
				BL moveN
				
				; retrieving results
				POP {N}
				POP {stack_C_ptr}
				POP {stack_B_ptr}
				POP {stack_A_ptr}

stop			B	stop
                ENDP
					
; +-------------------------------------------+
; | fill_stack SUBROUTINE TO INITIALIZE POLES |
; +-------------------------------------------+
; 
; fill_stack(stack_pointer_X, data_AREA_pointer)
;
;	PARAMETERS:
; 		- stack_pointer_X: the stack pointer associated to the pole, where X = {A, B, C};
;		- data_AREA_pointer: the address to a READONLY area containing a sequence of constants.
;
;		r6 stores the stack_pointer_X
;		r7 stores the data_AREA_pointer
; 
; 	RESULTS:
; 		- it fills the pole with the initial_config data;
; 		- it updates the stack pointer to the last entered data;
; 		- it updates the data AREA poinyer to the address in the READONLY area 
;				which is subsequent to the last constant entered.
; -----------------------------------------------------------------------------------------------
fill_stack		PROC
				PUSH {LR}
				
				; RETRIEVING REGISTER AND ADDRESSES
				; r6 and r7 works as local variables to the subroutine
				; in order to store the addresses of stack and data, edit
				; them and pushing them back into the stack (where they
				; will be retrieved by the main routine).
				; -------------------------------------------------------
				LDR  r7, [SP, #4]	        ; LOCAL in_data_ptr
				LDR  r6, [SP, #8]	        ; LOCAL stack_X_ptr
				
				; -----------------------
				;     DO...WHILE LOOP
				; -----------------------
				; do {
				; 		old_value = initial_data[in_data_ptr]
				;		stack_X[stack_X_ptr] = old_value
				;		in_data_ptr++		
				;		new_value = initial_data[in_data_ptr]
				;
				; } while (old_value > new_value || new_value != 0)
				;
			
old_value       RN r8
new_value       RN r9
add_a_value		LDR   old_value, [r7]
				STMFD r6!, {old_value}      
				ADD   r7, r7, #4            ; in_data_ptr++
				LDR   new_value, [r7]
				CMP   new_value, #0         ; check if new_value == 0
				BEQ   remove_zero            
				CMP   old_value, new_value  ; if not, check if old_value > new_value
				BCS   add_a_value           ; and repeat
				; else
next_stack		STR  r7, [SP, #4]	        ; original in_data_ptr
				STR  r6, [SP, #8]	        ; stack_X_ptr
				POP  {PC}
remove_zero		ADD   r7, r7, #4            ; in_data_ptr++
				B     next_stack
				ENDP
					
; +----------------------------------------+
; | move1 SUBROUTINE TO MOVE A SINGLE DISK |
; +----------------------------------------+
; 
; move1(stack_pointer_start, stack_pointer_end, return_value)
;
;	PARAMETERS:
; 		- stack_pointer_start: the stack pointer FROM which the disk should be moved
;       - stack_pointer_end: the stack pointer TO which the disk should be moved
;		- return_value: space for the returned value (1 on success, 0 otherwise).
;                       Stack pointers are updated if the disk has been moved.
;    
;       r5 stores the result value
;		r6 stores the stack_pointer_start
;		r7 stores the stack_pointer_end
;       r8 stores the value to be moved
;       r9 stored the upper-most value in the destination stack
; -----------------------------------------------------------------------------------------------	
move1			PROC
	
				; PRESERVING ALL THE REGISTERS USED IN THE ROUTINE
				; Some of them may store some useful data during the moveN
				; subroutine call or the main routine.
				; --------------------------------------------------------
				PUSH {r5, r6, r7, r8, r9, LR}
				
				; RETRIEVING THE ADDRESSES NEEDED
				; r6 and r7 works as local variables to the subroutine
				; in order to store the addresses of stack, edit
				; them and pushing them back into the stack (where they
				; will be retrieved by the main routine).
				; -----------------------------------------------------------
				LDR  r6, [SP, #32]	; LOCAL stack_pointer_start
				LDR  r7, [SP, #28]	; LOCAL stack_pointer_end
				
				; RETRIEVING ELEMENT IN SOURCE STACK AND DESTINATION STACK
				; r8 and r9 works as local variables to the subroutine
				; in order to store the values data, edit and pushing them 
				; back into the stack (where they will be retrieved by the 
				; main routine).
				; ----------------------------------------------------------
				LDMFD r6, {r8}
				LDMFD r7, {r9}
				
				; CHECKING IF DISK CAN BE MOVED
				; --------------------------------
				CMP r9, r8              ; r9: disk_dst, r8: disk_src
				BLS move_invalid        ; if disk_dst <= disk_src
				; else
				STMFD r7!, {r8}             ; LOCAL stack_pointer_end--    (r7)
				ADD  r6, r6, #4             ; LOCAL stack_pointer_start++  (r6)
				MOV  r5, #1                 ; LOCAL return_value = 1       (r5)
				
				STR  r5, [SP, #24]          ; ORIGINAL return_value
				STR  r6, [SP, #32]	        ; ORIGINAL stack_pointer_start
				STR  r7, [SP, #28]	        ; ORIGINAL stack_pointer_end
				POP  {r5, r6, r7, r8, r9, PC}
				
move_invalid    MOV  r5, #0                 ; LOCAL return_value = 0       (r5)
				STR  r5, [SP, #24]          ; ORIGINAL return_value
				STR  r6, [SP, #32]	        ; ORIGINAL stack_pointer_start
				STR  r7, [SP, #28]	        ; ORIGINAL stack_pointer_end
				POP  {r5, r6, r7, r8, r9, PC}
				ENDP
					
; +-----------------------------------------+
; | moveN SUBROUTINE TO MOVE MULTIPLE DISKS |
; +-----------------------------------------+
; 
; moveN(stack_pointer_start, stack_pointer_end, stack_pointer_aux, N)
;
;	PARAMETERS:
; 		- stack_pointer_start: the stack pointer FROM which the disk should be moved
;       - stack_pointer_end: the stack pointer TO which the disk should be moved
;		- stack_pointer_aux: the stack pointer of the support stack
;       - N: number of moves
;
; PSEUDOCODE:
; 
; M = 0;                  // M is the number of movs made
; if (N == 1) {
;     move1(X, Y, a);
;     M = M + a;          // a is the return value: 0-1
; } else {
;     moveN(X, Z, Y, N-1); 
;     M = M + b;
;     move1(X, Y, a);
; 
;     if (a == 0) {
;          return; 
;     } else { 
;          M = M + 1; 
;     }
;     moveN(Z, Y, X, N-1); 
;     M = M + c;
; }
; 
; -----------------------------------------------------------------------------------------------	
moveN			PROC
M               RN r5
				
				; PRESERVING ALL THE REGISTERS USED IN THE ROUTINE
				; Some of them may store some useful data in the main routine
				; ------------------------------------------------------------
				PUSH {r5, r6, r7, r8, r9, LR}
				
				; RETRIEVING PARAMETERS
				; r6: stores stack_pointer_start
				; r7: stores stack_pointer_end
				; r8: stores stack_pointer_aux
				; r9: stores N
				; ----------------------------
				LDR r9, [SP, #24]          ; LOCAL N
				LDR r8, [SP, #28]          ; LOCAL stack_pointer_aux
				LDR r7, [SP, #32]          ; LOCAL stack_pointer_end
				LDR r6, [SP, #36]          ; LOCAL stack_pointer_start
				

				; M = 0
				; if (N == 1) {
				;     move1(X, Y, a);
				;     M = M + a;          // a is the return value: 0-1
				; }
				; -------------------------------------------------------
				MOV M, #0
				CMP r9, #1
				BEQ call_move1
				
				; ELSE CALL moveN(X, Z, Y, N-1)
				; -------------------------------------------------------
				SUB r9, r9, #1         ; N--
				PUSH{r6}               ; LOCAL stack_pointer_start
				PUSH{r8}               ; LOCAL stack_pointer_aux
				PUSH{r7}               ; LOCAL stack_pointer_end
				PUSH{r9}               ; LOCAL N
				BL moveN
				
				; RESTORING STACK
				; --------------------------------------------------------
				POP {r9}               ; LOCAL N
				POP {r7}               ; LOCAL stack_pointer_end
				POP {r8}               ; LOCAL stack_pointer_aux
				POP {r6}               ; LOCAL stack_pointer_start
				
				; M = M + (N-1)
				ADD M, M, r9
				ADD r9, r9, #1         ; Restoring N
				
				; CALL move1(X, Y, a)
				; --------------------------------------------------------
				PUSH {r6}              ; LOCAL stack_pointer_start
				PUSH {r7}              ; LOCAL stack_pointer_end
				PUSH {r0}              ; LOCAL return_value
				BL move1
				POP {r0}               ; LOCAL return_value
				POP {r7}               ; LOCAL stack_pointer_end
				POP {r6}               ; LOCAL stack_pointer_start
				
				; if returned_value == 0 -> return
				; ----------------------------------
				CMP r0, #0
				BEQ end_procedure
				
				; else
				;-----------------------------------
				ADD M, M, #1
				
				; moveN(Z, Y, X, N-1); 
				; M = M + c;
				; ----------------------------------------------------------
				SUB r9, r9, #1
				PUSH{r8}               ; LOCAL stack_pointer_aux
				PUSH{r7}               ; LOCAL stack_pointer_end
				PUSH{r6}               ; LOCAL stack_pointer_start
				PUSH{r9}               ; LOCAL N
				BL moveN
				POP {r9}               ; LOCAL N
				POP {r6}               ; LOCAL stack_pointer_start
				POP {r7}               ; LOCAL stack_pointer_end
				POP {r8}               ; LOCAL stack_pointer_aux
				
				; M = M + (N-1)
				ADD M, M, r9
				ADD r9, r9, #1         ; Restoring N
				
				; END POINT OF moveN IF N != 1
				; -----------------------------------------
				STR r8, [SP, #28]          ; LOCAL stack_pointer_aux
				STR r7, [SP, #32]          ; LOCAL stack_pointer_end
				STR r6, [SP, #36]          ; LOCAL stack_pointer_start 
				POP {r5, r6, r7, r8, r9, PC}

				; +-------------------------+
				; | CALLING move1 PROCEDURE |
				; +-------------------------+
				; Subroutine used to prepare the stack for the move1
				; procedure
				; -----------------------------------------------------------
call_move1      ; passing parameters
				PUSH {r6}        ; LOCAL stack_pointer_start
				PUSH {r7}        ; LOCAL stack_pointer_end
				PUSH {r0}        ; LOCAL return_value
				
				; calling the function
				BL  move1
				
				; retrieving results
				POP {r0}         ; LOCAL return_avlue
				POP {r7}         ; LOCAL stack_pointer_end
				POP {r6}	     ; LOCAL stack_pointer_start
				
				; M = M + return_value
				ADD M, M, r0
				
				; END POINT OF moveN IF N == 1
				; -----------------------------------------
				STR r8, [SP, #28]          ; LOCAL stack_pointer_aux
				STR r7, [SP, #32]          ; LOCAL stack_pointer_end
				STR r6, [SP, #36]          ; LOCAL stack_pointer_start 
				POP {r5, r6, r7, r8, r9, PC}
				
end_procedure	POP {r5, r6, r7, r8, r9, PC} 
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
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
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
