#include <stdio.h>
#include <stdlib.h>
#include "hexdump.h"

void main6() {
    char ch = 'A';
    char* ptr = &ch;

    hexdump(ptr, 32);
    ptr[20] = 'B';
    printf("%c\n", ptr[20]);
    hexdump(ptr, 32);
}
int main(void) {
  main6();
}

