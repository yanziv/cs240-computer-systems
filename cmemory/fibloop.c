/**
 * Debugging challenge: crasher.
 */
#include <stdlib.h>
#include <stdio.h>

/**
 * Compute the 0th - nth Fibonacci numbers using a loop to accumulate
 * the sequence in an array.
 *
 * Fibonacci sequence:
 * Index: 0 1 2 3 4 5 6  7
 * Value: 1 1 2 3 5 8 13 21 ...
 */
int* fibs(int n) {
  // Pointer to int array to hold sequence up through n.
  int* fibs = (int*)malloc((n+1) * sizeof(int));
  // Fill array with sequence.
  for (int i = 0; i <= n; i++) {
    if (i == 0 || i == 1) {
      fibs[i] = i;
    } else {
      fibs[i] = fibs[i-2] + fibs[i-1];
    }
  }
  return fibs;
}
int main(int argc, char** argv) {
  int* result1 = fibs(5);  // has 6 values
  int* result2 = fibs(7);  // has 8 values
  
  printf("result1 = ");
  for (int i = 0; i <= 5; i++) {
    printf("%d ", result1[i]);
  }
  printf("\n");


  printf("result2 = ");
  for (int i = 0; i <= 7; i++) {
    printf("%d ", result2[i]);
  }
  printf("\n");
  
  free(result1);
  free(result2);
  return 0;
}
