/**
 * A simple command parsing library.
 *
 * More information, documentation, examples:
 * https://cs.wellesley.edu/~cs240/assignments/pointers/pointers.html
 */

#ifndef _COMMAND_SOLN_H_
#define _COMMAND_SOLN_H_

/** Parse a command-line string to a command array. */
char** soln_command_parse(char* line, int* foreground);

/** Show the structure of a command array. */
void soln_command_show(char** command);

/** Pprint a command array in the form of a command line. */
void soln_command_print(char** command);

/** Free all parts of a command array. */
void soln_command_free(char** command);

#endif // _COMMAND_SOLN_H_
