section .text
default rel

extern __strlen_base:function
extern ft_write:function

global __strlen_ifunc:function
__strlen_ifunc:
	mov	 rdi, 1
	lea	 rsi, [rel ayes_msg]
	mov rdx, ayes_len
	call ft_write

.crampte:
    lea rax, [rel __strlen_base]
    ret

global ft_strlen:ifunc
ft_strlen equ __strlen_ifunc


section .rodata
ayes_msg:        db "yes", 10, 0
ayes_len         equ $ - ayes_msg
