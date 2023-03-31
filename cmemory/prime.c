/* CS 240 program to check if a number is prime */
#include <stdio.h>

int test_prime(int num) { 
  int i;
  int prime=1; //assume prime initially
 
  for (i=2; i<=num/2; ++i) { 
    if (num%i == 0) {
      prime = 0; // set to not prime
      break;
    }
  }

  if (prime) {
    printf("%d is prime\n", num);
  } else {
    printf("%d is not prime\n", num);
  }
  return 0;
}
 
int main() { 
  int test1 = 5;
  int test2 = 12;
  test_prime(test1);
  test_prime(test2);
  return 0;
}
