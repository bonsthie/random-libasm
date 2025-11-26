
# YET AN OTHER LIBASM
my first time writing nasm asm (i'm a clang kind of guy) and first time really writing asm (something that is not a _start). normaly I only write x86 mir test for the llvm backend

## SUPPORTED ARCH

### X86-64
- [x] tester
- [x] cpuid info in .init_array
- [x] ifunc selector (forked nasm need to do a pull request)
- [x] strlen
    - [x] base
    - [x] sse2
    - [x] avx2
    - [x] avx512
- [ ] strcmp
    - [x] base
    - [ ] sse2
    - [ ] avx2
    - [ ] avx512
- [ ] strlcpy
    - [x] base
    - [ ] sse2
    - [ ] avx2
    - [ ] avx512
- [x] read
- [x] write
- [ ] strdup


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
