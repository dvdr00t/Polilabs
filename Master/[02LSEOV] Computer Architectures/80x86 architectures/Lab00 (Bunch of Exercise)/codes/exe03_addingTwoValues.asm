.MODEL small
.STACK
.DATA
    OPD1 DW 10         ; Storing 000AH into OPD1 
    OPD2 DW 24         ; Storing 0018H into OPD1 
    RESULT DW ?        ; Creating a 16 bits variable
.CODE
.STARTUP
    MOV AX, OPD1       ; Loading the first operand to perform the ADD
    ADD AX, OPD2       ; Adding the second operand
    MOV RESULT, AX     ; Storing the result in the 16 bits variable
.EXIT
END