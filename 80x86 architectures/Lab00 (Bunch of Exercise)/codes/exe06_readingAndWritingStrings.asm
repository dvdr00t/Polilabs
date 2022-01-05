DIM EQU 20
.MODEL small
.STACK
.DATA
    VETT DB DIM DUP(?)
.CODE
.STARTUP
    MOV CX, DIM           ; Storing in the counter the number of operations to be performed by the loop
    XOR DI, DI            ; Zeroing DI
    MOV AH, 1             ; Set AH for reading    
reading:
    INT 21H               ; Reading a character
    MOV VETT[DI], AL      ; Storing read character 
    INC DI                ; Incrementing index
    DEC CX                ; Decrementing counter to keep track of operations left
    CMP CX, 0             ; Performing <CX> - 0. If <CX> = 0 --> CF = 1, else CF = 0
    JNE reading           ; Repeat if <CX> != 0 (i.e. CF = 0)
    MOV CX, DIM           ; Recharging the dimension for writing
    MOV AH, 2             ; Set AH to writing
writing: 
    DEC DI                ; Starting from the last position (printing backwards)
    MOV DL, VETT[DI]      ; Storing character to write
    INT 21H               ; Writing a character
    DEC CX                ; Decrementing counter to keep track of operations left
    CMP CX, 0             ; Performing <CX> - 0. If <CX> = 0 --> CF = 1, else CF = 0
    JNE writing           ; Repeat if <CX> != 0 (i.e. CF = 0)
.EXIT
END