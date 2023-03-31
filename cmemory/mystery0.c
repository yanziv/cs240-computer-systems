#include <stdlib.h>
#include <stdio.h>
#include "hexdump.h"

void mystery0(char* ps, char* pa) {  /// copies contents of ps into contents of pa
  while (*ps) {
    *pa = *ps;
    ps++;
    pa++;
  }
  *pa = '\0';
}

int main() {
  char a[7] = {'h', 'e', 'l', 'l', 'o', '!', '\0'};
  char b[7];

  // hexdump(b, 16);
  printf("\n");
  
  mystery0(a, b);
  // hexdump(b, 16);
  printf("a = \"%s\"\n", a);      // "hello!"
  printf("b = \"%s\"\n\n", b);    // "hello!"

  mystery0("cs 240", a);
  // hexdump(b, 16);
  printf("a = \"%s\"\n", a);  // "cs 240"
  printf("b = \"%s\"\n\n", b);  // "hello!"

  mystery0("0xF", &a[2]);   // contents of a[2] gets "0xF" (add 3 char to array)
  // hexdump(b, 16);
  printf("a = \"%s\"\n", a);  // "cs 0xF"
  printf("b = \"%s\"\n\n", b);  // "hello!"

}
