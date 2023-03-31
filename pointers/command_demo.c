/**
 * Demo driver for all command functions
 *
 * Ben Wood, 2015 - 2021.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "command.h"
#include "command_lines.h"
#include "command_support.h"

/**
 * command_demo:
 *
 * Take a command line string and demonstrate the usage of the command
 * library to parse and potentially show and free a command array.
 */
static bool command_demo(char* line) {
  printf("# Parsing \"%s\"\n", line);
  
  // The client may request parsing, giving any well-formed C string
  // and the address of any accessible memory location where a valid
  // command line's status can be stored.
  int status;
  char** command = command_parse(line, &status);
  
  // The command_parse return value distinguishes two cases:
  if (command == NULL) {

    // The command line was invalid, yielding no other useful
    // information to the client.
    printf("# > reports invalid command line\n");
    
  } else {
    
    // The command line was valid, yielding (1) a pointer return value
    // that refers to a heap-allocated command array and (2) an
    // updated status reflecting the status of the command line.
    
    // The command array and the command status can be used by the
    // client any number of times.
    printf("# > reports valid command line\n");
    printf("# > with command status %s (%d)\n", command_status_name(status), status);
    printf("# > and command array @%p\n", command);
    command_show(command);
    
    // The client must free the command exactly once at some point
    // with command_free.
    printf("# Freeing command array\n");
    command_free(command);
    printf("# > done\n");

    // After freeing the command array, the client must never access
    // it again.
    
  }

  // This function does no checking. It assumes all command library
  // functions work correctly.
  return true;
}  


/**
 * main is the top-level function that executes when the compiled
 * executable program is run.
 */
int main(int argc, char** argv) {
  return command_check(argc, argv, command_demo, false,
                       "all the command functions for demonstration\n"
                       "or incremental testing during development",
                       "This driver does not check correctness automatically.\n"
                       "Use manual inspection of the expected outputs and valgrind messages\n"
                       "to check correctness. Or add your own checks in command_demo.c.");
}
