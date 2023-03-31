/**
 * CS 240 Connect Four Lab: x86 data structure programming
 *
 * C implementation of a command-line Connect Four game, with one 
 * function missing.  You will implement the checkcol function in
 * x86 assembly code in checkcol.s.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// The board has 6 rows and 7 columns.
#define ROWS 6
#define COLS 7

// Players must connect 4 pieces to win.
#define FOUR 4

// Pieces are represented by unsigned numbers.
typedef unsigned long piece;

// Empty spaces on the board hold "empty" pieces from unused player 0.
#define EMPTY 0

// -- DISPLAY CODE --------------------------------------------------

/**
 * Print a line in the grid.
 */
void line() {
  printf("--");
  for (long j = 0; j < COLS; j++) {
    printf("---");
  }
  printf("\n");
}
/**
 * Print index labels for the columns.
 */
void labels() {
  line();
  printf("  ");
  for (long j = 0; j < COLS; j++) {
    printf("%ld  ", j);
  }
  printf("\n");
  line();
}

/**
 * Print the given board grid.
 */
void show(piece grid[ROWS][COLS]) {
  labels();
  for (long i = ROWS - 1; i >= 0; i--) {
    printf("  ");
    for (long j = 0; j < COLS; j++) {
      piece x = grid[i][j];
      if (x == EMPTY) {
	printf("_  ");
      } else {
	printf("%c  ", (char)x);
      }
    }
    printf("\n");
  }
  labels();
}

// -- BOARD UPDATE CODE ---------------------------------------------

/**
 * Clear the given grid of pieces.
 */
void clear(piece grid[ROWS][COLS]) {
  for (long i = 0; i < ROWS; i++) {
    for (long j = 0; j < COLS; j++) {
      grid[i][j] = EMPTY;
    }
  }
}

/**
 * Check if column col is full.
 */
long full(piece grid[ROWS][COLS], long col) {
  assert(col < COLS);
  return grid[ROWS-1][col];
}

/**
 * Play piece p in column col.
 * Return the row in which the piece was played.
 */
long play(piece grid[ROWS][COLS], long col, piece p) {
  assert(col < COLS);
  assert(!full(grid, col));
  for (long i = ROWS-1; i > 0; i--) {
    if (grid[i-1][col]) {
      grid[i][col] = p;
      return i;
    }
  }
  grid[0][col] = p;
  return 0;
}

// -- BOARD CHECKING CODE -----------------------------------------

/**
 * Check if the board grid contains four connected pieces of type p
 * in a column, including at [row,col].
 */
long checkcolOpt(piece grid[ROWS][COLS], long row, long col, piece p) {
  assert(row < ROWS);
  assert(col < COLS);
  assert(grid[row][col] == p);
  if (row < FOUR - 1) {
    return 0;
  }
  for (long r = row; r >= (row + 1 - FOUR); r--) {
    if (grid[r][col] != p) {
      return 0;
    }
  }
  return 1;
}
/**
 * Check if the board grid contains four connected pieces of type p
 * in a column, including [row,col].
 *
 * Simpler, less efficient algorithm.
 */
long checkcolSimple(piece grid[ROWS][COLS], long row, long col, piece p) {
  long i;
  for (i = 0; i <= row; i++) {
    if (grid[row - i][col] != p) {
      break;
    }
  }
  return i >= FOUR;
}

/**
 * Implement an equivalent of checkcolSimple in checkcol.s.
 */
extern long checkcol(piece grid[ROWS][COLS], long row, long col, piece p);

/**
 * Check if the board grid contains four connected pieces of type p
 * in a row, including [row,col].
 */
long checkrow(piece grid[ROWS][COLS], long row, long col, piece p) {
  assert(row < ROWS);
  assert(col < COLS);
  assert(grid[row][col] == p);
  long first = (col < FOUR) ? 0 : col + 1 - FOUR;
  long last = (col + FOUR - 1 < COLS) ? col + FOUR - 1 : COLS - 1;
  long count = 0;
  for (long c = first; c <= last; c++) {
    if (grid[row][c] == p) {
      count++;
      if (count == FOUR) {
	return 1;
      }
    } else {
      count = 0;
    }
  }
  return 0;
}


/**
 * Check if the board grid contains four connected pieces of type p
 * in a southwest to northeast diagonal, including [row,col].
 */
long checkswne(piece grid[ROWS][COLS], long row, long col, piece p) {
  assert(row < ROWS);
  assert(col < COLS);
  assert(grid[row][col] == p);

  long swRow, swCol;
  if (row < col) {
    if (row < FOUR) {
      swRow = 0;
      swCol = col - (row - swRow);
    } else {
      swRow = row - FOUR;
      swCol = col - FOUR;
    }
  } else {
    if (col < FOUR) {
      swCol = 0;
      swRow = row - (col - swCol);
    } else {
      swCol = col - FOUR;
      swRow = row - FOUR;
    }
  }
  long count = 0;
  for (long i = 0; swRow + i < ROWS && swCol + i < COLS; i++) {
    if (grid[swRow + i][swCol + i] == p) {
      count++;
      if (count == FOUR) {
	return 1;
      }
    } else {
      count = 0;
    }
  }
  return 0;
}

/**
 * Check if the board grid contains four connected pieces of type p
 * in a northwest to southeast diagonal, including [row,col].
 */
long checknwse(piece grid[ROWS][COLS], long row, long col, piece p) {
  assert(row < ROWS);
  assert(col < COLS);
  assert(grid[row][col] == p);

  long nwRow, nwCol;
  if (ROWS - 1 - row < col) {
    if (ROWS - 1 - row < FOUR) {
      nwRow = ROWS - 1;
      nwCol = col - (nwRow - row);
    } else {
      nwRow = row + FOUR;
      nwCol = col - FOUR;
    }
  } else {
    if (col < FOUR) {
      nwCol = 0;
      nwRow = row + (col - nwCol);
    } else {
      nwCol = col - FOUR;
      nwRow = row + FOUR;
    }
  }
  long count = 0;
  for (long i = 0; nwRow - i >= 0 && nwCol + i < COLS; i++) {
    if (grid[nwRow - i][nwCol + i] == p) {
      count++;
      if (count == FOUR) {
	return 1;
      }
    } else {
      count = 0;
    }
  }
  return 0;
}

/**
 * Check for four connected pieces completed by play at [row,col].
 */
long checkfour(piece grid[ROWS][COLS], long row, long col) {
  assert(row < ROWS);
  assert(col < COLS);
  piece p = grid[row][col];
  assert(p != EMPTY);
  return checkcol(grid, row, col, p)
    || checkrow(grid, row, col, p)
    || checkswne(grid, row, col, p)
    || checknwse(grid, row, col, p);
}

// -- GAME PLAY CODE ---------------------------------------------

/**
 * Interactively take one turn for player p.
 */
long turn(piece grid[ROWS][COLS], piece p) {
  show(grid);
  long col = COLS;
  while (1) {
    printf("Player %c drop in column: ", (char)p);
    fflush(stdout);
    int scanned = scanf(" %ld", &col);
    if (scanned != 1 || col < 0 || COLS <= col) {
      printf("Invalid column: %ld\n", col);
    } else if (full(grid, col)) {
      printf("Column is full: %ld\n", col);
    } else {
      break;
    }
  }
  return checkfour(grid, play(grid, col, p), col);
}

/**
 * Proclaim victory for player p.
 */
void win(piece grid[ROWS][COLS], piece p) {
  show(grid);
  printf("Connect four!  Player %c wins.\n", (char)p);
  printf("Game over.\n");
}

/**
 * Interactively set up and run a game with two players.
 */
int main(int argc, char** args) {
  
  piece grid[ROWS][COLS];
  clear(grid);

  long p1 = 0; // = 'A'; //0;
  printf("First player's symbol: ");
  fflush(stdout);
  while (1) {
    int scanned = scanf(" %c", (char*)&p1);
    if (scanned != 1) printf("Invalid symbol entry.\n");
    else break;
  }
  
  long p2 = 0; // = 'B'; //0;
  while (1) {
    printf("Second player's symbol: ");
    fflush(stdout);
    int scanned = scanf(" %c", (char*)&p2);
    if (scanned != 1) printf("Invalid symbol entry.\n");
    else if (p1 == p2) printf("That piece is taken! Please enter another.\n");
    else break;
  }

  long moves = 0;
  while (1){
    if(ROWS*COLS <= moves) break;
    ++moves;
    if (turn(grid, p1)) {
      win(grid, p1);
      return 0;
    }
    if(ROWS*COLS <= moves) break;
    ++moves;
    if (turn(grid, p2)) {
      win(grid, p2);
      return 0;
    }
  }
  show(grid);
  printf("Draw!\nGame over.\n");
  return 0;
}
