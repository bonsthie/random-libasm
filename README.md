
# YET AN OTHER LIBASM
my first time writing nasm asm (i'm a clang kind of guy) and first time really writing asm (something that is not a _start). normaly I only write x86 mir test for the llvm backend

## SUPPORTED ARCH

### X86-64
- [x] tester
- [x] cpuid info in .init_array
- [x] ifunc selector (fork of nasm is curently evalueted)
- [ ] strlen
    - [x] test
    - [ ] ifunc
    - [x] base
    - [x] sse2
    - [x] avx2
    - [x] avx512
- [ ] strcmp
    - [x] test
    - [ ] ifunc
    - [x] base
    - [ ] sse2
    - [ ] avx2
    - [ ] avx512
- [ ] strlcpy
    - [x] test
    - [ ] ifunc
    - [x] base
    - [ ] sse2
    - [ ] avx2
    - [ ] avx512
- [x] read
    - [x] func
    - [x] test
- [x] write
    - [x] func
    - [x] test
- [ ] strdup
    - [x] func
    - [ ] test


### AARCH64
- [ ] cpuid info in .init_array
- [ ] ifunc selector
- [ ] strlen
    - [ ] base
    - [ ] neon
    - [ ] sve2
- [ ] strcmp
    - [ ] base
    - [ ] neon
    - [ ] sve2
- [ ] strlcpy
    - [ ] base
    - [ ] neon
    - [ ] sve2
- [ ] read
- [ ] write
- [ ] strdup
