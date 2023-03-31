/**
 * Test driver for command_free
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
 * Test command_free on the correct command array for the given
 * command line string.
 */
static bool command_free_test(char* raw_line) {
  int status;
  char** command = soln_command_parse(raw_line, &status);
  if (command == NULL) {
    printf("# Skipping invalid command since a command array cannot be created.\n");
    return true;
  }

  printf("# Command array to free\n");
  soln_command_show(command);
  printf("# Calling command_free\n");
  fflush(stdout);
  command_free(command);
  fflush(stdout);
  printf("# > done\n");

  // This function does no checking. It provides a memory usage
  // measurement target for valgrind.
  return true;
}

/**
 * main is the top-level function that executes when the compiled
 * executable program is run.
 */
int main(int argc, char** argv) {
  return command_check(argc, argv, command_free_test, false,
                       "command_free",
                       "This driver does not check correctness automatically.\n"
                       "Use manual inspection of valgrind output to check for memory errors or leaks.");
}
