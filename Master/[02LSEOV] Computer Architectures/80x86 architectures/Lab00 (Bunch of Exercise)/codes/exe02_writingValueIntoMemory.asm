.MODEL small
.STACK
.DATA    
    VAR DW ?
.CODE
.STARTUP
    MOV VAR, 111    ; Storing 006FH into VAR
.EXIT
END