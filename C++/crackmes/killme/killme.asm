;[.org 666666h]

section .text
global dbg5554

; lololol
dbg5554:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    mov rax, [fs:0x30]
    add rax, 0
    mov rbx, rax
    xor rdx, rdx
    mov dl, [rax + 0x2]
    add rdx, 1
    sub rdx, 1
    xor rbx, rbx
    mov rsi, rdx
    shl rsi, 1
    shr rsi, 1
    movzx rax, dl
    xor rdi, rdi
    add rdi, 1
    sub rdi, 1
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    ret
