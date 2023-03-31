#include <stdio.h>
#include <stdlib.h>
#include "hexdump.h"

// a global variable
char global_ch = 'B';
// const makes this global variable an immutable constant
const char const_global_ch = 'C';

int main(void) {
    char ch = 'A';
    // Allocates space on the heap, like new in Java.
    char* heap_ch = (char*) malloc(1); 
    *heap_ch = 'D';

    hexdump(&ch, 1);
    hexdump(&global_ch, 1);
    hexdump(&const_global_ch, 1);
    hexdump(heap_ch, 1);
    hexdump(main, 16);
}

