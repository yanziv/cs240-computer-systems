#include <stdio.h>
#include <stdlib.h>
#include "hexdump.h"

char global_ch = 'B';
int global_int = 0x12345678;
unsigned long global_ulong = 0xdeadbeef;


const char const_global_ch = 'C';
const int const_global_int = 0x23456789;
const unsigned long const_global_ulong = 0xfeedbeef;

int main(void) {
    char ch = 'A';
    int l_int = 0x3456789a;
    unsigned long l_ulong = 0xcafebeef;

    char* heap_ch = (char*) malloc(1);
    int *heap_int = (int *)malloc(sizeof(int));
    unsigned long *heap_ulong = (unsigned long *)malloc(sizeof(unsigned long));
    *heap_ch = 'D';
    heap_int = (char*)0x456789ab;
    heap_ulong = (unsigned long*)0xc001beef;

    hexdump(&ch, 16);
    printf("\n");
    hexdump(&global_ch, 16);
    printf("\n");
    hexdump(&const_global_ch, 16);
    printf("\n");
    hexdump(heap_ch, 16);
    printf("\n");
    hexdump(main, 16);
}

