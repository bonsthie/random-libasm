
section .text
global __strcpy_avx2:hidden

%define PAGE_SIZE 4096
%define VEC_SIZE 32

; char	*__strcpy_avx2(char *dest, char *src)
; rdi -> dest
; rsi -> src
__strcpy_avx2:
