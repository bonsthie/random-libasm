
# YET AN OTHER LIBASM
my first time writing nasm asm (i'm a clang kind of guy) and first time really writing asm (something that is not a _start). i only write x86 mir test for the llvm backend

## SUPPORTED ARCH

### X86-64
    - [ ] cpuid feat (en cour)
    - [ ] ifunc selector
    - [ ] strlen
        - [x] base
        - [x] sse2
        - [x] avx2
        - [ ] avx512
    - [ ] strcmp
        - [x] base
        - [ ] sse2
        - [ ] avx2
        - [ ] avx512
    - [ ] strlcpy
        - [ ] base
        - [ ] sse2
        - [ ] avx2
        - [ ] avx512
    - [x] read
    - [x] write
    - [ ] strdup


### AARCH64
    - [ ] cpuid feat (en cour)
    - [ ] ifunc selector
    - [ ] strlen
        - [ ] neon
        - [ ] sve2
    - [ ] strcmp
        - [ ] neon
        - [ ] sve2
    - [ ] strlcpy
        - [ ] neon
        - [ ] sve2
    - [ ] read
    - [ ] write
    - [ ] strdup
