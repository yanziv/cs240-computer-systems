#include <stdlib.h>
#include <stdio.h>
#include "hexdump.h"

void copy(char* src, char* dst) {   // copies the content of pointer src into the contents of pointer dst
  while (*src) {
    *dst = *src;
    src++;
    dst++;
  }
  *dst = '\0';
}

int main() {
  char* p;
  char a[8];  
  int x = 19;
  char b[4];

  p = &a[4];

  // hexdump(b, 32);
  printf("\n");
  
  copy("Hello!", a);    // contents of a gets "Hello!"
  copy("Hi!", b);       // contents of b gets "Hi!"
  // hexdump(b, 32);
  printf("x = %d\n", x);   // 19
  printf("p = %p\n", p);   // prints the address of p (address of a[4]) -> 0x6c20796c6c616572
  printf("a = \"%s\"\n", a);  // "Hello!"
  printf("b = \"%s\"\n\n", b); // "Hi"

  copy("Hi, CS 240!", b);   // contents of b gets "Hi, CS 240!"
  // hexdump(b, 32);
  printf("x = %d\n", x);   // 19
  printf("p = %p\n", p);    // prints the address of p (address of a[4]) -> 0x6c20796c6c616572
  printf("a = \"%s\"\n", a);  // "Hello!"
  printf("b = \"%s\"\n\n", b);  // "Hi, CS 240!"

  copy("What happens if we use a really really long string?", b);
  // hexdump(b, 64);     // contents of b also overflowed and overwrote important value -> return address
  printf("x = %d\n", x);    // 19 X -> 1769108595
  printf("p = %p\n", p);    // prints the address of p (address of a[4])  -> 0x6c20796c6c616572
  printf("a = \"%s\"\n", a);  // "Hello!" X -> contents in b overflow to contents in a
  printf("b = \"%s\"\n\n", b); // "What happens if we use a really really long string?"


// segmentation fault error 
// last 4 print statements not reached
  copy("Hi?", p);  
  // hexdump(b, 64);
  printf("x = %d\n", x); 
  printf("p = %p\n", p);  
  printf("a = \"%s\"\n", a);  
  printf("b = \"%s\"\n\n", b); 

}
