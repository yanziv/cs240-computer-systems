#ifndef _COMMAND_SUPPORT_H_
#define _COMMAND_SUPPORT_H_

#include <stdbool.h>

#include "soln_command.h"

#define INITIAL_STATUS 0xDECADE

char* command_status_name(int status_code);
bool command_compare(char** expected, char** actual);
int command_lines_apply_all(bool (*check)(char*), bool does_checks, char* purpose, char* note);
int command_check(int argc, char** argv, bool (*check)(char*), bool does_checks, char* purpose, char* note);

typedef struct AdversarialBlockDesc {
  char* base;
  bool header_safe;
  bool prefix_safe;
  bool suffix_safe;
  size_t payload_bytes;
} AdversarialBlockDesc;
  
void command_adversarial_fill(void* block, size_t payload_bytes);
void* command_adversarial_malloc(size_t bytes);
AdversarialBlockDesc command_adversarial_check(void* payload);
void command_adversarial_clear(void* raw_base, size_t raw_bytes);
void command_adversarial_free(void* payload);
long command_adversarial_errors();

#endif
