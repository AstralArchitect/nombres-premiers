section .note.GNU-stack

section .text
    global search_square_root
    global search_square
    extern sqrt

; RDI : pointer to the list (previously allocated)
; RSI : number of primes to find
search_square_root:
.init_firsts:
    mov dword [rdi], 2          ; first prime number is 2
    mov dword [rdi + 4], 3      ; second prime number is 3
    mov dword [rdi + 8], 5      ; third prime number is 5

.init_loop:
    mov r9d, 5          ; actual number to test
    mov r13d, 3         ; number of primes found
    mov r8d, 4
    call _find_loop     ; start the loop
    ret                 ; return

_find_loop:
    ; --- Convert Integer to Double ---
    ; Convert the 64-bit signed integer in R9 to a double-precision float in xmm0
    ; sqrt() requires a double in xmm0 according to the ABI
    cvtsi2sd xmm0, r9   ; xmm0 = (double)R9

    ; --- Call sqrt ---
    call sqrt            ; Call the sqrt function. Result (double) is in xmm0.

    ; --- Convert Result Double to Integer ---
    ; Convert the double in xmm0 back to a 32-bit *signed* integer, truncating.
    ; Store the result in EBX (a 32-bit register).
    ; cvttsd2si truncates (cuts off the fractional part).
    ; If the mathematical result is positive and fits within 32 bits (0 to 2^32-1),
    ; the resulting bit pattern in EBX will represent the correct unsigned value.
    cvttsd2si ebx, xmm0  ; ebx = (int32_t)xmm0 (truncated)

    mov r12d, 2          ; counter for the loop

; check R9
.is_prime:
    mov eax, r9d                ; store the dividend into EAX
    xor edx, edx                ; zero EDX, wich will store the remainder of the division
    mov r11d, dword [rdi + r12 * 4]

    div r11d                    ; divide : R9 / R11 -> remainder = EDX; quotien = EAX
    test edx, edx               ; test if the remainder is null
    jz .end                     ; if so, the number is not a prime, exit the loop
    
    cmp r11d, ebx               ; check if we've reached the square root of the number to test
    jge .add_prime              ; if so, exit the loop

    inc r12d                    ; increase the loop counter (R12)
    jmp .is_prime               ; go to the next iteration

.add_prime:
    mov dword [rdi + r13 * 4], r9d  ; store the number into the list
    inc r13d                        ; increase 'number of primes found'

.end:
    ; Alternate step between 2 and 4
    xor r8d, 6          ; R8 = (R8 == 2) ? 4 : 2

    add r9d, r8d
    cmp r13d, esi       ; check if we have reached the number of primes asked
    jl _find_loop       ; if no, continue the loop
    ret                 ; if yes, stop the loop


; RDI : pointer to the list (previously allocated)
; RSI : number of primes to find
search_square:
.init_firsts:
    mov dword [rdi], 2          ; first prime number is 2
    mov dword [rdi + 4], 3      ; second prime number is 3
    mov dword [rdi + 8], 5      ; third prime number is 5

.init_loop:
    mov r9d, 5          ; actual number to test
    mov r13d, 3         ; number of primes found
    mov r8d, 4
    call _find_loop_sq     ; start the loop
    ret                 ; return

_find_loop_sq:
    mov r12d, 2          ; counter for the loop

; check R9
.is_prime:
    mov eax, r9d                ; store the dividend into EAX
    xor edx, edx                ; zero EDX, wich will store the remainder of the division
    mov r11d, dword [rdi + r12 * 4]

    div r11d                    ; divide : R9 / R11 -> remainder = EDX; quotien = EAX
    test edx, edx               ; test if the remainder is null
    jz .end                     ; if so, the number is not a prime, exit the loop
    
    mov eax, r11d               ; set EAX to R11
    mul r11d                    ; to perform R11²
    cmp r11d, r9d                ; check if we've reached the square root of the number to test
    jge .add_prime              ; if so, exit the loop

    inc r12d                    ; increase the loop counter (R12)
    jmp .is_prime               ; go to the next iteration

.add_prime:
    mov dword [rdi + r13 * 4], r9d  ; store the number into the list
    inc r13d                        ; increase 'number of primes found'

.end:
    ; Alternate step between 2 and 4
    xor r8d, 6          ; R8 = (R8 == 2) ? 4 : 2

    add r9d, r8d
    cmp r13d, esi       ; check if we have reached the number of primes asked
    jl _find_loop       ; if no, continue the loop
    ret                 ; if yes, stop the loop