/**
 * Debugging challenge: crasher.
 */
#include <stdlib.h>
#include <stdio.h>

/**
 * Linear search in an array, where the array is manipulated using
 * pointers and pointer arithmetic.
 *
 * Precondition:
 * Only call with non-NULL elems and val such that elems contains val.
 *
 * Return:
 * A pointer to first occurence of val in array elems.
 */
int* search(int* elems, int val) {

  // Use a pointer to scan elems (instead of array indexing).
  // Initially, the cursor points to the first element in elems.
  int* cursor = elems;
  
  // While the element at the cursor is not equal to val, adjust the
  // cursor to point to the next element in the int array.
  while (*cursor != val) {
    cursor += sizeof(int);
  }

  // Return final cursor position, which should point to a location
  // holding the samve number as val.
  return cursor;
}

/**
 * Test driver for search.
 */
int main(int argc, char** argv) {

  // Sample array.
  int array[25] = {
    110, 111, 112, 114, 117,
    215, 220, 230, 231, 232,
    235, 240, 242, 249, 251,
    301, 304, 307, 310, 313,
    315, 320, 332, 342, 349
  };

  // Search for 231.
  int* pointerTo231 = search(array, 231);
  printf("Found %d\n", *pointerTo231);

  // Search for 240.
  int* pointerTo240 = search(array, 240);
  printf("Found %d\n", *pointerTo240);

  return 0;
}
