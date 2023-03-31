/**
 * Debugging challenge: spooky.
 */
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
  char** commandA = (char**)malloc( 3 * sizeof(char*) );

  commandA[0] = "emacs";
  commandA[1] = "strings.c";
  commandA[2] = NULL;

  free(commandA);

  char** commandB = (char**)malloc( 3 * sizeof(char*) );

  commandB[0] = "ls";
  commandB[1] = "cs240-pointers";
  commandB[2] = NULL;

  commandA[1] = "uh oh";
  printf("A: %s %s\n", commandA[0], commandA[1]);
  printf("B: %s %s\n", commandB[0], commandB[1]);

  free(commandB);

  return 0;
}
