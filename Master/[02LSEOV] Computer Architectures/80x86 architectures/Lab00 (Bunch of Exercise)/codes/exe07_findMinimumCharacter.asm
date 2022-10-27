.MODEL small
.STACK
    DIM EQU 20
.DATA     
    TABLE DB DIM DUP(?)
.CODE
.STARTUP

    MOV CX, DIM           ; Storing the dimension to perform checkings
    LEA DI, TABLE         ; Load TABLE in DI
    MOV AH, 1             ; Setting AH to read

reading: 
    INT 21H               ; Reading a character
    MOV [DI],  AL         ; Storing the read character in the table
    INC DI                ; Moving index of TABLE
    DEC CX                ;\
    CMP CX, 0             ; } while(CX != 0) {keep reading}
    JNE reading           ;/
    MOV CL, 0FFH          ; Setting current minimum to 255_dec
    XOR DI, DI            ; Zeroing index

comparing: 
    CMP CL, TABLE[DI]     ; Compare with current minimum
    JB next_compare       ; New minimum not found --> skip
    MOV CL, TABLE[DI]     ; New minimum found     --> storing new minimum
next_compare:
    INC DI                ; Moving index
    CMP DI, DIM           ;\
    JE output             ; } if (DI == DIM) {output} else {keep comparing}
    JMP comparing         ;/
    
output:
    MOV DL, CL            ; Preparing output
    MOV AH, 2             ; Setting AH to write
    INT 21H               ; Write result    
    
.EXIT
END                                    