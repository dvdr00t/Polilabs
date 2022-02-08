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

; The Bus Fault Status Register is a byte register (R/W) specified 
; in the Nested Interrupt Vectored Controller (NVIC) at the address 0xE000ED29. It stores
; several different information for each bits. In particular:
; 	- bit[7] -> Indicates that the Bus Fault Address Register (BFAR) contains a valid bus fault address
; 	- bit[4] -> Stacking error: stack pointer is corrupted, or stack size became too large (reaching an 
;				undefined memory region), or PSP is used but not initialized
; 	- bit[3] -> Unstacking error or stack pointer changed during exception
; 	- bit[2] -> Imprecise data access violation: device not initialized, access of privileged-only device 
;				in user mode, incorrect transfer size for the device.
; 	- bit[1] -> Precise data access violation (fault address is in BFAR)
; 	- bit[0] -> Instruction access violation: branch to invalid region, invalid exception return code, 
;				invalid entry in interrupt vector table, access to NVIC in user mode.
BusFaultStatusRegister EQU 0xE000ED29
	
; The Bus Fault Address Register is a word register (R) specified 
; in the Nested Interrupt Vectored Controller (NVIC) at the address 0xE000ED38. It stores
; several different information for each bits.
BusFaultAddressRegister EQU 0xE000ED38
	
	


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
					
				; Loading the System Handler Control and State Register
				LDR r0, =SystemHandlerControlandStateRegister
				LDR r1, [r0]
				
				; Enabling Bus faults by setting bit[17] to 1
				; bin: 0000 0000 0000 0010 0000 0000 0000 0000
				; 0x :    0    0    0    2    0    0    0    0
				ORR r1, r1, #0x00020000
				
				; Storing back the result in the System Handler Control and State Register
				; Check in the NVIC when debugging!
				STR r1, [r0]   

				; +-------------------------------------+
				; |	       BUS FAULT EXCEPTION          |
				; +-------------------------------------+
				; Suppose to raise the Bus Fault exception by generating a stacking error: stack pointer is 
				; corrupted, or stack size became too large (reaching an undefined memory region), or PSP is used 
				; but not initialized.
				; In this case we want to raise the exception by using PSP without initializing it. 
				
				; First, we need to switch to PSP. In order to do so, we need to change CONTROL[1] to 1.
				MRS r0, CONTROL
				ORR r0, r0, #0x00000002	; bin: 0000 0000 0000 0000 0000 0000 0000 0010
				MSR CONTROL, r0
				
				; Raising the exception: PSP is not initialized (SP = 0x00000000)
				; However, by running the code in the simulation (remember: we are simulating the board since 
				; the board is not connected), nothing happens and the instruction is skipped. In case the code 
				; is run on the board, a Bus Fault Exception is raised.
				PUSH {r1}
				
                BX      R0
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
					
; The Bus Fault Handler has been enabled by setting the 17th bit in the System Handler Control and State Register
; to 1. The advantages of having the Bus Fault Handler enabled is that, if the code call the handler, we already know
; what caused the exception.
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
					
				; Loading the Bus Fault Status Register
				; NOTE THAT: this register is a BYTE register, therefore LDRB is required to
				; load a byte value.
				LDR  r0, =BusFaultStatusRegister
				LDRB r1, [r0]
				
				; NOTE THAT now, the register r1 contains the following value:
				; 	r1 = 0x00000092
				; which corresponds, in binary, to:
				; 	r1 = 2_0000 0000 0000 0000 0000 0000 1001 0010
				; The Bus Fault Status register is a byte register, therefore it contains the last byte of 
				; the value stored in r1, where:
				; 	- bit[7] is set to 1 -> valid bus fault is stored in BFAR noy
				; 	- bit[4] is set to 1 -> stacking error;
				; 	- bit[1] is set to 1 -> precise data access violation. 
				
				; Loading the Bus Fault Address Register
				LDR  r2, =BusFaultAddressRegister
				LDRB r3, [r2]
				
				; In r3 there is now the valid bus fault address
				; 	r3 = 0x1FADE120
					
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
