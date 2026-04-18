# R8ASM

---

```nasm

section .data
    UNIT_MAX 255

section .text

%macro erase 1
    .tp %1
    .loop UNIT_MAX
        biz
            flb
        rnz
        ror
    .endloop
%endmacro

_start:
    .xor 15
    erase $

```

##
