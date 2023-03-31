/**
 * Test driver for command_show
 *
 * Ben Wood, 2021.
 */
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "command.h"
#include "command_support.h"

/**
 * Test command_show functions on the correct command array for the
 * given command line.  line: command line input alloc: true to do
 * tests in the heap, false otherwise.
 */
static bool command_show_test(char* raw_line) {
  int status;
  char** command = soln_command_parse(raw_line, &status);
  if (command == NULL) {
    printf("# Skipping invalid command since a command array cannot be created.\n");
    return true;
  }

  printf("# Command line: \"%s\"\n", raw_line);  

  printf("# > actual   command_show output:\n");
  fflush(stdout);
  command_show(command);
  fflush(stdout);
  
  printf("# > expected command_show output:\n");
  soln_command_show(command);

  soln_command_free(command);

  // This function does no checking. It provides outputs for visual
  // comparison by the user.
  return true;
}

/**
 * main is the top-level function that executes when the compiled
 * executable program is run.
 */
int main(int argc, char** argv) {
  return command_check(argc, argv, command_show_test, false,
                       "command_show",
                       "This driver does not check correctness automatically.\n"
                       "Use manual inspection of the actual vs. expected outputs\n"
                       "and valgrind messages to check correctness.");
}
