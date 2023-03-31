/**
 * General test driver support
 *
 * Ben Wood, 2021.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "command_lines.h"
#include "command_support.h"

/**
 * Translate a status code (foreground/background) to a descriptive string.
 */
char* command_status_name(int status_code) {
  switch (status_code) {
  case 0:
    return "background";
  case 1:
    return "foreground";
  case INITIAL_STATUS:
    return "UNCHANGED";
  default:
    return "UNKNOWN";
  }
}

/**
 * Compare two command arrays for equivalence.
 */
bool command_compare(char** expected, char** actual) {
  for (int i = 0; expected[i] || actual[i]; i++) {
    if (!expected[i] != !actual[i]) {
      return false;
    }
    for (int j = 0; expected[i][j] || actual[i][j]; j++) {
      if (expected[i][j] != actual[i][j]) {
        return false;
      }
    }
  }
  return true;
}


/**
 * Apply the given check function to each command line in COMMAND_LINES.
 *
 * does_checks:
 * - when true, return values from check indicate pass/fail
 * - when false, return values from check do not have meaning and are expected to be true
 * purpose: short description of what the check function is doing, printed in summary.
 * note: additional note, printed in summary.
 */
int command_check(int argc, char** argv, bool (*check)(char*), bool does_checks, char* purpose, char* note) {
  if (argc == 2) {
    bool ok = check(argv[1]);
    if (ok) {
      printf("\n#### Status: Ok\n\n");
    } else {
      printf("\n#### Status: Error\n\n");
    }
    return !ok;
  } else {
    return command_lines_apply_all(check, does_checks, purpose, note);
  }
}

int command_lines_apply_all(bool (*check)(char*), bool does_checks, char* purpose, char* note) {
  // Count the command line test cases.
  int ntests;
  for (ntests = 0; COMMAND_LINES[ntests]; ntests++);

  int pass = 0, fail = 0;
  bool results[ntests];
  
  // Run a demo for each command line test case.
  for (int i = 0; COMMAND_LINES[i]; i++) {
    printf("#### Test %3d ##################################################################\n", i);
    results[i] = check(COMMAND_LINES[i]);
    if (results[i]) {
      pass++;
    } else {
      fail++;
    }
    printf("\n");
  }
  printf("#### Summary ###################################################################\n\n");
  printf("This driver runs "
#ifdef SOLN
         "ONLY SOLUTION CODE"
#else
         "YOUR CODE"
#endif
         " for %s.\n\n", purpose);
  if (note) {
    printf("%s\n\n", note);
  }
  printf("%4d test cases:\n", pass + fail);
  printf("%4d   without explicit errors", pass);
  if (pass && fail) {
    printf(" >> ");
    for (int i = 0; i < ntests; i++) {
      if (results[i]) {
        printf("%d ", i);
      }
    }
  }
  printf("\n");
  if (does_checks || fail) {
    printf("%4d   with    explicit errors", fail);
    if (fail) {
      printf(" >> ");
      for (int i = 0; i < ntests; i++) {
        if (!results[i]) {
          printf("%d ", i);
        }
      }
    }
    printf("\n");
  }
  if (pass + fail != ntests) {
    printf("%4d test cases were not completed\n", ntests - (pass + fail));
  }

  printf("\n################################################################################\n");
  return fail;
}


/**** ADVERSARIAL MALLOC/FREE SUPPORT ************************/

const char UNINIT[] = { '%', }; // 'U', 'N', 'I', 'N', 'I', 'T', '%', };
const char PADDING[] = { '@', }; // 'C', 'A', 'N', 'A', 'R', 'Y', '@', };
const char FREE[] = { '$', }; // '!', 'F', 'R', 'E', 'E', '!', '$', };
const char TERM[] = {
  '\a', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
};



static const size_t HEADER_BYTES = 2 * sizeof(size_t);
static const size_t PADDING_BYTES = 16;
static const size_t TERM_BYTES = sizeof(TERM);

static size_t block_size_from_payload_size(size_t payload_bytes) {
  return HEADER_BYTES + PADDING_BYTES + payload_bytes + PADDING_BYTES + TERM_BYTES;
}

static char* block_base(void* payload) {
  return ((char*) payload) - (HEADER_BYTES + PADDING_BYTES);
}

static void block_fill(void* payload_base, size_t payload_bytes, const char filler[], size_t filler_bytes) {
  char* base = block_base(payload_base);
  
  // Fill the size.
  size_t* header = (size_t*) base;
  header[0] = payload_bytes;
  header[1] = ~payload_bytes;

  // Fill the padding with bounds canaries.
  // Nearby accesses out of bounds will get distinctive results.
  // Forgetting to install a null terminator character will cause
  // printing of a heap string to show the canary.
  for (int i = 0; i < PADDING_BYTES; i++) {
    base[HEADER_BYTES + i] = PADDING[i % sizeof(PADDING)];
    base[HEADER_BYTES + PADDING_BYTES + payload_bytes + i] = PADDING[i % sizeof(PADDING)];
  }
  
  // Fill the terminator canary.
  // Forgetting to install a null terminator character will cause
  // printing of a heap string to end within actually malloc'd bounds,
  // but print a newline and a terminal bell.
  for (int i = 0; i < TERM_BYTES; i++) {
    base[HEADER_BYTES + PADDING_BYTES + payload_bytes + PADDING_BYTES + i] = TERM[i % sizeof(TERM)];
  }
  
  // Fill the payload with uninit canaries.
  // Failing to initialize within bounds will yield distinctive
  // results for later in-bounds accesses.
  for (int i = 0; i < payload_bytes; i++) {
    base[HEADER_BYTES + PADDING_BYTES + i] = filler[i % filler_bytes];
  }
}

void* command_adversarial_malloc(size_t payload_bytes) {
  char* block = (char*) malloc(block_size_from_payload_size(payload_bytes));
  if (block == NULL) {
    return NULL;
  }

  char* payload_base = block + HEADER_BYTES + PADDING_BYTES;
  block_fill(payload_base, payload_bytes, UNINIT, sizeof(UNINIT));

  // Return a pointer to the payload.
  return payload_base;
}

static void putc_escape(char ch) {
  const char escapes[] = "\a\b\f\n\r\t\v\"\\";
  const char escapees[] = "abfnrtv\"\\";
  for (int i = 0; escapes[i]; i++) {
    if (escapes[i] == ch) {
      printf("\\%c", escapees[i]);
      return;
    }
  }
  printf("%c", (' ' < ch && ch <= '~') ? ch : ' ');
}

static void print_suspect_bytes(char* base, size_t n) {
  printf(" as characters: \"");
  for (int i = 0; i < n; i++) {
    putc_escape(base[i]);
  }
  printf("\"    as bytes: ");
  for (int i = 0; i < n; i++) {
    printf("0x%02x ", base[i]);
  }
}
  
static long adversarial_errors = 0;


AdversarialBlockDesc command_adversarial_check(void* payload) {
  AdversarialBlockDesc desc = { 0 };
  
  // Recover block base and size.
  desc.base = block_base(payload);
  size_t* header = (size_t*) desc.base;
  size_t payload_bytes = header[0];
  size_t checksum = header[1];
  
  desc.header_safe = payload_bytes == ~checksum;
  if (desc.header_safe) {
    desc.payload_bytes = payload_bytes;
  } else {
    printf("#   [Error: out-of-bounds corruption affected some metadata necessary for checks in block @%p.]\n",
           payload);
    adversarial_errors++;
  }
  
  // Check bounds canaries beyond start.
  desc.prefix_safe = true;
  for (int i = 0; i < PADDING_BYTES; i++) {
    if (desc.base[HEADER_BYTES + i] != PADDING[i % sizeof(PADDING)]) {
      printf("#   [Error: corruption out of bounds %s %d byte(s) before the start of block @%p\n",
             i == 0 ? "from at least" : "from",
             (int)PADDING_BYTES - i, payload);
      printf("#          ");
      print_suspect_bytes(desc.base + HEADER_BYTES + i, PADDING_BYTES - i);
      printf("]\n");
      desc.prefix_safe = false;
      adversarial_errors++;
      break;
    }
  }

  if (desc.header_safe) {
    // Check bounds canaries beyond end, only if size was not likely corrupted.
    desc.suffix_safe = true;
    for (int i = PADDING_BYTES - 1; i >= 0; i--) {
      if (desc.base[HEADER_BYTES + PADDING_BYTES + payload_bytes + i] != PADDING[i % sizeof(PADDING)]) {
        printf("#   [Error: corruption out of bounds %s %d byte(s) beyond the end of block @%p\n",
               i == PADDING_BYTES - 1 ? "through at least" : "through",
               i + 1, payload);
        printf("#          ");
        print_suspect_bytes(desc.base + HEADER_BYTES + PADDING_BYTES + payload_bytes, i + 1);
        printf("]\n");
        desc.suffix_safe = false;
        adversarial_errors++;
        break;
      }
    }
  }

  return desc;
}

void command_adversarial_clear(void* payload_base, size_t payload_bytes) {
  block_fill(payload_base, payload_bytes, FREE, sizeof(FREE));
}

void command_adversarial_free(void* payload) {
  // Check canaries.
  AdversarialBlockDesc desc = command_adversarial_check(payload);
  if (desc.header_safe) {
    command_adversarial_clear(payload, desc.payload_bytes);
  }
  
  free(desc.base);
}

long command_adversarial_errors() {
  long n = adversarial_errors;
  adversarial_errors = 0;
  return n;
}
