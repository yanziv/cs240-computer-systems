/**
 * Debugging challenge: crasher.
 *
 * See note in main about re-running.
 */
#include <stdlib.h>
#include <stdio.h>

/**
 * Compute the nth Fibonacci number using a loop to accumulate
 * the sequence in an array.
 *
 * Fibonacci sequence:
 * Index: 0 1 2 3 4 5 6  7
 * Value: 1 1 2 3 5 8 13 21 ...
 */
int fib(int n) {
  // Pointer to int array to hold sequence up through n.
  int* fibs = (int*)malloc(n * sizeof(int));;
  // Fill array with sequence.
  for (int i = 0; i <= n; i++) {
    if (i == 0 || i == 1) {
      fibs[i] = i;
    } else {
      fibs[i] = fibs[i-2] + fibs[i-1];
    }
  }
  return fibs[n];
}

int main(int argc, char** argv) {
  // Try running with bound of 10, 100, 1000, 10000, ... (until things go wrong)
  // Don't forget to recompile when you change this.
  // Be prepared for your machine to slow down.
  for (int i = 0; i < 10; i++) {
    printf("Round %d: fib(1000000) = %d\n", i, fib(1000000));
  }
  return 0;
}
