
%include "syscall.inc"

section .text
global __write:hidden

; ssize_t ft_write(int fd, const char *str, size_t size);
; fd -> rdi
; str -> rsi
; size -> rdx
__write:
	 SYSCALL_CALL __NR_write
	
global ft_write:weak
ft_write equ __write
