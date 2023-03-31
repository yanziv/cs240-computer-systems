/* 
 * CS 240 Bits
 * 
 * bits.c - Source file with your solutions.
 *
 * Do not add #include lines in this file.
 */

#if 0
CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int puzzle(arg1, arg2, ...) {
      /* Brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      // Inline comments explain specific steps if needed.
      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer literals 0 through 255 (0xFF), inclusive, using only decimal
     or hexadecimal notation.
       - Binary notation (0b11111111) is not accepted by the checker tools.
       - You are not allowed to use large literals such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. The list of "Legal ops" given in the function comment.
  
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  Your solution to a puzzle should use no more than the number of "Max ops"
  listed for the fuction.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:, not listed in the
     the "Legal ops" list for each function.
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.
 
  Assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts (>>) arithmetically on ints.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   * Legal ops: ! ~ & ^ | + << >>
   */
  int pow2plus1(int x) {
     /* Shifting left by n bits multiplies by 2^n. */
     return (1 << x) + 1;
  }
  int pow2plus1(int x) {
     /* Exploit ability of shifts to compute powers of 2. */
     int result = (1 << x);
     result = result + 1;
     return result;
  }
#endif


/* 
 * bitAnd: x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
/* According to DeMorgan's law, ~(AB) = ~A + ~B. In bitAnd, we need to find x&y, which is, AB. By modifying 
 * ~(AB) = ~A + ~B to get AB, we need to negate both sides since ~(~(AB)) = AB. Hence, we get ~(~(AB)) = AB = ~(~A + ~B).
 * We built our return statement following this logic. 
 * Trying the sample case, we have 6 = 0b0110 and 5 = 0b0101. By negating both of them, we have 0b1001 and 0b1010.
 * OR them, we have 0b1011. And finally negating 0b1011, we will have 0b0100 which is 4. This proves that our logic and code
 * works correctly.
 */
int bitAnd(int x, int y) {
  return ~(~x|~y); //NOT of NOT x OR NOT y, which is the same as x AND y
}

/* [INDEPENDENT PROBLEM]
 * bitXor: x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 * 
 * According to the truth table of the XOR gate, the XOR gate can be expressed as A'B + AB' in the sum of products. 
 * By DeMorgan's rule, (A'B + AB')' = (A'B)' & (AB')' = ~(~A & B) & ~(A & ~B), because we negated the entire
 * A'B + AB' to use the DeMorgan's rule, we now need to negate it again to cancel out the effect, so the final
 * answer is ~(~(~A & B) & ~(A & ~B)), and if we replace the inputs A and B by x and y, we will get 
 * ~(~(~x & y) & ~(x & ~y)).
 * Trying the sample case, we have 4 = 0100 and 5 = 0101, the negated binary representation of 4 is 1011 and the 
 * negated binary representation of 5 is 1010, we ampersand 1011 and 1010 and get 1010, we negate 1010 and get 0101.
 * 0100 & 0101 is 0100, and we negate 0100 and get 1011, lastly, we ampersand 0101 and 1011, we will get 0001, which is 1
 * in hex, which is the correct output.
 */
int bitXor(int x, int y) {
  return ~(~(~x&y) & ~(x&~y));
}

/* 
 * thirdBits: return the int value where every third bit (including
 *            the LSB) is 1 and all other bits are 0.  This function
 *            takes no arguments and always returns the same result.
 *   Example: thirdBits() = 0b01001001001001001001001001001001
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int thirdBits() {
/*
 * Generally, our approach to this question was to create the needed value through shifting and
 * arithmetic addition, starting with an allowed two-digits hex.
 * 0b01001001001001001001001001001001, the int value we want, is essentially 0x49249249.
 * Our idea was to use shifting to complete this pattern. We start with the 
 * first two hex digits, which are 49, and store 0x49 into the integer variable a,
 * then we shift a to the left by 8 positions, which creates 8 0's following 0b01001001. We
 * then add 0x24 to it, which makes it 0b0100100100100100 (0x4924). Similarly, we shift it
 * to the left again by 8 positions and add 0x92 to it, which makes it 0b010010010010010010010010 (0x492492),
 * eventually we shift it left again by 8 and add 0x49, so it outputs 
 * 0b01001001001001001001001001001001 (0x49249249).
 */
  int a = 0x49; // start with 0b01001001
  int b = (a << 8) + 0x24;
  int c = (b << 8) + 0x92;
  int d = (c << 8) + a;
  return d;
}

/* 
 * fitsBits: return 1 if x can be represented as an n-bit, two's
 *           complement integer, where 1 <= n <= 32.
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 *   Hint: think about sign extension
 */
int fitsBits(int x, int n) {
  /* when the sign bit is the same after the shift, it can work*/
  /* shift to the left end according to n 
   * this is 32-n according to lecture 
   */
  int a = 32+ ~n +1;
  /* get rid of the digits we dont wanna keep */
  int b = (x<<a);
  /* shift back to match x digits, arithmetic shift filled front with the second bit which now becomes sign bit 
   * sign extension
   */
  int c = (b>>a);
  /*compare with x by minusing
   * return 1 if x-c = 0 
   */
  int d = !(x + ~c +1);
  return d;
}

/* [INDEPENDENT PROBLEM]
 * sign: return 1 if positive, 0 if zero, -1 if negative
 *   Examples: sign(130) = 1
 *             sign(-23) = -1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 2
 *   Hint: compute separate answers and combine
 */
/*
 * Analyzing what sign wants us to do, we can say that there will be three steps to this. First we
 * need to check the sign if x. We do this by x>>31, leaving only the sign bit filling the 32 bits: 0000... for x>=0,
 * 1111... (-1) for x<0
 * Then, we should check whether x is 0 or not using !!(x). If !!(x) =0, x=0; If !!(x)=1, x is not 0.
 * Now, we & the sign bit and the result from checking whether it is 0 or not together. If x is 0,
 * which results in a=0 and b=0 (a and b from the below code), will return 0|0 =1. If x>0, a=0 and b=1, a|b = 1;
 * If x<0, a=-1 and b=1, a|b = -1. This satisfies sign.
 */
int sign(int x) {
  /* check >=0
   * a = 0 if x>=0; a=1111 (-1) if x<0
   */
  int a = x>>31; //only sign bit left
  /* check if x is 0
   * if x = 0, b = 0
   * if x != 0, b = 1
   */
  int b = !!(x);
  /* when x is not zero, we want to just return the sign bits, so OR them with 1, 
   * return 0|1 = 1 (positive) or 11111(-1)|1 = -1 (negative)
   *
   * when x is zero, we want to return 0 = 0|0
   * basically return 0 when both sign and checkZero is 0
   */
  return (a | b);
}

/* 
 * getByte: extract byte n from int x,
 *          where bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
/* 
 * Our general idea for getBytle is to leave the desired byte n through left and right shifting, then
 * masking out the undesired bits.
 * Considering LSB to MSB 0 to 3, we can see that to get byte 0 at LSB, we need to x>>0; to get
 * byte 1 in LSB, we need to x>>8, byte 2 x>>16, byte 3 x>>24
 * Thus, we first need to determine how many positions to shift right according to n. When n=0, we want 0 to be 0; 
 * when n=1, we want 1 to be 8; when n=2, we want 2 to be 16; when n=3, we want 3 to be 24. Following this
 * pattern, we can see that 8 is 1<<3, 16 is 2<<3, 24 is 3<<3, and 0 could be 0<<3. Thus we can tell that the
 * positions x should shift right is n<<3.
 * After getting n<<3, we shift x to the right by n>>3. We now have the desired byte in LSB, but all the previous
 * positions will be filled with MSB. They need to be masked.
 * By ANDing x>>(n<<3) with 0xff (11111111), we now have only the last 2 bytes left, which is the desired byte.
 * Because anything & 11111111 will be the last 8 bits of that anything. 
 */
int getByte(int x, int n) {
  /* how many spaces to shift by: 0,8,16,24*/
  int a = n<<3;
  /*shift right to get desired byte in LSB*/
  int b = x>>a;
  /*masking out all the previous, leaving only the last 8 bits*/
  int c = b & 0xff;
  return c;
}

/* 
 * logicalShift: logical shift x to the right by n,
 *               where 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 *   Hint: Shifting a 32-bit value by 32 is undefined.
 *         Think about splitting a potentially undefined shift into
 *         smaller defined shifts.
 */
/*
 * Logicalshift should perform a shift to the right by n positions which fills the front with n 0's.
 * Analyzing the sample case, we can see that x is given in hex and n is given in decial number. And after the logical shift 
 * 0x87654321 to the right by 4, we get 0x08765432. Therefore we know that 4 is the number of binary bits that were 
 * shifted, which resulted in a shift of 1 hex digit.
 * Our general idea for logicalshift is that we first arthmetically shift x to the right by n, resulting in the 
 * front being filled with n of MSB. Then we should mask off the first n digits to 0 and leave us with the last 32-n,
 * which looks like a logical shift.
 * To design this mask, we can shart with a 1 and <<31, making it 10000000000000000000000000000000, 32 bits ready to make the mask.
 * because we want to mask the first n bits, we would want the first n positions of the mask to be 0,
 * so that we can AND it with the shifted x for masking. So, we shift 10000000000000000000000000000000 n positions to the right, now we
 * get n 1's in front of 1000000.., which makes n+1 1's infrot of 32-n-1 0's. Now we just need to <<1 to make the front exactly n 1's.
 * To complete the mask, we ~ it to turn the n 1's to n 0's (for the digits we want to mask off), and the rest 32-n 0's 
 * to 32-n 1's (for the digits we want to keep).
 * At last, we use AND to & together the arithmetic shift of x with the mask, leaving a logical right shift form of x>>n.
 * In the end, the first n positions will be n and the last 32-n positions will keep what they already have since they are
 * & with 1. 1&x depends on x and 0&x will sure be 0.
 */
int logicalShift(int x, int n) {
  /* shift x by n positions*/
  int a = x >> n;
  int b = ~(((1<<31)>>n)<<1); //how the mask was designed is explained above
  int c = a & b; //masking a with b
  return c;
}

/* 
 * addOK: determine if x+y succeeds without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 * 
 * bitwise addition will result in an overflow only when the sign 
 * bit of the result is different from those of the input bits.
 * The program returns 1 when there isn't an overflow and 0 otherwise.
 * Because we only care about the sign bits of the two inputs, we want
 * to perform shifting on x. So we first shift x to right by 31 positions, 
 * which will make all 32 bits the same as its sign bit, we then shift it to
 * the left by 31 positions, which will fill the last 31 bits with 0 and only
 * keep the sign bit. Similarly, we perform the same operations on the
 * second input y. To know whether the two inputs have the same sign bit, we
 * can use the XOR operation, which only returns true when two inputs are 
 * different, so we double negate the result of XOR to determine whether they
 * have the same sign bit. We then perform bitwise addition on x and y to get 
 * the result, and we shift it to the right by 31 positions and shift it to the
 * left by 31 positions to get its sign bit. Lastly, our goal is to compare the
 * sign bit of the result to the sign bit of the two inputs, and we can do so by
 * using XOR and do a single negation to compare the sign bit to that of the 
 * two inputs respectively. If the sign bits of two inputs are different, this 
 * will not result in an overflow, and if the sign bits of the two inputs are 
 * the same, and the sign bit of the result is different from the inputs' sign bit, 
 * then this is considered an overflow.
 */
int addOK(int x, int y) {
  //to overflow: same sign arguments and different sign result
  /* same same = 1
   * diff same = 1
   * diff diff = 1
   * same diff = 0
   * use OR
   */
  //sign bit of x
  int a = ((x>>31)<<31);
  //sign bit of y
  int b = ((y>>31)<<31);
  // if a and b are the same, c = 0
  int c = !!(a^b);
  //add x and y
  int d = x+y;
  //sign bit of d
  int e = ((d>>31)<<31);
  //if either e and a or e and b have the same sign, return 1 ==> no overflow bc no diff sign result
  int f = ! (e^a) | ! (e^b);
  //return 0 (overflow) when a b are the same and e has different sign from both a and b
  return c | f;
}

/* [INDEPENDENT PROBLEM]
 * bang: compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 *   Hint: 0 is the only int x for which both x >= 0 and -x >= 0
 */
/*
 * Analyzing what !x does, it is a logical not that returns 0 when x is not 0, and returns 1 when x is 0.
 * Our general idea is that to compute !x without using !, we could determine whether x is 0. 
 * If x=0, bang(x) should return 1; If it is not 0, bang(x)=0.
 * To determine whether x is 0, we can use the hint that x = 0 = -x. We can compare whether x and -x (its two's complement) are
 * the same. And to determine whether they are the same, we will use bitwise OR because as long as x and -x
 * aren't the same, x|-x will result in 111111111 because they are two's complements of each other. If they are the same, 
 * x|-x will result in 0 and we now know that x is 0. To have the result be presented as a binary bit, we will right shift
 * them 31 places from MSB to LSB.
 * Now knowing whether x is 0 or not, with x=0 resulting in a 0b0 (0) and x!=0 resulting in a 0b1 (-1), we then have to do the equivalant
 * of ! to 0 and -1. !0 = 1 and !-1 = 0. We can see that 0+1 =1 and -1+1 = 0 fits what we want to achieve. Therefore
 * the last step will be to arithmetically + it by 1 to fully perform bang(x) without !.
 * Walking through bang(3) = 0, x= 3 = 0011 and -x= ~x+1=1101. 0011 | 1101 = 1111, proving that it is not 0. 1111 >> 31 moves
 * the MSB 1 to the LSB for the next operation. 0b1 + 1 = 0 with an 1 as a carry out. 0 = 0, proving our code works. 
 * Walking through bang(0) = 1, x = 0 = 0000 and -x= ~x+1 = 1111+1 = 0000. 0000|0000 = 0000, proving that x=0. 0000>>31 moves 
 * the MSB 0 to LSB for the next operation. 0b0 + 1 = 1. 1=1, proving our code works.
 */
int bang(int x) {
  int a = ~x+1; // find twos complement (-x) of x, we will negate x and +1 to it.
  int b = (x|a) >>31; //x|-x tells us whether x is 0 or not. If x=0, b= 0b0; if x != 0, b=0b1
  int c = b+1; //Perform arithmetic addition to 0b0 or 0b1, resulting in 1 or 0.
  return c;
}


/* 
 * conditional: same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 * 
 * Following the logic that if x is 1 then return y, else return z.
 * We would only return z when x is 0, so we need to create an integer
 * variable which determines whether x is 0. 
 * If a is 0, and we ampersand it with y, a&y will be 0, ~a will be 1 
 * and ~a&z will be z; if a is 1 and we ampersand it with y, a&y will
 * be y, ~a will be 0 and ~a&z will be 0, thus this will return the 
 * desired output. We create an integer variable a that stores the
 * value of input x and we OR inverted x, we then shift it by 31 positions
 * so that we can move the value from the most significant digit to the 
 * least significant digit, then we can tell if the input x is 0 or not. 
 * Once our variable a is done, it will automatically return the correct
 * output depending on the value of a.
 */

int conditional(int x, int y, int z) {
  int a = (x|(~x+1)) >> 31;
  return ((a&y)+(~a&z));
}



/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 *
 * SAMPLE DESCRIPTION AND SOLUTION:
 * This description is on the detailed side. Explains the properties
 * extensively and includes quick connections with the operators.
 * 
 * Use the property that, if x is the nth power of two, x has exactly
 * one 1 bit, followed by n zeros.  For example, 8 = 0b0...01000. The
 * predecessor of the nth power of 2 has exactly n ones, all in the n
 * least significant bits; all other bits are 0.  For example, 7 =
 * 0b0...00111. Any other pair of x and x-1 has at least one 1 bit in
 * common. Example: 6 (0b0...0110) and 5 (0b0...0101).  There are
 * actually two other values that are *not* powers of two that also
 * satisfy the property "x and x-1 share no 1 bits." Zero (=0...0 vs
 * -1 = 1...1) and the minimum twos complement value (=10...0 vs the
 * maximum twos complement value 01...1). In fact, no negative is a
 * power of, so we filter out all negatives, capturing the twos
 * complement minimum.
 */
int isPower2(int x) {
  return !(x & (x + ~0)) // x and its predecessor share no 1 bits
    & !!x               // AND x is nonzero
    & !(x >> 31);       // AND x is non-negative
}



#if 0
/* 
 * SAMPLE ALTERNATIVE DESCRIPTION:
 * This description reorganizes the ideas more incrementally. It
 * explains more about the connection to operators and mechanics,
 * making it a more comprehensive description.
 */
int isPower2_alt1(int x) {
  // A power of 2 has exactly one 1 bit among all its bits and shares
  // no 1 bits in common with the next smallest value (the predecessor).
  // This expression yields the value 1 if this property holds and 0
  // otherwise.
  int shares_no_ones_with_pred = !(x & (x + ~0));
  // But two other values share this property with powers of 2:
  //   A. zero also satisfies this property, so we must check for it.
  //      !! transforms 0 to 0 and transforms any nonzero number to 1.
  int nonzero = !!x;
  //   B. The most negative number, the twos complement minimum, also
  //      satisfies the same 1-sharing property. Since no power of 1
  //      is negative, isolate the sign bit and use Boolean NOT to
  //      yield 1 if x is non-negative.
  int nonnegative = !(x >> 31);
  // Overall, x is a power of two if and only if it shares no ones
  // with its predecessor and it is nonzero and it is non-negative.
  return shares_no_ones_with_pred & nonzero & nonnegative;
}

/* 
 * ALTERNATIVE SHORTER SOLUTION:
 * This version is reorganized vs alt1 to use fewer operators. It
 * still uses the same insights.
 */
int isPower2_alt2(int x) {
  // A power of 2 has exactly one 1 bit among all its bits and shares
  // no 1 bits in common with the next smallest value (the predecessor).
  // This expression yields the value 0 if this property holds and nonzero
  // otherwise.
  int shares_ones_with_pred = x & (x + ~0);
  // But two other values share this property with powers of 2:
  //   A. zero also satisfies this property, so we must check for it.
  //      ! transforms 0 to 1 and transforms any nonzero number to 0.
  int iszero = !x;
  //   B. The most negative number, the twos complement minimum, also
  //      satisfies the same 1-sharing property. Since no power of 1
  //      is negative, duplicate the sign bit to produce -1 or all
  //      negative x and 0 for all non-negative x.
  int negative = x >> 31;
  // Overall, x is a power of two if and only if it shares no ones
  // with its predecessor and it is nonzero and it is non-negative.
  // If any one of these three values carries any 1 bit, x is not a
  // power of 2.
  return !( shares_ones_with_pred | iszero | negative);
}
#endif
