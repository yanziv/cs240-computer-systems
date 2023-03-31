/**
 * Test cases
 *
 * The COMMAND_LINES array lists command-line test cases that will be
 * used as arguments for testing command_parse. Add more!
 */
#include <stdlib.h>

#include "command_lines.h"

char* COMMAND_LINES[] = {
  "Hello world!  Please parse    me.",
  "ls -l cs240-pointers &",
  "a & b",
  "& this is    a   test case.",
  "  &  hey hello &&   ",
  "  what r   u      doing   ",
  " hellp sdkfj   ehhuu hhej    &",
  " Another TEST CASE.",
  "A new & TEST CASE",
  "More    T   &*   *& test cases!",
  "This is the pointers assignment.",
  "This     assignment is    quite challenging&   ",
  " a ^  0  == 9",
  "keep writing test cases is fun",
  "test cases      &",
  "    777   moon    jupiter  &  earth ",
  "There will always be && & more test cases!",
  "secret code   &   ",
  " DATA ENCRYPTION   !&",
  "cryptography  computer systems distributed systems    *",
  "sudoku   adversarial searchhh   N;p   ",
  "  & &funfunfun  !",
  "  this is & short test case  ",
  "  today is October  30th  and tomorrow & is tomorrow.", 
  
  
  // Be adversarial. Test all possible corner cases of valid and
  // invalid command line shapes that you can imagine. Add small
  // simple cases. Add large complex cases.
  
  NULL // Keep NULL to mark the end.
};
