DIM EQU 15
.MODEL small
.STACK
.DATA
    VETT DW 2, 5, 16, 12, 34, 7, 20, 11, 31, 44, 70, 69, 2, 4, 23
    RESULT DW ? 
.CODE
.STARTUP
    XOR AX, AX          ; Zeroing AX 
    MOV CX, DIM         ; Array size is now stores in CX 
    XOR DI, DI          ; Zeroing DI 
sum:
    ADD AX, VETT[DI]    ; Add to AX the i-th element of VETT
    ADD DI, 2           ; Incrementing the pointer
    DEC CX              ; Keeping track of the number of operations left
    CMP CX, 0           ; Checking if the array has been fully scanned
    JNE sum             ; Reiterating if ZF = 0 (CX != 0 ---> CMP result is != 0)
    MOV RESULT, AX      ; Storing the result   
.EXIT
END
