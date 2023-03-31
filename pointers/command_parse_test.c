/**
 * Test driver for command_show
 *
 * Ben Wood, 2021.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "command.h"
#include "command_lines.h"
#include "command_support.h"

static bool command_parse_test(char* raw_line) {
  int line_length = strlen(raw_line);
  
  // Expected result and status.
  int expected_status = INITIAL_STATUS;
  char** expected_command = soln_command_parse(raw_line, &expected_status);

  // Put the line in the heap to help valgrind detect bounds errors.
  char* line = malloc(sizeof(char) * (strlen(raw_line) + 1));
  if (line == NULL) {
    perror("malloc line copy");
    exit(2);
  }
  strcpy(line, raw_line);
  printf("# Parsing \"%s\"\n", line);

  // Actual result and status.
  int status = INITIAL_STATUS;
  char** command = command_parse(line, &status);

  // Check that the line has been respected.
  if (strncmp(line, raw_line, line_length)) {
    printf("#   [Error: command line string @%p has been mutated by command_parse.]\n", line);
  }
  // Trip up implementations that use aliasing.
  free(line);

  // Did the actual validity match the correct validity?
  bool validity_ok = !command == !expected_command;
  printf("# > reports %s command line\n"
         "#   [%s: expected %s.]\n",
         command ? "valid" : "invalid",
         validity_ok ? "Ok" : "Error",
         expected_command ? "valid" : "invalid");


  // Did the actual status match the expected status?
  // Was the actual status at least consistent (for some command) with
  // the actual validity?
  const bool expected_status_changed = expected_status != INITIAL_STATUS;
  const bool status_changed = status != INITIAL_STATUS;
  const bool validity_status_consistent = !command == !status_changed;
  const bool status_ok = status == expected_status && validity_status_consistent;
  // Was actual status initialized?
  if (status_changed) {
    printf("# > with command status %s (%d)\n",
           command_status_name(status), status);
  } else {
    printf("# > with no command status\n");
  }
  // Was the presence or absence of status initialization consistent
  // with the actual validity, disregarding expected status/validity?
  if (!status_ok) {
    printf("#   [Note: %s return value with %s change is %s.]\n",
           command ? "non-NULL" : "NULL",
           status == INITIAL_STATUS ? "no status" : "status",
           validity_status_consistent ? "consistent" : "inconsistent");
  }
  // Did the actual status match the expected status?
  if (expected_status_changed) {
    printf("#   [%s: expected status %s (%d) for this valid command.]\n",
           status == expected_status ? "Ok" : "Error",
           command_status_name(expected_status), expected_status);
  } else {
    printf("#   [%s: expected no status for this invalid command.]\n",
           status == expected_status ? "Ok": "Error");
  }

  
  bool array_ok = validity_ok;
  if (command && validity_ok) {
    printf("# > and command array @%p\n", command);
    array_ok = command_compare(expected_command, command);
    printf("#   [%s: command array is %s.]\n",
           array_ok ? "Ok" : "Error",
           array_ok ? "correct" : "malformed");
    soln_command_show(command);
    if (!array_ok) {
      printf("#   [Note: expected command array as follows.]\n");
      soln_command_show(expected_command);
    }
  }

  if (expected_command) {
    soln_command_free(expected_command);
  }

  if (command) {
    printf("# Freeing command array\n");
    command_free(command);
    printf("# > done\n");
  }

  return validity_ok && status_ok && array_ok && !command_adversarial_errors();
}  


/**
 * main is the top-level function that executes when the compiled
 * executable program is run.
 */
int main(int argc, char** argv) {
  return command_check(argc, argv, command_parse_test, true,
                       "command_parse",
                       "This driver checks several aspects of correctness automatically.\n\n"
#ifdef ADVERSARIAL
                       "This variant uses an adversarial malloc/free that make memory safety\n"
                       "violations more likely to cause crashes or obvious corruption.\n"
                       "However, this intervention causes valgrind to report many fewer errors.\n"
                       "Be sure to run the non-adversarial variant as well.\n"
#else
                       "This variant is well suited for running under valgrind.\n"
                       "Be sure to try the adversarial variant as well. It turns errors that\n"
                       "are often silent (without valgrind) into crashes or obvious data corruption."
#endif
                       );
}
