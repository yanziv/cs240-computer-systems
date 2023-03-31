/**
 * A simple command parsing library.
 *
 * More information, documentation, examples:
 * https://cs.wellesley.edu/~cs240/assignments/pointers/
 */

#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <stdbool.h>

/** Determine whether a character is a space for parsing purposes. */
bool command_char_is_space(char ch);

/** Parse a command-line string to a command array. */
char** command_parse(char* line, int* foreground);

/** Show the structure of a command array. */
void command_show(char** command);

/** Free all parts of a command array. */
void command_free(char** command);

#ifdef REPLACE_PRINTF
#define printf(...) fprintf(stderr, __VA_ARGS__)
#endif // REPLACE_PRINTF

#endif // _COMMAND_H_
