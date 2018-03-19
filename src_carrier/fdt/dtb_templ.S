#define STRINGIFY0(v) #v
#define STRINGIFY(v) STRINGIFY0(v)
    .global FDT_NAME
    .section .rodata
FDT_NAME:
    .incbin FDT_FILE
1: