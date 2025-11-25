
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; CPU_INFO struct utils
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; This is a smaller and more organized way of storing CPUID feature info.
; It removes the entries I don't need and keeps the representation simpler
; than the full CPUID leaf system and its unusual ordering.

struc cpu_info_s
	.SSE resb 1
	.AVX resb 1
	.AVX512 resb 1
endstruc

%define CPU_INFO(field) [rel __cpu_info + cpu_info_s.%+field]
%define CPU_INFO_BIT(field, version) CPU_INFO_BIT_%+field%+version

;; in CPU_INFO(sse)
%define CPU_INFO_BIT_SSE1		(1)
%define CPU_INFO_BIT_SSE2		(1 << 1)
%define CPU_INFO_BIT_SSE4_1		(1 << 2)
%define CPU_INFO_BIT_SSE4_2		(1 << 3)

;; in CPU_INFO(avx)
%define CPU_INFO_BIT_AVX1		(1)
%define CPU_INFO_BIT_AVX2		(1 << 1)

;; in CPU_INFO(avx512)
%define CPU_INFO_BIT_AVX512F	(1)
%define CPU_INFO_BIT_AVX512VL	(1 << 1)
%define CPU_INFO_BIT_AVX512BW	(1 << 2)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; CPUID LEAF 1 (EAX=1)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%define CPUID_BIT(field, version) CPUID_BIT_%+field%+version

; ECX bits
%define CPUID_BIT_SSE3       (1 << 0)
%define CPUID_BIT_SSSE3      (1 << 9)
%define CPUID_BIT_FMA        (1 << 12)
%define CPUID_BIT_SSE4_1     (1 << 19)
%define CPUID_BIT_SSE4_2     (1 << 20)
%define CPUID_BIT_XSAVE      (1 << 26)
%define CPUID_BIT_OSXSAVE    (1 << 27)
%define CPUID_BIT_AVX1		 (1 << 28)

; EDX bits
%define CPUID_BIT_MMX        (1 << 23)
%define CPUID_BIT_SSE1       (1 << 25)
%define CPUID_BIT_SSE2       (1 << 26)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; CPUID LEAF 7, SUBLEAF 0 (EAX=7, ECX=0)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; EBX bits
%define CPUID_BIT_ERMS         (1 << 9)
%define CPUID_BIT_AVX2         (1 << 5)
%define CPUID_BIT_AVX512F      (1 << 16)
%define CPUID_BIT_AVX512DQ     (1 << 17)
%define CPUID_BIT_AVX512IFMA   (1 << 21)
%define CPUID_BIT_AVX512PF     (1 << 26)
%define CPUID_BIT_AVX512ER     (1 << 27)
%define CPUID_BIT_AVX512CD     (1 << 28)
%define CPUID_BIT_AVX512BW     (1 << 30)
%define CPUID_BIT_AVX512VL     (1 << 31)

; 1 -> cpu_info field
; 2 -> field version
; 3 -> reg of the cpuid
%macro DETECT_CPU_FEAT 3
	mov			r8b, byte CPU_INFO(%1)
	test		%3, CPUID_BIT(%1, %2)
	jz			%%.end

	or			r8b, CPU_INFO_BIT(%1, %2)
	Mov			byte CPU_INFO(%1), r8b

%%.end:
%endmacro

; set the flag if has cpu feature
; 1 -> cpu_info field
; 2 -> field version
%macro HAS_CPU_FEAT 2
	mov		al, byte CPU_INFO(%1)
	test	al, CPU_INFO_BIT(%1, %2)
%endmacro

; set the flag if has cpu feature
; 1 -> cpu_info field
; 2 -> field version
%macro PRINT_HAS_CPU_FEAT 2
	mov		edi, 1
	lea		rsi,  [rel has_%+%1%+%2]
	mov		edx, has_%+%1%+%2%+_len
	call	ft_write

	xor		esi,esi
	HAS_CPU_FEAT %1, %2
	setnz	sil
	call	__print_yes_or_no
%endmacro


; create a db string with len
; 1 -> var name
; 2 -> string
%macro STRING 2 
%1:			db %2, 0
%1%+_len	equ $ - %1
%endmacro


