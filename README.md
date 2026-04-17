# R8ASM

---

```nasm

section .data
    UNIT_MAX 255

section .text

%macro erase 1
    .tp %1
    stp
    .xor UNIT_MAX
    btp
    .loop
        biz
            flb
            ror
        rnz
    .endloop
%endmacro

_start:
    .xor 15
    erase $

```

##
