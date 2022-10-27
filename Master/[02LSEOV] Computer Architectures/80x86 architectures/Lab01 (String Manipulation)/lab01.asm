; +------------------+
; | GLOBAL CONSTANTS |
; +------------------+

DIM_MIN EQU 20    ; Minimum size of the input string
DIM_MAX EQU 50    ; Maximum size of the input string
N_LINES EQU  4    ; Number of lines to be read in input
OFFSET  EQU 65    ; Offset used to compute the index of the frequency array  


.MODEL small
.STACK      
.DATA       

    ; +-----------------+
    ; | DATA STRUCTURES |
    ; +-----------------+
    ;
    ; Input strings are stored in separated lines in order to increase readability 
    ; of the code.
    ; -----------------------------------------------------------------------------
    ;
    ; Frequencies of characters are stored in an array of 58 bytes, composed in the
    ; following way:
    ; 
    ; +---+---+-----+---+---+---+---+---+---+---+---+---+---+-----+---+---+
    ; | A | B | ... | Y | Z | [ | \ | ] | ^ | _ | ' | a | b | ... | y | z | 
    ; +---+---+-----+---+---+---+---+---+---+---+---+---+---+-----+---+---+
    ;   0   1                                                      56  57
    ; 
    ; NOTE THAT: ascii values of the characters range from 65 (A) to 122 (z) with
    ; 6 special characters in the middle ([\]^_') which are added in the array
    ; in order to simply the computation of the offset when computing the index.
    ; In this way, when a character is read from input, the correspondent frequency
    ; can be easily accessed in the array by doing AL - OFFSET (i.e. 65).
    ;
    ; NOTE THAT: bytes are sufficient to store the maximum possible frequency of a
    ; particular character, which is 50 for each line (DIM_MAX). 
    ; -----------------------------------------------------------------------------

  ;   Messages
  ; --------------------------------------------------------------------------------------                                
    error_msg       DB 0Dh, 0Ah, 'Insert at least 21 characters!', 0Dh, 0Ah, '$'
    new_line_msg    DB 0Dh, 0Ah, '$'
    separator_msg   DB '--------------------------------------------', '$'
    ciphered_txt    DB 'The chipered text is the following: ', '$'
    
    mfc_first_line  DB 'In the first line, the most frequent character is ', '$'
    mfc_second_line DB 'In the second line, the most frequent character is ', '$'
    mfc_third_line  DB 'In the third line, the most frequent character is ', '$'
    mfc_fourth_line DB 'In the fourth line, the most frequent character is ', '$'
    f_line          DB ' with frequency: ', '$'
    
    fc_first_line   DB 'Chars that appears at least MAX/2 times in the first line:', '$'
    fc_second_line  DB 'Chars that appears at least MAX/2 times in the second line:', '$'
    fc_third_line   DB 'Chars that appears at least MAX/2 times in the third line:', '$'
    fc_fourth_line  DB 'Chars that appears at least MAX/2 times in the fourth line:', '$'
  ; ---------------------------------------------------------------------------------------   
          
  ;   Lines
  ; ------------------------      
    first_line  DB 50 DUP(?) 
    second_line DB 50 DUP(?) 
    third_line  DB 50 DUP(?) 
    fourth_line DB 50 DUP(?)
  ; ------------------------
          
  ;   Frequency
  ; -------------------------------  
    first_freq_array   DB 58 DUP(0)
    second_freq_array  DB 58 DUP(0)
    third_freq_array   DB 58 DUP(0)
    fourth_freq_array  DB 58 DUP(0)
    max_value   DB ?
  ; -------------------------------  
                                  
  ;   Alphabeta
  ; --------------------------------------------------------------------
    alphabeta DB 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'
  ; --------------------------------------------------------------------

.CODE
.STARTUP   

    ; +----------------+
    ; | INITIALIZATION |
    ; +----------------+
    MOV AH, 01h      ; Setting the interrupt to "reading mode". Input is stored in AL. 
    XOR DI, DI       ; Counts the number of characters read from input
    MOV CX, 01h      ; Counts the number of lines read from input
        
    
    ; +---------------+
    ; | READING CHARS |
    ; +---------------+
    ;
    ; Input strings are read character by character in order
    ; to check if an ENTER character has been inserted and 
    ; therefore evaluate if the reading has to be terminated
    ; or must continue.
    ; Each line is read separately.
    ; 
    ; reading_char -> main routine, it reads a char from input
    ;                 and check the ascii code of the char
    ; ------------------------------------------------------
    
reading_char:
    
    INT 21h
    CMP AL, 0Dh                    ; Check if the user inserted ENTER (ascii code: CR, 13 or 0Dh)
    JZ  check_DIM_MIN              ; Jump if AL == ENTER
    
    CMP DI, DIM_MAX                ; Check if the upper bound (DIM_MAX) has been reached
    JNZ begin_storing_process      ; Store character if DIM_MAX has not been reached
    JMP next_line                  ; Jump to the next line if DIM_MAX has been reached
        
        
    ; +---------------------------+
    ; | CHECKING LOWEST DIMENSION |
    ; +---------------------------+
    ;
    ; If an ENTER character has been inserted, we need to check
    ; if the DIM_MIN has been reached. If the DIM_MIN has been
    ; reached, then increments the counter register CX, clear 
    ; the destination index DI and restart from the beginning.
    ; If DIM_MIN has NOT been reached, print the error messsage
    ; and continue reading.
    ;
    ; next_line -> manages the registers for a new line reading.
    ;              It checks if CX == 5 and jump to the analysis.
    ; 
    ; print_error_msg -> manages the displaying of error_msg
    ; ----------------------------------------------------------                
check_DIM_MIN:
    CMP  DI, DIM_MIN            ; if (DI >= DIM_MIN) {
    JAE  next_line              ;     GOTO next_line 
    CALL print_error_msg        ; } else {
    JMP  reading_char           ;     CALL error_msg
                                ;     GOTO reading_char
                                ; }
                                
                                
next_line:                      
    XOR DI, DI                  ; DI = 0 (we need to store back to the position 0 in the new line)
    INC CX                      ; CX++   (CX counts the number of lines)
    CMP CX, 5                   ; if (CX == 5) {
    JZ  analysis_first_line     ;     GOTO analysis      // We have finished storing chars
                                ; } else {
    CALL print_new_line         ;     GOTO reading_char  // We still have to read
    MOV AH, 01h
    JMP reading_char

print_error_msg:
    MOV AH, 09h              ; 09h interrupts prints a string 
    LEA DX, error_msg        ; DX stores the string pointer
    INT 21h                  ; display error_msg
    MOV AH, 01h
    JMP reading_char 
         




         
    ; +------------------------------------+
    ; | STORE A CHAR AND COMPUTE FREQUENCY |
    ; +------------------------------------+
    ;
    ; Once the reading character is checked and evaluated to
    ; a valid character, it can be stored in the corresponding
    ; position in the string variable.
    ; 
    ; begin_storing_process -> checks CX register to understant in which line the character 
    ;               needs to be stored; then it calls the correspondend
    ;               subroutine.
    ; 
    ; begin_storing_process_in_xxx -> stores the character in the string variable, 
    ;                      increment the destination index and increment 
    ;                      the correspondent frequency.
    ; ---------------------------------------------------------------------
begin_storing_process:     
    CMP CX, 1
    JE  begin_storing_process_in_first
    CMP CX, 2
    JE  begin_storing_process_in_second
    CMP CX, 3
    JE  begin_storing_process_in_third
    CMP CX, 4
    JE  begin_storing_process_in_fourth
                   
; +----------------------------+
; | First line storing process |
; +----------------------------+                   
begin_storing_process_in_first:
    
    ; Check if the character to be stored is an alphabetic character
    ; or if it is a special character. 
    ; In the first case, stores the character and the frequency, else
    ; just store the character
    ; Should be: 'A' <= AL <= 'Z' OR 'a' <= AL <= 'z'
    ; -----------------------------------------------------------------
    
    CMP AL, 41h           ; Compare with 'A'
    JL  storing_first     ; if AL < 'A' -> just store it
    CMP AL, 5Ah           ; Compare with 'Z'
    JLE frequency_first   ; && AL <= '>' -> found a letter
    CMP AL, 61h           ; Compare al with 'a'
    JL  storing_first     ; if AL < 'a' -> just store it
    CMP AL, 7Ah           ; Compare with 'z'
    JG  storing_first     ; if AL > 'z' -> just store it
    
frequency_first:                       
    XOR  BX, BX                   ; BX = 0
    MOV  BL, AL                   ; BL = AL (AL stores the hex code of the char)
    SUB  BL, OFFSET               ; Mapping every char to the specific position of the array 
                                  ; 'A' (HEX65)  --> 0
                                  ; 'B' (HEX66)  --> 1
                                  ; ...
                                  ; 'y' (HEX121) --> 56
                                  ; 'z' (HEX122) --> 57
    INC  first_freq_array [BX]

storing_first: 
    MOV first_line[DI], AL        ; AL stores the character
    INC DI
    
    JMP reading_char
  
  
; +-----------------------------+
; | Second line storing process |
; +-----------------------------+    
begin_storing_process_in_second:

    ; Check if the character to be stored is an alphabetic character
    ; or if it is a special character. 
    ; In the first case, stores the character and the frequency, else
    ; just store the character
    ; Should be: 'A' <= AL <= 'Z' OR 'a' <= AL <= 'z'
    ; -----------------------------------------------------------------
    
    CMP AL, 41h           ; Compare with 'A'
    JL  storing_second    ; if AL < 'A' -> just store it
    CMP AL, 5Ah           ; Compare with 'Z'
    JLE frequency_second  ; && AL <= '>' -> found a letter
    CMP AL, 61h           ; Compare al with 'a'
    JL  storing_second    ; if AL < 'a' -> just store it
    CMP AL, 7Ah           ; Compare with 'z'
    JG  storing_second    ; if AL > 'z' -> just store it
    
frequency_second:    
    XOR  BX, BX                   ; BX = 0
    MOV  BL, AL                   ; BL = AL (AL stores the hex code of the char)
    SUB  BL, OFFSET               ; Mapping every char to the specific position of the array 
                                  ; 'A' (HEX65)  --> 0
                                  ; 'B' (HEX66)  --> 1
                                  ; ...
                                  ; 'y' (HEX121) --> 56
                                  ; 'z' (HEX122) --> 57
    INC  second_freq_array [BX]

storing_second: 
    MOV second_line[DI], AL       ; AL stores the character
    INC DI
    
    JMP reading_char             
    

; +----------------------------+
; | Third line storing process |
; +----------------------------+
begin_storing_process_in_third:

    ; Check if the character to be stored is an alphabetic character
    ; or if it is a special character. 
    ; In the first case, stores the character and the frequency, else
    ; just store the character
    ; Should be: 'A' <= AL <= 'Z' OR 'a' <= AL <= 'z'
    ; -----------------------------------------------------------------
    
    CMP AL, 41h           ; Compare with 'A'
    JL  storing_third     ; if AL < 'A' -> just store it
    CMP AL, 5Ah           ; Compare with 'Z'
    JLE frequency_third   ; && AL <= '>' -> found a letter
    CMP AL, 61h           ; Compare al with 'a'
    JL  storing_third     ; if AL < 'a' -> just store it
    CMP AL, 7Ah           ; Compare with 'z'
    JG  storing_third     ; if AL > 'z' -> just store it
    
frequency_third:    
    XOR  BX, BX                   ; BX = 0
    MOV  BL, AL                   ; BL = AL (AL stores the hex code of the char)
    SUB  BL, OFFSET               ; Mapping every char to the specific position of the array 
                                  ; 'A' (HEX65)  --> 0
                                  ; 'B' (HEX66)  --> 1
                                  ; ...
                                  ; 'y' (HEX121) --> 56
                                  ; 'z' (HEX122) --> 57
    INC  third_freq_array [BX]

storing_third: 
    MOV third_line[DI], AL        ; AL stores the character
    INC DI
    
    JMP reading_char
    

; +-----------------------------+
; | Fourth line storing process |
; +-----------------------------+    
begin_storing_process_in_fourth:

    ; Check if the character to be stored is an alphabetic character
    ; or if it is a special character. 
    ; In the first case, stores the character and the frequency, else
    ; just store the character
    ; Should be: 'A' <= AL <= 'Z' OR 'a' <= AL <= 'z'
    ; -----------------------------------------------------------------
    
    CMP AL, 41h           ; Compare with 'A'
    JL  storing_fourth    ; if AL < 'A' -> just store it
    CMP AL, 5Ah           ; Compare with 'Z'
    JLE frequency_fourth  ; && AL <= '>' -> found a letter
    CMP AL, 61h           ; Compare al with 'a'
    JL  storing_fourth    ; if AL < 'a' -> just store it
    CMP AL, 7Ah           ; Compare with 'z'
    JG  storing_fourth    ; if AL > 'z' -> just store it
    
frequency_fourth:    
    XOR  BX, BX                   ; BX = 0
    MOV  BL, AL                   ; BL = AL (AL stores the hex code of the char)
    SUB  BL, OFFSET               ; Mapping every char to the specific position of the array 
                                  ; 'A' (HEX65)  --> 0
                                  ; 'B' (HEX66)  --> 1
                                  ; ...
                                  ; 'y' (HEX121) --> 56
                                  ; 'z' (HEX122) --> 57
    INC  fourth_freq_array [BX]

storing_fourth: 
    MOV fourth_line[DI], AL       ; AL stores the character
    INC DI
    
    JMP reading_char
    
    
 
 
 
 
 
 
 
    
    
    

    ; +----------+
    ; | ANALYSIS |
    ; +----------+
    ; 
    ; Number of occurrences:fFor each line, the program has to count 
    ; how many times a certain character appears. Consider only 
    ; letters, a...z, A...Z, discerning upper and lower case. 
    ; 
    ; 1) For each line, output the most frequent character (appearing MAX times). 
    ; 2) For each line, print the list of characters appearing at least MAX/2 times.
    ; 3) After each character printed, print also the number of occurrences.
    ;    NOTE THAT: the number of occurrences is stored as an integer byte value, 
    ;               therefore we need to convert it to a string representation
    ; ------------------------------------------------------------------------------ 


; -------------------               
; First line analysis
; -------------------               
analysis_first_line:
    ; Print new line feed
    CALL print_new_line
    CALL print_new_line
    
    ; Searching the most frequent character 
    ; -------------------------------------
    XOR SI, SI      ; Source index
    XOR DX, DX      ; DH stores the most frequent character, DL stores the frequency
    MOV CX, 3Ah     ; Storing 58 (0x3A) decimal

    search_max_first:
        CMP  first_freq_array[SI], DL         ; for (SI = 0; SI < 58; SI++) {
        JA   change_max_first                 ;      if (freq[SI] > DL) {         // DL = max_value
        INC  SI                               ;          GOTO change_max
        LOOP search_max_first                 ;      }
                                              ; }
                                              
        
    continue_first:    
    ; Printing information about the most frequent character
    ; -------------------------------------------------------  
    ; Print 'In ... line, the most frequent character is: '
    PUSH DX
    MOV  AH, 09h          
    LEA  DX, mfc_first_line
    INT  21h
    POP  DX
    
    ; Print the most frequent character (ascii code is in DH)
    MOV AH, 02h          
    MOV DL, DH
    INT 21h
    
    ; Print 'with frequency: '
    MOV AH, 09h          
    LEA DX, f_line
    INT 21h
    
    ; Print the frequency         
    MOV AL, max_value
    CALL print_hex
    
    ; Print new line feed
    CALL print_new_line
    
    
    ; Searching the characters that appear at least MAX / 2 times
    ; -----------------------------------------------------------
    ; Print message
    MOV  AH, 09h          
    LEA  DX, fc_first_line
    INT  21h
    
    XOR SI, SI
    XOR DX, DX 
    MOV DL, max_value    ; DL is now storing max_value
    SAR DL, 1            ; DL = max_value / 2
    MOV CX, 3Ah          ; Looping through all the characters
    
    search_max2_first:
        CMP  first_freq_array[SI], DL       ; for (SI = 0; SI < 58; SI++) {
        JA   print_char_first               ;      if (freq[SI] > DL) {      // DL = max/2
        INC  SI                             ;          GOTO print_char
        LOOP search_max2_first              ;       }
                                            ; }
    JMP analysis_second_line

change_max_first:
    MOV  DL, first_freq_array[SI]    ; Storing frequency
    MOV  max_value, DL
    MOV  BX, SI
    ADD  BX, OFFSET
    MOV  DH, BL                      ; Storing character
    INC  SI
    LOOP search_max_first
    JMP  continue_first

print_char_first:
    PUSH DX                          ; Preserving DL

    ; Printing information about the characters that appear at least MAX / 2 times
    ; ----------------------------------------------------------------------------
    ; Print new line feed
    CALL print_new_line
    
    ; Print the char
    MOV AH, 02h
    MOV DX, SI
    ADD DX, OFFSET         
    INT 21h
    
    ; Print 'with frequency: '
    MOV  AH, 09h          
    LEA  DX, f_line
    INT  21h
    
    ; Print the frequency
    PUSH CX         
    MOV  AL, first_freq_array[SI]
    CALL print_hex
    POP  CX
    
    ; Looping
    POP  DX                            ; Restoring DL
    INC  SI
    LOOP search_max2_first 
    

                 
; --------------------               
; Second line analysis
; --------------------               
analysis_second_line:
    ; Print new line feed
    CALL print_new_line
    CALL print_new_line
    
    ; Searching the most frequent character 
    ; -------------------------------------
    XOR SI, SI      ; Source index
    XOR DX, DX      ; DH stores the most frequent character, DL stores the frequency
    MOV CX, 3Ah     ; Storing 58 decimal

    search_max_second:
        CMP  second_freq_array[SI], DL
        JAE  change_max_second
        INC  SI
        LOOP search_max_second
    
    continue_second:    
    ; Printing information about the most frequent character
    ; -------------------------------------------------------  
    ; Print 'In ... line, the most frequent character is: '
    PUSH DX
    MOV  AH, 09h          
    LEA  DX, mfc_second_line
    INT  21h
    POP  DX
    
    ; Print the most frequent character (ascii code is in DH)
    MOV AH, 02h          
    MOV DL, DH
    INT 21h
    
    ; Print 'with frequency: '
    MOV AH, 09h          
    LEA DX, f_line
    INT 21h
    
    ; Print the frequency         
    MOV AL, max_value
    CALL print_hex
    
    ; Print new line feed
    CALL print_new_line
    
    ; Searching the characters that appear at least MAX / 2 times
    ; -----------------------------------------------------------
    ; Print message
    MOV  AH, 09h          
    LEA  DX, fc_second_line
    INT  21h
    
    XOR SI, SI
    XOR DX, DX 
    MOV DL, max_value    ; DL is now storing max_value
    SAR DL, 1            ; DL = max_value / 2
    MOV CX, 3Ah          ; Looping through all the characters
    
    search_max2_second:
        CMP  second_freq_array[SI], DL
        JA   print_char_second
        INC  SI
        LOOP search_max2_second 
    
    JMP analysis_third_line

change_max_second:
    MOV  DL, second_freq_array[SI]    ; Storing frequency
    MOV  max_value, DL
    MOV  BX, SI
    ADD  BX, OFFSET
    MOV  DH, BL                       ; Storing character
    INC  SI
    LOOP search_max_second
    JMP  continue_second

print_char_second:
    PUSH DX                           ; Preserving DL

    ; Printing information about the characters that appear at least MAX / 2 times
    ; ----------------------------------------------------------------------------
    ; Print new line feed
    CALL print_new_line
    
    ; Print the char
    MOV AH, 02h
    MOV DX, SI
    ADD DX, OFFSET         
    INT 21h
    
    ; Print 'with frequency: '
    MOV  AH, 09h          
    LEA  DX, f_line
    INT  21h
    
    ; Print the frequency
    PUSH CX         
    MOV  AL, second_freq_array[SI]
    CALL print_hex
    POP  CX
    
    ; Looping
    POP  DX                           ; Restoring DL
    INC  SI
    LOOP search_max2_second 
    

; -------------------               
; Third line analysis
; -------------------               
analysis_third_line:
    ; Print new line feed
    CALL print_new_line
    CALL print_new_line
    
    ; Searching the most frequent character 
    ; -------------------------------------
    XOR SI, SI      ; Source index
    XOR DX, DX      ; DH stores the most frequent character, DL stores the frequency
    MOV CX, 3Ah     ; Storing 58 decimal

    search_max_third:
        CMP  third_freq_array[SI], DL
        JAE  change_max_third
        INC  SI
        LOOP search_max_third
    
    continue_third:    
    ; Printing information about the most frequent character
    ; -------------------------------------------------------  
    ; Print 'In ... line, the most frequent character is: '
    PUSH DX
    MOV  AH, 09h          
    LEA  DX, mfc_third_line
    INT  21h
    POP  DX
    
    ; Print the most frequent character (ascii code is in DH)
    MOV AH, 02h          
    MOV DL, DH
    INT 21h
    
    ; Print 'with frequency: '
    MOV AH, 09h          
    LEA DX, f_line
    INT 21h
    
    ; Print the frequency         
    MOV AL, max_value
    CALL print_hex
    
    ; Print new line feed
    CALL print_new_line
    
    ; Searching the characters that appear at least MAX / 2 times
    ; -----------------------------------------------------------
    ; Print message
    MOV  AH, 09h          
    LEA  DX, fc_third_line
    INT  21h
    
    XOR SI, SI
    XOR DX, DX 
    MOV DL, max_value    ; DL is now storing max_value
    SAR DL, 1            ; DL = max_value / 2
    MOV CX, 3Ah          ; Looping through all the characters
    
    search_max2_third:
        CMP  third_freq_array[SI], DL
        JA   print_char_third
        INC  SI
        LOOP search_max2_third 
    
    JMP analysis_fourth_line

change_max_third:
    MOV  DL, third_freq_array[SI]    ; Storing frequency
    MOV  max_value, DL
    MOV  BX, SI
    ADD  BX, OFFSET
    MOV  DH, BL                      ; Storing character
    INC  SI
    LOOP search_max_third
    JMP  continue_third
    
print_char_third:
    PUSH DX                          ; Preserving DL

    ; Printing information about the characters that appear at least MAX / 2 times
    ; ----------------------------------------------------------------------------
    ; Print new line feed
    CALL print_new_line
    
    ; Print the char
    MOV AH, 02h
    MOV DX, SI
    ADD DX, OFFSET         
    INT 21h
    
    ; Print 'with frequency: '
    MOV  AH, 09h          
    LEA  DX, f_line
    INT  21h
    
    ; Print the frequency
    PUSH CX         
    MOV  AL, third_freq_array[SI]
    CALL print_hex
    POP  CX
    
    ; Looping
    POP  DX                             ; Restoring DL
    INC  SI
    LOOP search_max2_third
    
    
; --------------------               
; Fourth line analysis
; --------------------               
analysis_fourth_line:
    ; Print new line feed
    CALL print_new_line
    CALL print_new_line
    
    ; Searching the most frequent character 
    ; -------------------------------------
    XOR SI, SI      ; Source index
    XOR DX, DX      ; DH stores the most frequent character, DL stores the frequency
    MOV CX, 3Ah     ; Storing 58 decimal

    search_max_fourth:
        CMP  fourth_freq_array[SI], DL
        JAE  change_max_fourth
        INC  SI
        LOOP search_max_fourth
    
    continue_fourth:    
    ; Printing information about the most frequent character
    ; -------------------------------------------------------  
    ; Print 'In ... line, the most frequent character is: '
    PUSH DX
    MOV  AH, 09h          
    LEA  DX, mfc_fourth_line
    INT  21h
    POP  DX
    
    ; Print the most frequent character (ascii code is in DH)
    MOV AH, 02h          
    MOV DL, DH
    INT 21h
    
    ; Print 'with frequency: '
    MOV AH, 09h          
    LEA DX, f_line
    INT 21h
    
    ; Print the frequency         
    MOV AL, max_value
    CALL print_hex
    
    ; Print new line feed
    CALL print_new_line
    
    ; Searching the characters that appear at least MAX / 2 times
    ; -----------------------------------------------------------
    ; Print message
    MOV  AH, 09h          
    LEA  DX, fc_fourth_line
    INT  21h
    
    XOR SI, SI
    XOR DX, DX 
    MOV DL, max_value    ; DL is now storing max_value
    SAR DL, 1            ; DL = max_value / 2
    MOV CX, 3Ah          ; Looping through all the characters
    
    search_max2_fourth:
        CMP  fourth_freq_array[SI], DL
        JA   print_char_fourth
        INC  SI
        LOOP search_max2_fourth 
    
    JMP begin_cipher_process

change_max_fourth:
    MOV  DL, fourth_freq_array[SI]    ; Storing frequency
    MOV  max_value, DL
    MOV  BX, SI
    ADD  BX, OFFSET
    MOV  DH, BL                       ; Storing character
    INC  SI
    LOOP search_max_fourth
    JMP  continue_fourth

print_char_fourth:
    PUSH DX                           ; Preserving DL

    ; Printing information about the characters that appear at least MAX / 2 times
    ; ----------------------------------------------------------------------------
    ; Print new line feed
    CALL print_new_line
    
    ; Print the char
    MOV AH, 02h
    MOV DX, SI
    ADD DX, OFFSET         
    INT 21h
    
    ; Print 'with frequency: '
    MOV  AH, 09h          
    LEA  DX, f_line
    INT  21h
    
    ; Print the frequency
    PUSH CX         
    MOV  AL, fourth_freq_array[SI]
    CALL print_hex
    POP  CX
    
    ; Looping
    POP  DX                              ; Restoring DL
    INC  SI
    LOOP search_max2_fourth
    
    JMP begin_cipher_process
    


    ; +------------------------------------------+
    ; | PROCEDURE TO PRINT A HEX VALUE TO OUTPUT |
    ; +------------------------------------------+
    ; 
    ; AL is the parameter storing the hexadecimal value that needs to be converted
    ; to a string in order to be printed.
    ; ------------------------------------------------------------------------------ 
    
print_hex:
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
    
    

    
    
    ; +----------------------+
    ; | BEGIN CHIPER PROCESS |
    ; +----------------------+
    ; 
    ; Print the text using Caesar cipher, only applied to a...z, A...Z characters.
    ; Given parameter k (k = 1 for the first row, 2 for the second, 3 for
    ; the third, 4 for the fourth.), the Caesar cipher transforms the letter in a+k, 
    ; considering the following pattern: a...zA...Za...zA...Z etc. Non-alphabetic 
    ; characters stay the same. Eample with k = 3: piZza -> slcCd
    ;
    ; cipher_line -> reads char by char (understanding from which line), checks if
    ;                the char is letter or special character and in case compute
    ;                the enciphered char. 
    ; ------------------------------------------------------------------------------
    
begin_cipher_process:
    
    ; Formatting the output
    ; ---------------------
    CALL print_new_line
    CALL print_new_line
    MOV  AH, 09h          
    LEA  DX, separator_msg
    INT  21h
    CALL print_new_line             
    LEA  DX, ciphered_txt
    INT  21h
    CALL print_new_line
     
     ; Set k parameter to 0. Increments k every new line, starting from 1
     ; --------------------------------------------------------------------
    XOR  DH, DH                                                            
    
    
    
; +------------------------+
; | Encryption of the line |
; +------------------------+   
cipher_line:

    ; Initilization
    ; --------------
    INC  DH             ; starts from 1
    CALL print_new_line
    XOR  SI, SI         ; Source index
    MOV  CX, DIM_MAX
    
    ; Loop for reading char by char the line
    ; DL stores the character extracted
    ; DH stores the k parameter to be applied
    ; -----------------------------------------
    extract_char:
        CMP DH, 01h
        JE  extract_from_first
        CMP DH, 02h
        JE  extract_from_second
        CMP DH, 03h
        JE  extract_from_third
        CMP DH, 04h
        JE  extract_from_fourth
        CMP DH, 05h
        JE finished
        
extract_from_first:
    MOV  DL, first_line[SI]
    JMP  validate_char
extract_from_second:
    MOV  DL, second_line[SI]
    JMP  validate_char
extract_from_third:
    MOV  DL, third_line[SI]
    JMP  validate_char
extract_from_fourth:
    MOV  DL, fourth_line[SI]
    JMP  validate_char

; Check if the character is a letter or a special character in order to
; encipher only the alphabetic character
; ----------------------------------------------------------------------    
validate_char:
    ; Should be: 'A' <= DL <= 'Z' OR 'a' <= DL <= 'z'
    
    CMP DL, 41h                 ; Compare with 'A'
    JL  print_unciphered_char   ; if DL < 'A' -> move to the next char
    CMP DL, 5Ah                 ; Compare with 'Z'
    JLE compute_index_position  ; && DL <= '>' -> found a letter
    CMP DL, 61h                 ; Compare al with 'a'
    JL  print_unciphered_char   ; if DL < 'a' -> move to the next char
    CMP DL, 7Ah                 ; Compare with 'z'
    JG  print_unciphered_char   ; if DL > 'z' -> just store it
    JMP compute_index_position    
 
; Move to the next char in the line
; ------------------------------       
next_char:
    INC  SI
    LOOP extract_char
    JMP  cipher_line                   ; Start from a new line in case you reached the end

; for (DI = 0; DI < 52; DI++) {
;      if (AL == alphabeta[DI]) {   // AL stores the character to encipher
;          GOTO compute_offset
;      }
; }
compute_index_position:                                                         
    XOR  DI, DI
    PUSH CX
    MOV  CX, 34h                ; Looping through all the 52 letters (A...Za...z)
    
    find_index:
        MOV  AL, alphabeta[DI]  ; AL stores one letter of the alphabet 'A...Za...z'
        CMP  AL, DL             ; Check if the letter (AL) is the same as the current one
        JE   compute_offset
        INC  DI
        LOOP find_index

; AL = 51                         // position of 'z'
; AL = AL - DI                    // 'z' - index_of_current_char
; if (AL >= k) {
;    GOTO add_offset
; } else {
;    index_of_current_char = AL - k    // Subtract the number of position still available
; }                                    // at the end of the alphabet   
compute_offset:
    XOR AX, AX
    MOV AL, 33h                 ; AL = 51
    SUB AX, DI                  ; AL = 51 - DI
    CMP AL, DH                  ; if (AL >= DH) -> Just add the offset DH
    JAE just_add_offset
    PUSH DX                     ; Preserving DX
    SUB DH, AL                  ; else -> DI = AL - DH
    MOV DL, DH
    XOR DH, DH
    MOV DI, DX
    DEC DI                      ; Just because the alphabet array starts from zero
    POP DX
    CALL print_ciphered_char
    POP CX
    JMP next_char

; index_of_current_char = index_of_current_char + k    
just_add_offset:
    PUSH DX
    MOV  DH, DL
    XOR  DH, DH
    ADD  DI, DX
    POP  DX
    CALL print_ciphered_char
    
    ; Move to the next char
    POP CX
    JMP next_char

print_unciphered_char:
    MOV  AH, 02h
    INT  21h
    JMP next_char
         
print_ciphered_char:
    PUSH DX
    MOV AH, 02h
    MOV DL, alphabeta[DI]
    INT 21h
    POP DX     
    RET 
     
finished:     
     
.EXIT
END