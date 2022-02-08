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

; +---------------------------------------+
; |										  |
; |        FAULT STATUS REGISTERS         |
; |										  |
; +---------------------------------------+

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
	
; The Hard Fault Status Register is a word register (R/W) specified 
; in the Nested Interrupt Vectored Controller (NVIC) at the address 0xE000ED2C. It stores
; several different information for each bits. In particular:
; 	- bit[31] -> Debug event (breakpoint or watchpoint).
; 	- bit[30] -> A bus fault, memory management fault, or usage fault occurred, 
;				 but the corresponding handler is disabled or cannot be started because 
;				 another exception with same or higher priority is running, or because exception 
; 				 mask is set.
; 	- bit[1] -> Failed interrupt vector fetch during exception processing (due to bus fault 
; 				or incorrect vector table offset setup).
HardFaultStatusRegister EQU 0xE000ED2C
	
; The Configuration Control Register is a word register (R/W) specified 
; in the Nested Interrupt Vectored Controller (NVIC) at the address 0xE000ED14. It stores
; several different information for each bits. In particular:
; 	- bit[9] -> Force exception stacking start in double word aligned address.
; 	- bit[8] -> Ignore data bus fault during hard fault and NMI.
; 	- bit[4] -> Trap on divide by 0
;	- bit[3] -> Trap on unaligned accesses
; 	- bit[1] -> If set to 1, allow user code to write to Software Trigger Interrupt register
; 	- bit[0] -> If set to 1, allow exception handler to return to thread state at any level by controlling return value.
ConfigurationControlRegister EQU 0xE000ED14
	
	
; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
				
				; Loading the System Handler Control and State Register
				LDR r0, =SystemHandlerControlandStateRegister
				LDR r1, [r0]
				
				; Enabling all faults by setting bit[16:18] to 1
				; bin: 0000 0000 0000 0111 0000 0000 0000 0000
				; 0x :    0    0    0    7    0    0    0    0
				ORR r1, r1, #0x00070000
				
				; Storing back the result in the System Handler Control and State Register
				; Check in the NVIC when debugging!
				STR r1, [r0]             
				
				; +-------------------------------------+
				; |	   CASE 1: wrong instruction set    |
				; +-------------------------------------+
				; Generating a Usage Fault Exception by jumping to a Thumb instruction
				; with the ARM instruction set enabled. 
				; Remember that, if the last bit of the address to jump to is 0, the instruction set 
				; used aftet the jump is the ARM instruction set. If the code area where the code jump 
				; to uses a Thumb instruction set, it generates a Usage Fault Exception (which leads to 
				; a Hard Fault Exception if the Usage Fault Exception is not enabled).
				ADRL r0, stop
				BX   r0
				
				; +-------------------------------------+
				; |	     CASE 2: division by zero       |
				; +-------------------------------------+
				; Generating a Usage Fault Exception by performing a division by zero.
				; NOTE THAT: before raising the exception it is necessary to enable the DIV_0_TRP in the 
				; Configuration Control Register at bit[4]. If the DIV_0_TRP is not enabled, the result of
				; division becomes zero. If this behavior is OK for the programmer, nothing has to be done; otherwise
				; write the code in a dedicated subroutine.
				
				; Enabling the DIV_0_TRP by loading the Configuration Control Register and setting bit[4] = 1 
				LDR r0, =ConfigurationControlRegister
				LDR r1, [r0]
				ORR r1, #0x00000010			; bin: 0100 0000 0000 0000 0000 0000 0001 0000
				STR r1, [r0]
				
				; Raising the exception
				MOV r0, #0x0
				MOV r1, #0x11111111			; Even if this istruction is too long, it is supported by the MOV
											; because it is of the type 0xaabbaabb where a = 1 and b = 1
				UDIV r2, r1, r0
				
				; +-------------------------------------+
				; |	     CASE 3: unaligned access       |
				; +-------------------------------------+
				; When a 32 bit addres is loaded, it is better that the address is a multiple of 4 (in 
				; order to exploit only one cycle for the load).
				; NOTE THAT: before raising the exception it is necessary to enable the UNALIGN_TRP in the 
				; Configuration Control Register at bit[8]. If the UNALIGN_TRP is not enabled, the result does not
				; change but it takes two cycle to be completed.
				
				; Enabling the UNALIGN_TRP by loading the Configuration Control Register and setting bit[3] = 1 
				LDR r0, =ConfigurationControlRegister
				LDR r1, [r0]
				ORR r1, #0x00000008		; bin: 0100 0000 0000 0000 0000 0000 0000 1000
				STR r1, [r0]
				
				; Raising the exception
				LDR r0, =var1
				LDR r1, [r0]			; Is r0 a multiple of 4?
				LDR r2, =var2          
				LDR r3, [r2]            ; For sure this is an UNALIGNED ACCESS since var2 starts 1B after var1
				
				; +-------------------------------------+
				; |	   CASE 4: undefined instruction    |
				; +-------------------------------------+
				; In case the compiler finds, at a certain point in the code, an OP Code which does not represent
				; any given instruction, it raises a Usage Fault Exception. As an example, suppose that 0xE7F0DEF0
				; does not correspond to any particular Thumb instruction. It is possible for the programmer to add 
				; it to the memory by doing: DCD 0xE7F0DEF0. However, by running the code in the simulation (remember:
				; we are simulating the board since the board is not connected), nothing happens and the instruction is
				; skipped. In case the code is run on the board, a Usage Fault Exception is raised.
				DCD 0xE7F0DEF0
				

stop            B	 stop

; Declaring some variables
var1			DCB 0x1					; For sure var1 starts at an even address
var2			DCB 0x2					; For sure var2 is placed at an odd address (1 Byte after var1)
var3			DCB 0x3, 0x4, 0x5
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
					
				; Loading the Hard Fault Status Register
				LDR  r0, =HardFaultStatusRegister
				LDRH r1, [r0]
				
				; Testing if the bit[30] of the register has been set to 1 ( meaning that a bus 
				; fault, memory management fault, or usage fault occurred, but the corresponding handler 
				; is disabled)
				TST r1, #0x40000000 ; bin: 0100 0000 0000 0000 0000 0000 0000 0010
				BNE faultNotEnabled
				; This section: hard fault exception due to other reasons

faultNotEnabled
				
				; Checking Bus Fault Status Register, Memory Management Statu Register or Usage
				; Fault Staus Register.
				
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
					
				; Loading the Usage Fault Status Register
				; NOTE THAT: this register is a HALF-WORD register, therefore LDRH is required to
				; load a half-word value.
				LDR  r0, =UsageFaultStatusRegister
				LDRH r1, [r0]
				
				; +-------------------------------------+
				; |	   CASE 1: wrong instruction set    |
				; +-------------------------------------+
				; Testing if the bit[1] of the register has been set to 1 (meaning that code branch target
				; address to PC with LSB equals to 0)
				TST r1, #0x00000002 ; bin: 0000 0000 0000 0000 0000 0000 0000 0010
				BNE handleBranchWith_LSB_0
				
				; +-------------------------------------+
				; |	     CASE 2: division by zero       |
				; +-------------------------------------+
				; Testing if the bit[9] of the register has been set to 1 (meaning that division by zero was 
				; performed and DIV_0_TRP was enabled)
				TST r1, #0x00000200 ; bin: 0000 0000 0000 0000 0000 0010 0000 0000
				BNE handleDivisionByZero
				
				; +-------------------------------------+
				; |	     CASE 3: unaligned access       |
				; +-------------------------------------+
				; Testing if the bit[8] of the register has been set to 1 (meaning that unaligned access was 
				; performed and UNALIGN_TRP was enabled)
				TST r1, #0x00000100 ; bin: 0000 0000 0000 0000 0000 0001 0000 0000
				BNE handleUnalignedAccess
				
				; +-------------------------------------+
				; |	   CASE 4: undefined instruction    |
				; +-------------------------------------+
				; Testing if the bit[0] of the register has been set to 1 (meaning that undefined instruction was 
				; tried to be run)
				TST r1, #0x00000001 ; bin: 0000 0000 0000 0000 0000 0000 0000 0001
				BNE handleUndefinedInstruction
				
				
				; This section: usage fault exception due to other reasons


; Manage the Usage Fault Exception when a branch with a wrong instruction set happens
handleBranchWith_LSB_0
				; The proble was due to the LSB = 0. I would like to return where I left
				; and act as nothing happened (with the right instruction set!)
				; NOTE THAT: the code is not returning to the address stored in r14 (LR). The LR stores
				; the address 0xFFFFFFF9 (EXEC_RETURN) return to thread mode with MSP. The correct address 
				; has been stored by the processor in the MSP. However, in that specific case, the return 
				; address stores in the MSP is 0x000000E0 (LSB is still 0) and again we return with ARM instruction 
				; set enabled. In order to solve the problem, it is necessary to force the T (Thumb) bit of 
				; the PSR to 1 before jumping back.
				
				; Before forcing the T bit to 1, it is necessary to understand in which stack (MSP or PSP?) the 
				; PSR was stored.
				; Checking if the EXEC_RETURN address has bit[2] set to 0 or 1:
				; 	- bit[2] == 1 then PSP was being used before the call
				; 	- bit[2] == 0 then MSP was being used before the call
				TST LR, #0x00000004 	; bin: 0000 0000 0000 0000 0000 0000 0000 0100 
				ITE EQ					; if (bit[2] == 1) {
				MRSEQ r0, MSP			; 		r0 = [MSP]
				MRSNE r0, PSP			; } else {r0 = [PSP]}
				
				; Now, remember that PSR is stored as the first value in the stack, followed by: 
				; PC, LR, r12, r3, r2, r1 and r0. So an offset of 28 bytes is required.
				; Loading the PSR in r1.
				LDR r1, [r0, #28]
				
				; NOTE THAT: the T (Thumb) bit is at position 24 in the PSR
				; Setting bit[24] = 1
				ORR r1, #0x01000000 	; bin: 0000 0001 0000 0000 0000 0000 0000 0000
				
				; Store back the PSR
				STR r1, [r0, #28]
				
				; Now I can return with Thumb instruction set
				BX LR
				
; Manage the Usage Fault Exception when a division by zero happens
handleDivisionByZero

; Manage the Usage Fault Exception when an unaligned access happens
handleUnalignedAccess

; Manage the Usage Fault Exception when an undefined instruction happens
handleUndefinedInstruction

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
