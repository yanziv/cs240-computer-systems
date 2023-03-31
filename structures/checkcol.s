/**
 * CS 240 Connect Four: colcheck.s
 * Implement this function in assembly code.
 * Check if the board grid contains four connected pieces of type p
 * in a column, including the piece p just played at [row,col].
 *
 */
/*
long checkcol(piece grid[6][7], long row, long col, piece p) {
  long i;
  for (i = 0; i <= row; i++) {
    if (grid[row - i][col] != p) {
      break;
    }
  }
  return i >= 4;
}
*/
	// Tell the assembler that this is code.
	.text
	// Align to 8-byte aligned address.
	.align 8
	// Export the name "checkcol" for use by code in other files.
	.globl checkcol

// x86 assembly implementation of
// long checkcol(piece grid[6][7], long row, long col, piece p);
// Jean & Ben's solution is about 23 lines, not counting comments.
checkcol:
	// -- registers --
	// grid: 
	//  row: 
	//  col: 
	//    p: 
	//    i: 

	// FIXME: implement checkcol to replace these two lines.
	movq $0, %rax
	retq

