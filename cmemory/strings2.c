/**
 * Debugging challenge: crasher.
 */
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
  char** commandA = (char**)malloc( 4 * sizeof(char*) );

  commandA[0] = "emacs";
  commandA[1] = "strings.c";
  commandA[2] = "search.c";
  commandA[3] = NULL;

  printf("%s %s %s\n", commandA[0], commandA[1], commandA[2]);

  free(commandA);

  char** commandB = (char**)malloc( 3 * sizeof(char*) );

  commandB[0] = "ls";
  commandB[1] = "cs240-pointers";
  commandB[2] = NULL;
  
  printf("%s %s\n", commandB[0], commandB[1]);

  free(commandA);

  return 0;
}
