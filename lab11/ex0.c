#include <stdio.h>
 
int elements[] = {0x1,0x3,0x5,0x7,0x9,0x11,0x13,0x15};
long aindex;

int getElement(int* baseAddress,int index) {
    return *(baseAddress + index);
 }
 
int main() {
  printf("Enter an array index: ");
  scanf("%lx",&aindex);
  printf("The value of the array element is: %x\n",getElement(elements,aindex));
  return 0;
}
