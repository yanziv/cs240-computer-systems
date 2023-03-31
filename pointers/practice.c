/**
 * Try these practice string functions to work with pointers.
 */
// Include functions from the standard C library.
#include <stdlib.h>
// Include string functions (for testing)
#include <string.h>
// Include functions to support input and output (printing).
#include <stdio.h>
// Include assertions
#include <assert.h>

/** PRACTICE FUNCTIONS *******************************************************/

/**
 * Return the length of str, not counting the null terminator
 * character.
 *
 * Precondition: str is a well-formed C string.
 *
 * Use array indexing. Do not use any other functions.
 */
int string_length_a(char str[]) {
  int length = 0;
  while (str[length] != '\0') {
    length++;
  }
  return length;
}

/**
 * Return the length of str, not counting the null terminator
 * character.
 *
 * Precondition: str is a well-formed C string.
 *
 * Do not use array indexing.  [] is banned.  Use pointer arithmetic instead.
 * Do not use any other functions.
 */
int string_length_p(char* str) {
  char* cursor = str;
  while (*cursor != '\0') {
    cursor++;
  }
  return cursor - str;
}


/**
 * Return 1 if the string given by haystack contains the character
 * given by needle.
 * Return 0 otherwise.
 *
 * Precondition: haystack is a valid pointer to a well-formed string.
 * 
 * Use array indexing.
 * Do not use any other functions.
 */
int contains_char_a(char* haystack, char needle) {
  // Examine all characters of the haystack.
  for (int i = 0; haystack[i] != '\0'; i++) {
    if (haystack[i] == needle) {
      // This character of the haystack matches the needle.
      return 1;
    }
  }
  // No characters of the haystack matched the needle.
  return 0;
}

/**
 * Return 1 if the string given by haystack contains the character
 * given by needle.
 * Return 0 otherwise.
 *
 * Precondition: haystack is a valid pointer to a well-formed string.
 * 
 * Do not use array indexing.  [] is banned.  Use pointer arithmetic instead.
 * Do not use any other functions.
 */
int contains_char_p(char* haystack, char needle) {
  // Walk a cursor pointer over each character of the haystack.
  for (char* cursor = haystack; *cursor != '\0'; cursor++) {
    if (*cursor == needle) {
      // The character at the cursor matches the needle.
      return 1;
    }
  }
  // The cursor has reached the end of the haystack without finding a
  // match for the needle.
  return 0;
}


/**
 * Return a pointer to a newly allocated string holding the characters
 * in haystack starting with the character at index start and ending
 * just before the character at index end.
 *
 * Preconditions:
 * - haystack is a valid pointer to a well-formed string.
 * - To begin, assume start and end respect the bounds of haystack:
 *    - start > 0
 *    - start < (length of string) - 1
 *    - end < (length of string) - 1
 *    - end - start >= 0
 * 
 * Do not use any other functions besides malloc.
 */
char* substring(char* haystack, int start, int end) {
  // The number of characters to be copied from haystack.
  int substring_length = end - start;
  // Allocate space for the result string, including space all
  // characters to be copied from haystack, plus a null terminator.
  char* result = (char*) malloc((substring_length + 1) * sizeof(char));
  if (result == NULL) {
    // There was an error in malloc. Let's just give up.
    perror("malloc");
    return NULL;
  }
  // Copy the substring characters from haystack to the result allocation.
  for (int i = 0; i < substring_length; i++) {
    result[i] = haystack[start + i];
  }
  // Fill in the null terminator.
  result[substring_length] = '\0';
  // The result string is complete.
  return result;
}


/**
 * Return 1 if the string given by haystack contains the string given
 * by needle as a substring.
 * Return 0 otherwise.
 *
 * Precondition: haystack and needle are both valid pointers to
 * well-formed strings.
 * 
 * There are a few ways to implement this, some more efficient than
 * others.  Start with a simple approach.
 */
int contains_string(char* haystack, char* needle) {
  // Enumerate all candidate places where a match of needle could
  // start within haystack.
  for (char* candidate = haystack; 1; candidate++) {
    // Compare the characters of this candidate to the characters of
    // the needle.
    for (int i = 0; 1; i++) {
      // Does the needle terminate at this position?
      if (needle[i] == '\0') {
        // Yes, thus the current candidate matches the needle.
        return 1;
      }
      // No, the needle has a character at this position.
      // Does this candidate terminate at this position?
      if (candidate[i] == '\0') {
        // Yes, thus this and all subsequent candidates are too short
        // to match the needle.
        return 0;
      }
      // No, both the needle and the candidate have characters this
      // position.
      // Do this candidate and needle characters have a mismatch in
      // this position?
      if (candidate[i] != needle[i]) {
        // Yes, thus this candidate and the needle have a mismatch, but
        // later candidates could match the needle.
        break;
      }
      // No, thus this candidate and the needle could match. Continue
      // checking at the next character.
    }
  }
  return 0;
}

/** TESTING SUPPORT **********************************************************/

#define N 16
static char* test_strings[N] = {
  "act",
  "compaction",
  "actual",
  "face",
  "face the action",
  "face the faction",
  "factual",
  "facet",
  "facetious",
  "face facet",
  "face facts facetiously",
  "effacing",
  "efface",
  "aaabb",
  "aabb",
  ""
};

/** Test an implementation of the string_length function. */
int test_string_length(char* fname, int (*fp)(char*)) {
  printf("Testing %s\n", fname);
  int pass = 0;
  int total = 0;
  for (int i = 0; i < N; i++) {
    total += 1;
    int len = strlen(test_strings[i]);
    int s = fp(test_strings[i]);
    pass += (s == len);
    if (pass != total) {
      printf("[%s]  %s(\"%s\") = %d; expected %d\n",
             (s == len ? "PASS" : "FAIL"),
             fname, test_strings[i], s, len);
      break;
    }
  }
  printf("Of %4d tests of %s\n", total, fname);
  printf("   %4d PASS\n", pass);
  printf("   %4d FAIL\n\n", total - pass);
  return total - pass;
}

/** Test an implementation of the contains_char function. */
int test_contains_char(char* fname, int (*fp)(char*, char)) {
  printf("Testing %s\n", fname);
  int pass = 0;
  int total = 0;
  for (int i = 0; i < N; i++) {
    char needle[2];
    needle[1] = '\0';
    for (char c = 'a'; c <= 'z'; c++) {
      total += 1;
      needle[0] = c;
      int contains = strstr(test_strings[i], needle) != NULL;
      int s = fp(test_strings[i], c);
      pass += (s == contains);
      if (pass != total) {
        printf("[%s]  %s(\"%s\", '%c') = %d; expected %d\n",
               (s == contains ? "PASS" : "FAIL"),
               fname, test_strings[i], c, s, contains);
        break;
      }
    }
    if (pass != total) break;
  }
  printf("Of %4d tests of %s\n", total, fname);
  printf("   %4d PASS\n", pass);
  printf("   %4d FAIL\n\n", total - pass);
  return total - pass;
}

/** Test an implementation of the contains_string function. */
int test_contains_string(char* fname, int (*fp)(char*, char*)) {
  printf("Testing %s\n", fname);
  int pass = 0;
  int total = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      total += 1;
      int contains = strstr(test_strings[i], test_strings[j]) != NULL;
      int s = fp(test_strings[i], test_strings[j]);
      pass += (s == contains);
      if (pass != total) {
        printf("[%s]  %s(\"%s\", \"%s\") = %d; expected %d\n",
               (s == contains ? "PASS" : "FAIL"),
               fname, test_strings[i], test_strings[j], s, contains);
        break;
      }
    }
    if (pass != total) break;
  }
  printf("Of %4d tests of %s\n", total, fname);
  printf("   %4d PASS\n", pass);
  printf("   %4d FAIL\n\n", total - pass);
  return total - pass;
}

/** Test an implementation of the substring function. */
int test_substring() {
  printf("Testing %s\n", "substring");
  int pass = 0;
  int total = 0;
  char* str = "compaction";
  int len = strlen(str);
  for (int start = 0; start < len; start++) {
    for (int end = start; end <= len; end++) {
      total += 1;
      char* result = substring(str, start, end);
      if (!result) {
        printf("[FAIL]  substring(\"%s\", %2d, %2d) = NULL; expected a string\n",
               str, start, end);
        break;
      } else if (result[end - start] != '\0') {
        printf("[FAIL]  substring(\"%s\", %2d, %2d) result is missing '\\0' terminator in expected position\n",
               str, start, end);
        free(result);
        break;
      } else {
        int i;
        for (i = 0; i < end - start; i++) {
          if (result[i] != str[start + i]) {
            printf("[FAIL]  substring(\"%s\", %2d, %2d) = %s  [FAIL] mismatch at index %d of result\n",
                   str, start, end, result, i);
            free(result);
            break;
          }
        }
        if (i == end - start) {
          printf("[PASS]  substring(\"%s\", %2d, %2d) = \"%s\"; expected \"%s\"\n",
                 str, start, end, result, result);
          free(result);
          pass += 1;
        } else {
          break;
        }
      }
    }
    if (pass != total) break;
  }
  printf("Of %4d tests of %s\n", total, "substring");
  printf("   %4d PASS\n", pass);
  printf("   %4d FAIL\n\n", total - pass);
  return total - pass;
}

/**
 * Testing driver: test all practice functions.
 */
int main(int argc, char** argv) {
  int fail = 0;
  // Test both implementations of string_length.
  fail += test_string_length("string_length_a", string_length_a);
  fail += test_string_length("string_length_p", string_length_p);

  // Test both implementations of contains_char.
  fail += test_contains_char("contains_char_a", contains_char_a);
  fail += test_contains_char("contains_char_p", contains_char_p);

  // Test substring.
  fail += test_substring();

  // Test contains_string.
  fail += test_contains_string("contains_string", contains_string);

  return fail;
}
