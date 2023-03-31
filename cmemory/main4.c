#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char ch = 'A';
    char* ptr = &ch;
    ptr[20] = 'B';
    printf("%c\n", ptr[20]);
}

