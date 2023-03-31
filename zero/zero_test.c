/**
 * CS 240 Zero: make zero without zero.
 *
 * zero_test.c: testing code for the puzzles.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "zero.h"

static const unsigned char EXPECTED = 0x00;

bool test(unsigned char (*fp)(unsigned char), char* name) {
  printf("Testing %s... ", name);
  for (unsigned i = 0; i <= 0xFF; i++) {
    unsigned char actual = fp(i);
    if (actual != EXPECTED) {
      printf("at least one test failed.\n");
      printf("  %s(0x%02x) returned 0x%02x\n", name, i, actual);
      printf("  [Skipping remaining tests for %s]\n", name);
      return false;
    }
  }
  printf("all tests passed!\n");
  return true;
}

/**
 * The main function runs when the program is executed.
 * It tests your functions on all possible byte values.
 *
 * Do not change this code.
 */
int main(int argc, char** argv) {
  int failed = 0;

  failed += !test(zero_minus,    "zero_minus");
  failed += !test(zero_xor,      "zero_xor");
  failed += !test(zero_and_not,  "zero_and_not");
  failed += !test(zero_or_not,   "zero_or_not");
  failed += !test(zero_plus_not, "zero_plus_not");

  return failed;
}
