%include "syscall.inc"

section .text
global __read:hidden

; ssize_t ft_read(int fd, void *buf, size_t size);
; fd -> rdi
; but -> rsi
; size -> rdx
__read:
	 SYSCALL_CALL __NR_read
	
global ft_read:weak
ft_read equ __read
