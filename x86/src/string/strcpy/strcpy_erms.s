
section .text
global __strcpy_erms:hidden

extern ft_strlen:function

; char	*__strcpy_erms(char *dest, char *src)
; rdi -> dest
; rsi -> src
__strcpy_erms:
    mov     r8, rdi
    mov     r9, rsi

    sub     rsp, 8
    mov     rdi, r9
    call    ft_strlen
    add     rsp, 8

    inc     rax 
    mov     rcx, rax
    mov     rdi, r8
    mov     rsi, r9

    rep     movsb

    mov     rax, r8
    ret
