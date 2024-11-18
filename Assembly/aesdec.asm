section .data
    ; const uint8_t SBOX[256]
    sbox:   db 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5
            db 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76
            db 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0
            db 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0
            db 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc
            db 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15
            db 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a
            db 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75
            db 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0
            db 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84
            db 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b
            db 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf
            db 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85
            db 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8
            db 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5
            db 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2
            db 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17
            db 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73
            db 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88
            db 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb
            db 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c
            db 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79
            db 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9
            db 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08
            db 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6
            db 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a
            db 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e
            db 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e
            db 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94
            db 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf
            db 0x8c, 0xa1, 0x89, 0x0d, 0x9c, 0x3f, 0x81, 0xf0
            db 0x7c, 0x9a, 0x7d, 0x7b, 0x6b, 0x6f, 0x30, 0x01

section .text
global aesdec

aesdec:
    ; rdi - ciphertext (const uint8_t*)
    ; rsi - key (const uint8_t*)
    ; rdx - output (uint8_t*)

    mov rax, rsi
    mov rbx, rdi
    mov rcx, rdx

    call AddRoundKey

    mov rcx, 9
round_loop:
    call InvShiftRows
    call InvSubBytes
    call AddRoundKey
    call InvMixColumns
    dec rcx
    jnz round_loop

    ; Final round (without InvMixColumns)
    call InvShiftRows
    call InvSubBytes
    call AddRoundKey

    ret

AddRoundKey:
    mov rdx, 16                 ; AES block size
add_loop:
    mov al, [rbx]               ; Load byte from state
    xor al, [rax]               ; XOR with key byte
    mov [rbx], al               ; Store back to state
    inc rbx                     ; Move to next byte
    dec rdx
    jnz add_loop
    ret

InvSubBytes:
    mov rdx, 16                 ; AES block size
sub_bytes_loop:
    mov al, [rbx]               ; Load byte from state
    mov al, [sbox + rax]        ; Substitute using S-box
    mov [rbx], al               ; Store back to state
    inc rbx                     ; Move to next byte
    dec rdx
    jnz sub_bytes_loop
    ret

InvShiftRows:
    ; The state is represented as follows:
    ; 0  1  2  3
    ; 4  5  6  7
    ; 8  9  A  B
    ; C  D  E  F

    ; Row 0: No shift
    ; Row 1: Shift left by 1
    ; Row 2: Shift left by 2
    ; Row 3: Shift left by 3

    ; Shift Row 1 (bytes 4, 5, 6, 7)
    mov al, [rbx + 4]          ; Load state[1][0]
    mov bl, [rbx + 5]          ; Load state[1][1]
    mov cl, [rbx + 6]          ; Load state[1][2]
    mov dl, [rbx + 7]          ; Load state[1][3]

    mov [rbx + 4], bl          ; state[1][0] = state[1][1]
    mov [rbx + 5], cl          ; state[1][1] = state[1][2]
    mov [rbx + 6], dl          ; state[1][2] = state[1][3]
    mov [rbx + 7], al          ; state[1][3] = state[1][0]

    ; Shift Row 2 (bytes 8, 9, A, B)
    mov al, [rbx + 8]
    mov bl, [rbx + 9]
    mov cl, [rbx + 10]
    mov dl, [rbx + 11]

    mov [rbx + 8], cl          ; state[2][0] = state[2][2]
    mov [rbx + 9], dl          ; state[2][1] = state[2][3]
    mov [rbx + 10], al         ; state[2][2] = state[2][0]
    mov [rbx + 11], bl         ; state[2][3] = state[2][1]

    ; Shift Row 3 (bytes C, D, E, F)
    mov al, [rbx + 12]
    mov bl, [rbx + 13]
    mov cl, [rbx + 14]
    mov dl, [rbx + 15]

    mov [rbx + 12], dl         ; state[3][0] = state[3][3]
    mov [rbx + 13], al         ; state[3][1] = state[3][0]
    mov [rbx + 14], bl         ; state[3][2] = state[3][1]
    mov [rbx + 15], cl         ; state[3][3] = state[3][2]

    ret

InvMixColumns:
    xor rax, rax
    xor rdx, rdx

    ; Process each column
    mov rdi, rbx                 ; rdi points to the first byte of the state

    ; Column 0 (bytes 0, 4, 8, C)
    mov rsi, rdi                 ; Save the starting address of the column
    call InvMixColumn            ; Process the first column
    add rdi, 4                   ; Move to the next column (next 4 bytes)
    
    ; Column 1 (bytes 1, 5, 9, D)
    call InvMixColumn            ; Process the second column
    add rdi, 4                   ; Move to the next column (next 4 bytes)

    ; Column 2 (bytes 2, 6, A, E)
    call InvMixColumn            ; Process the third column
    add rdi, 4                   ; Move to the next column (next 4 bytes)

    ; Column 3 (bytes 3, 7, B, F)
    call InvMixColumn            ; Process the fourth column

    ret

InvMixColumn:
    ; Inputs: rdi points to the first byte of the column
    ; Output: results are stored back in the same location

    mov al, [rdi]
    mov bl, [rdi + 4]
    mov cl, [rdi + 8]
    mov dl, [rdi + 12]

    ; Perform the inverse mix columns operation
    ; using the inverse mix column matrix:
    ; | 0x0E 0x0B 0x0D 0x09 |
    ; | 0x09 0x0E 0x0B 0x0D |
    ; | 0x0D 0x09 0x0E 0x0B |
    ; | 0x0B 0x0D 0x09 0x0E |

    ; Temporary variables for results
    xor rax, rax
    xor rdx, rdx

    ; Calculate the new values for each byte
    ; result = 0x0E * a + 0x0B * b + 0x0D * c + 0x09 * d

    ; First byte calculation
    mov rdi, 0x0E
    mov rax, [rdi]              ; Load byte a
    call GFMul                  ; Multiply by 0x0E
    mov r8, rax                 ; Store result in r8

    mov rdi, 0x0B
    mov rax, [rdi + 4]          ; Load byte b
    call GFMul                  ; Multiply by 0x0B
    xor r8, rax                 ; XOR with previous result

    mov rdi, 0x0D
    mov rax, [rdi + 8]          ; Load byte c
    call GFMul                  ; Multiply by 0x0D
    xor r8, rax                 ; XOR with previous result

    mov rdi, 0x09
    mov rax, [rdi + 12]         ; Load byte d
    call GFMul                  ; Multiply by 0x09
    xor r8, rax                 ; XOR with previous result
    mov [rdi], r8               ; Store result back in the first byte

    ; Repeat for the second byte
    xor rax, rax
    mov rdi, 0x0E
    mov rax, [rdi + 1]          ; Load byte a
    call GFMul                  ; Multiply by 0x0E
    mov r8, rax                 ; Store result in r8

    mov rdi, 0x0B
    mov rax, [rdi + 5]          ; Load byte b
    call GFMul                  ; Multiply by 0x0B
    xor r8, rax                 ; XOR with previous result

    mov rdi, 0x0D
    mov rax, [rdi + 9]          ; Load byte c
    call GFMul                  ; Multiply by 0x0D
    xor r8, rax                 ; XOR with previous result

    mov rdi, 0x09
    mov rax, [rdi + 13]         ; Load byte d
    call GFMul                  ; Multiply by 0x09
    xor r8, rax                 ; XOR with previous result
    mov [rdi + 4], r8           ; Store result back in the second byte

    ; Repeat for the third byte
    xor rax, rax
    mov rdi, 0x0E
    mov rax, [rdi + 2]          ; Load byte a
    call GFMul                  ; Multiply by 0x0E
    mov r8, rax                 ; Store result in r8

    mov rdi, 0x0B
    mov rax, [rdi + 6]          ; Load byte b
    call GFMul                  ; Multiply by 0x0B
    xor r8, rax                 ; XOR with previous result

    mov rdi, 0x0D
    mov rax, [rdi + 10]         ; Load byte c
    call GFMul                  ; Multiply by 0x0D
    xor r8, rax                 ; XOR with previous result

    mov rdi, 0x09
    mov rax, [rdi + 14]         ; Load byte d
    call GFMul                  ; Multiply by 0x09
    xor r8, rax                 ; XOR with previous result
    mov [rdi + 8], r8           ; Store result back in the third byte

    ; Repeat for the fourth byte
    xor rax, rax
    mov rdi, 0x0E
    mov rax, [rdi + 3]          ; Load byte a
    call GFMul                  ; Multiply by 0x0E
    mov r8, rax                 ; Store result in r8

    mov rdi, 0x0B
    mov rax, [rdi + 7]          ; Load byte b
    call GFMul                  ; Multiply by 0x0B
    xor r8, rax                 ; XOR with previous result

    mov rdi, 0x0D
    mov rax, [rdi + 11]         ; Load byte c
    call GFMul                  ; Multiply by 0x0D
    xor r8, rax                 ; XOR with previous result

    mov rdi, 0x09
    mov rax, [rdi + 15]         ; Load byte d
    call GFMul                  ; Multiply by 0x09
    xor r8, rax                 ; XOR with previous result
    mov [rdi + 12], r8          ; Store result back in the fourth byte

    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; GFMul function multiplies a byte by a constant in GF(2^8)
; Inputs: 
;   rax = byte to multiply
;   rdi = constant to multiply by
; Output: 
;   rax = result of multiplication

GFMul:
    xor rbx, rbx                ; Clear rbx for the result
    mov rcx, 8                  ; Set loop counter for 8 bits

GFMul_Loop:
    test rax, 1                 ; Check if the least significant bit of the byte is set
    jz GFMul_NoAdd              ; If not set, skip addition

    xor rbx, rdi                ; XOR with the constant

GFMul_NoAdd:
    shr rax, 1                  ; Shift the byte right
    mov rdx, rdi                ; Copy constant to rdx
    and rdx, 0x80               ; Isolate the highest bit
    jz GFMul_Next               ; If not set, just shift the constant

    xor rdi, 0x1b              ; XOR with the irreducible polynomial

GFMul_Next:
    shl rdi, 1                  ; Shift the constant left
    test rdx, rdx               ; Check if the highest bit was set
    jnz GFMul_Overflow          ; If it was, we need to reduce

    loop GFMul_Loop             ; Decrement rcx and loop if not zero
    mov rax, rbx                ; Move the result into rax
    ret

GFMul_Overflow:
    shr rdi, 1                  ; Shift right to reduce
    xor rdi, 0x1b               ; XOR with the irreducible polynomial
    mov rax, rbx                ; Move the result into rax
    ret
