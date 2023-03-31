/* Adventure plan and obstacles */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "support.h"
#include "phases.h"

/* 
 * Note to self: Remember to erase this file so explorers will have no
 * idea what is going on. If everything goes as planned, it's not an
 * adventure!
 */

FILE *infile;

int main(int argc, char *argv[]) {
  char *input;

  // Note to self: remember to port this to iOS and Android and put a
  // gratuitous slick GUI on it.

  if (argc == 1) {  
    // When run with no arguments, the executable reads its input lines 
    // from standard input.
	infile = stdin;
  } else if (argc == 2) {
    // When run with one argument <file>, the executable reads from <file> 
    // until EOF, and then switches to standard input. Thus, as you 
    // solve each riddle phase, you can add its riddle solution string to
    // <file> and avoid having to retype it.
	if (!(infile = fopen(argv[1], "r"))) {
      printf("%s: Error: Couldn't open %s\n", argv[0], argv[1]);
      exit(8);
	}
  } else {
    // You can't run the executable with more than 1 command line argument.
	printf("Usage: %s [<input_file>]\n", argv[0]);
	exit(8);
  }

  // Apply protective obstacles to confound students.
  initialize_obstacles(argv[0]);

  printf("You have just ridden the elevator to the hidden 7th floor\n");
  printf("of the Science Center.  A fantastic adventure awaits!\n");
  printf("\n");

  // Hmm...  Six phases must be more secure than one phase!
  printf("First, you must pass a *stringy* spider's web beyond *compare*, guarding\n");
  printf("a trap door!\n");
  input = read_line();              // Get input
  int in_lab = phase_1(input);      // Catch them in our web!
  phase_disarmed();                 // Uh oh! They figured it out!
  printf("\nYou dissolved the spider web with hand sanitizer and slipped\n");
  printf("through the trap door, only to find...\n");
  if (in_lab) {
    printf("... that the lab activity is complete and you should now change over\n");
    printf("    to work on your own executable for the main assignment.\n\n");
    return 0;
  }

  printf("\n... that you are slipping down a *looping,* winding slide at dizzying speed!\n");
  input = read_line();
  phase_2(input);
  phase_disarmed();
  printf("\nWhoosh! You break free, dropping safely into a quiet corner of Clapp.");
  printf("\nPhase 2 done.\n");

  printf("\nNo one has been here since March 2020. Peering between the shadowy, dusty");
  printf("\nstacks, you find a *switch* and flip it on.");
  printf("\nBut instead of turning on the light, the book*cases* begin to encroach...\n");
  input = read_line();
  phase_3(input);
  phase_disarmed();
  printf("\nWow! You knock over a bookcase filled with C books, revealing a tunnel");
  printf("\nand escape route. Phase 3 complete!\n");

  printf("\nBut the tunnel seems to lead deeper and deeper, past taller and taller");
  printf("\nlibrary stacks. It grows familiar, even *recursive*!\n");
  input = read_line();
  phase_4(input);
  phase_disarmed();
  printf("\nWhew! You found the basement case and a map. After gradually");
  printf("\nreturning to the surface, you pop out onto the shore of Lake Waban.");
  printf("\nPhase 4 accomplished!\n");
    
  printf("\nYou confront a puzzling *array* of signs, each with an unlabeled");
  printf("\n*pointer* leading a different direction, but none match your map.");
  printf("\nOnly one way is safe!\n");
  input = read_line();
  phase_5(input);
  phase_disarmed();
  printf("\nImpressive! You navigate around the lake to reach Lulu in time for dinner.");
  printf("\nPhase 5 is history!\n");

  printf("\nConfronted by a long *list* of menu items, you must *order* the right one");
  printf("\nto complete the final *link*: finding your way back to your room.\n");
  input = read_line();
  phase_6(input);
  phase_disarmed();
  
  // Wow, they got it!  But isn't something... missing?  Perhaps
  // something they overlooked?
  return 0;
}
