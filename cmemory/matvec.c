/**
 * Debugging challenge: incorrect results.
 */

#include <stdlib.h>
#include <stdio.h>

/**
 * Return the product of the NxN matrix A by the N-element vector x.
 *
 * Dynamically allocates an N-int array to store the result vector.
 * The caller is responsible for freeing it.
 *
 * All comments and logic are correct, given one key (incorrect)
 * assumption about memory allocation.
 */
int* matvec(int N, int A[N][N], int x[N]) {

  // Dynamically allocate a contiguous chunk of memory to hold N ints:
  // an N-element array to hold the answer.
  int* y = (int*)malloc( N * sizeof(int) );

  // Row index i, column index j.
  int i, j;

  // y[i] is the sum of A[i][j]*x[j] from j = 0 to j = N-1.
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      y[i] += A[i][j] * x[j];
    }
  }

  // Return the result.
  return y;
}

/**
 * Test driver for matvec.
 */
int main(int argc, char** argv) {

  // A sample 4x4 matrix.
  int matrix[4][4] = {
    { 2,4,7,5 },
    { -3,5,0,9 },
    { 2,-5,9,1 },
    { -2,-4,1,0 }
  };
  
  // A sample 4-vector.
  int vector[4] = { 1, 4, 3, 5 };

  // Compute the product repeatedly.
  int reps;
  for (reps = 0; reps < 4; reps++) {
    // Compute the result vector.
    int* result = matvec(4, matrix, vector);
    // Print the result vector.
    int i;
    printf("result %d =  ", reps);
    for (i = 0; i < 4; i++) {
      printf("%d ", result[i]);
    }
    printf("\n");
    // Free the result vector.
    free(result);
  }

  // Return from main (0 means success).
  return 0;
}
