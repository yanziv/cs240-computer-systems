#include <stdlib.h>
#include <stdio.h>
#include "hexdump.h"

int main() {
                // a[0]      a[1]       a]2]         a[3]        a[4]
  int a[4] = { 0x41424344, 0x45464748, 0x494a4b4c, 0x4d4e4f50 };
  char* p0 = (char*)(a + 3);
  printf("%c\n", *p0);       // char that correeposned to 50
  char* p1 = ((char*)a) + 3; 
  printf("%c\n", *p1);   // char asc value that corresponds to 41
  for (char* p2 = (char*)a; p2 < (char*)(a + 4); p2++) {
    printf("%c ", *p2);    // print every char in array until we get past array
  }
  printf("\n");
}
