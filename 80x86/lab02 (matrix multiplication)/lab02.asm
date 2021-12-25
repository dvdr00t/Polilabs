; +------------------+
; | GLOBAL CONSTANTS |
; +------------------+
; 
;   Matrix A with size NxM 
;   Matrix B with size MxP
;   Matric C (result matrix) with size NxP
; ----------------------------------------------------------------------------
N EQU 2  
M EQU 2
P EQU 2


.MODEL small
.STACK
.DATA

    ; +-----------------+
    ; | DATA STRUCTURES |
    ; +-----------------+ 
    ; 
    ; Elements of the two matrices are signed numbers expressed with one byte. 
    ; The result is a matrix with N rows and P columns. The matrix contains 
    ; sign numbers expressed with one word.
    ;                   
    ; ------------------------------------------------------------------------
    ; !NOTE (IMPORTANT)!:
    ;  
    ;           matA is stored with a COLUMN MAJOR ORDERING
    ;           matB is stored with a ROW MAJOR ORDERING
    ; ------------------------------------------------------------------------
    ; In this example, the operation performed is the following:
    ; 
    ; [DECIMAL]
    ; 
    ;  /255 255\     /255 255\     / OF OF \ 
    ; (         ) x (         ) = (         )
    ;  \ 1   2 /     \ 1   2 /     \257 259/
    ;
    ; [HEXADECIAML]
    ; 
    ;  /FFh FFh\     /FFh FFh\     /  OF   OF  \ 
    ; (         ) x (         ) = (             )
    ;  \01h 02h/     \01h 02h/     \0101h 0103h/ 
    ; 
    ; where OF is replaced by the largest signed integer that can be
    ; represented in a word data type, i.e. 7FFFh (32767)
    ; 
    ; ------------------------------------------------------------------------
    matA DB 0FFh, 1, 0FFh, 2          ; First matrix  NxM (bytes) 
    matB DB 0FFh, 0FFh, 1, 2          ; Second matrix MxP (bytes)
         
    matC DW N*P DUP(?) ; Result matrix NxP (words)
    
    ; Messages
    ; ------------------------------------------------------------------------
    matC_msg     DB 'The result matrix is the following: ', '$'
    new_line_msg DB 0Dh, 0Ah, '$'
    ; ------------------------------------------------------------------------ 
    
.CODE
.STARTUP
    
    ; +---------------------------------------+
    ; | ALGORITHM FOR MATRICES MULTIPLICATION |
    ; +---------------------------------------+
    ; 
    ; The algorithm (C style) for computing the multiplication of two matrices
    ; is the following:
    ;
    ; for (int i = 0; i < N; i++) {
    ;   for (int j = 0; j < P; j++) {
    ;     int parsum = 0
    ;     for (int k = 0; k < M; k++) {
    ;         parsum = parsum + (A[i][k] * B[k][j])
    ;       }
    ;     C[i][j] = parsum
    ;    }
    ; }
    ; --------------------------------------------------------------------------- 
    XOR SI, SI                    ; Index i
    XOR DI, DI                    ; Index j
    XOR BP, BP                    ; index k
    
    MOV CX, N                     ; Looping over all the rows of the result matrix
    loopN:
        PUSH CX
        MOV  CX, P                ; Looping over all the colums of the result matrix
        loopP:
            PUSH CX
            MOV  CX, M            ; Looping over all the elements to sum (also, the  
                                  ; value M-CX is the index k)
            XOR  AX, AX           ; AX (parsum) = 0
            loopM:
                CALL atomic_mul   ; DX = A[i][k] * B[k][j]
                
                ; Checking for overflow after the MUL
                ; We need to check the OF flag before 
                ; performing an ADD operation (which
                ; may overwritten the flag itself)
                ; -----------------------------------
                JO overflow_happened
                
                ADD  AX, DX       ; parsum = parsum + DX
                
                ; Checking for overflow after the ADD
                ; -----------------------------------
                JO overflow_happened
                
                INC  BP           ; k++
                LOOP loopM
            continue:
            CALL compute_offset   ; Compute BX from indices i and j 
            MOV  matC[BX], AX     ; C[i][j] = parsum
            POP  CX
            XOR  BP, BP           ; k = 0
            INC  DI               ; j++
            LOOP loopP 
        POP  CX
        XOR  DI, DI               ; j = 0
        INC  SI                   ; i++
        LOOP loopN
    
    JMP show_result

overflow_happened:
    MOV AX, 7FFFh      ; Maximum signed integer representable in a word (32767)
    JMP continue       ; loopK can be ended prematurely

show_result:
    ; Print formatted message
    ; ----------------------------
    MOV AH, 09h
    LEA DX, matC_msg
    INT 21h
    
    ; Print a new line feed
    ; ----------------------------
    CALL print_new_line
    
    ; Print the matrix
    ; ----------------------------
    MOV CX, N*P
    XOR SI, SI
    print:
    
        ; Print a word data (first DH, then DL)
        ; -------------------------------------
        MOV DX, matC[SI]
        MOV AL, DH
        CALL print_hex
        MOV AL, DL
        CALL print_hex
        
        
        ; Print a space character
        ; -----------------------
        MOV AH, 02h
        MOV DL, ' '
        INT 21h
        
        ; Move to the next word (two bytes so double increase)
        ; ----------------------------------------------------
        INC SI
        INC SI
        
        ; Check if there is the need to format the matrix with a new line
        ; ---------------------------------------------------------------
        PUSH AX
        PUSH DX
        
        PUSH SI
        SHR  SI, 1          ; if (SI/2 % P == 0) then  // SI/2 since we work
        MOV  AX, SI         ;    GOTO new_line_feed    // with words (2 bytes)
        POP  SI
        
        MOV  DL, P  
        DIV  DL             ; AX / DL = AL + AH (AH is the remainder)
        CMP  AH, 0          ; compare remainder
        JE   new_line_feed
        
        POP DX  
        POP AX
         
        LOOP print

    JMP finished

new_line_feed:
    CALL print_new_line
    POP DX
    POP AX
    LOOP print
    JMP finished 

; +------------+
; | PROCEDURES |
; +------------+
; 
; Procedures implementation is defined below. In particular: 
;
;    - atomic_mul -> It performs the multiplication between A[i][k] and B[k][j] 
;                    by computing the two offsets for accessing the matrices
; 
;    - compute_offset -> It computes the offset given i and j, storing the result
;                        in BX 
; ------------------------------------------------------------------------------
 

atomic_mul:
    PUSH AX
    XOR  DX, DX
    XOR  AX, AX
    
    ; Accesses the element in matrix A (A[i][k]) and stores it in AX
    ; In particular:
    ;   index i corresponds to SI
    ;   index k corresponds to BP
    ; --------------------------------------------------------------
    PUSH AX
    PUSH DX
    MOV  DX, BP           ; index k
    MOV  AX, N            ; Number of rows (columns major ordering)
    MUL  DX               ; DX:AX = DX * AX i.e. k * N                 
    ADD  AX, SI           ; AX = (k * N) + SI 
    MOV  BX, AX
    POP  DX
    POP  AX                   
    MOV  AL, matA[BX]
    
    ; Accesses the element in matrix B (B[k][j]) and stores it in DX
    ; In particular:
    ;   index k corresponds to BP
    ;   index j corresponds to DI
    ; --------------------------------------------------------------
    PUSH AX
    PUSH DX
    MOV  DX, BP           ; index k
    MOV  AX, P            ; Number of columns (rows major ordering)
    MUL  DX               ; DX:AX = DX * AX i.e. k * P                 
    ADD  AX, DI           ; AX = (k * P) + SI  
    MOV  BX, AX
    POP  DX
    POP  AX 
    MOV  DL, matB[BX]
    
    ; Multiplication
    ; ---------------
    MUL  DL                ; AX = DL * AL
    MOV  DX, AX            ; Result is stored in DX
    POP  AX                ; beacuse AX needs to be restored              
    
    RET


compute_offset:
    PUSH AX
    MOV  AX, P            ; AX = P
    MUL  SI               ; DX:AX = SI * P
    ADD  AX, DI           ; AX = (SI * P) + DI
    MOV  BX, AX
    ADD  BX, BX
    POP  AX
    RET
    
    ; +------------------------------------------+
    ; | PROCEDURE TO PRINT A HEX VALUE TO OUTPUT |
    ; +------------------------------------------+
    ; 
    ; AL is the parameter storing the hexadecimal value that needs to be converted
    ; to a string in order to be printed.
    ; ------------------------------------------------------------------------------ 
    
print_hex:
    PUSH CX
    PUSH AX
    PUSH DX
    MOV CX, 2          ; Print 2 hex digits (= 8 bits)
    
    print_digit:
        ROL AL, 4      ; Move the currently left-most digit into the least significant 4 bits
        MOV DL, AL
        AND DL, 0Fh    ; Isolate the hex digit we want to print
        ADD DL, '0'    ; and convert it into a character..
        CMP DL, '9'    ; ...
        JBE ok         ; ...
        ADD DL, 7      ; ... (for 'A'..'F')
        
    ok:                
        PUSH AX        ; Save AX on the stack temporarily
        MOV  AH, 02h   ; INT 21H / AH=2: write character to stdout
        INT  21h
        POP  AX 
        LOOP print_digit
        
    POP DX
    POP AX
    POP CX
    RET
    
    ; +-------------------------------+
    ; | PROCEDURE TO PRINT A NEW LINE |
    ; +-------------------------------+
    ; 
    ; Print a new line to stdout
    ; --------------------------
print_new_line:
    PUSH AX
    PUSH DX 
    MOV  AH, 09h     
    LEA  DX, new_line_msg
    INT  21h
    POP  DX
    POP  AX
    RET 

finished:    
.EXIT
END