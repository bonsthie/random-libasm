extern __errno_location

%macro SYSCALL_RET 0
    cmp     rax, -4095
    jle     %%error
    ret

%%error:
	mov		rdi, rax
    neg     rdi 
    call    __errno_location wrt ..plt
    mov     [rax], rdi
    mov     rax, -1
    ret
%endmacro

%macro SYSCALL_CALL 1
	mov		rax, %1
	syscall
	SYSCALL_RET
%endmacro

%define __NR_read 0
%define __NR_write 1
