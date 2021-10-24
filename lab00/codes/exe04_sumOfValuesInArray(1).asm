.MODEL small
.STACK
.DATA
    VETT DW 5, 7, 3, 4, 3      ; Defining the array values
    RESULT DW ?                ; Creating a variable to store the result
.CODE
.STARTUP
    XOR AX, AX                 ; Zeroing AX
    ADD AX, VETT               ; Starting the sum (manually)
    ADD AX, VETT+2 
    ADD AX, VETT+4 
    ADD AX, VETT+6 
    ADD AX, VETT+8 
    MOV RESULT, AX            ; Storing the result in the RESULT variable
.EXIT
END