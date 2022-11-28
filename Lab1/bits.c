/* 
 * CS:APP Data Lab 
 * 
 * Jan Piroutek jpir
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return ~(~(x&~y) & ~ (~x&y));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 0x80 << 0x18;
}
//2
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  int odd_bits = 0xAA + (0xAA << 0x08) + (0xAA << 0x10) + (0xAA << 0x18);
  return !((odd_bits & x) ^ odd_bits);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1;
}
//3
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  int num = !!x;
  int con = ~(num) + 1;
  return (y & con) | (z & ~con);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  // diff bits
  int xor = x ^ y;
  int min = 0x80 << 0x18;
  int max_pos = ~min;
  int first_diff;
   
  // copy so every bit after first different one is positive
  xor = xor | (xor >> 0x01);
  xor = xor | (xor >> 0x02);
  xor = xor | (xor >> 0x04);
  xor = xor | (xor >> 0x08);
  xor = xor | (xor >> 0x10);

  // keep only the biggest different bit (next reffered to as BDB
  first_diff = xor & (~xor >> 0x01);
  // for case when x is positive and y is negative (more explained below)
  first_diff = first_diff | min;

  // right now I am looking for x > y
  // y ^ max pos -> I want y have BDB 0
  // x ^ min -> I want x have BDB 1
  // when it comes to MSB I want for x to be positive && y to be negative
  // now if x is positive and y is negative I'll have MSB in x ^ min and y ^ 
  // max_pos switched on 1 and first_diff also 1 -> so I'll get positive number for x > y (aka it's true)
  // if they have same positivity/negativity one of those bits will be 0 -> so the MSB of result will also be 0
  //  in this case I want x to have 1 on place of BDB and 0 for y
  //  xoring this with 0 or 1 gives me what I want and if this is true & with first_diff will get me 1 for BDB
  //  on every other place there will be 0
  // 
  first_diff = first_diff & (x ^ min) & (y ^ max_pos);
  
  // result is x > y so just negate the result
  return !first_diff;
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  // copy bits so if there is any 1, the lsb will also be a 1 
  x = x | (x >> 0x01);
  x = x | (x >> 0x02);
  x = x | (x >> 0x04);
  x = x | (x >> 0x08);
  x = x | (x >> 0x10);
  
  // flip the lsb (negation)
  x = x ^ 0x01;
  // remove all other 1 bits
  return x & 0x01;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  int min = 0x80 << 0x18;
  int fives = 0x55 + (0x55 << 0x08) + (0x55 << 0x10) + (0x55 << 0x18);
  int threes = 0x33 + (0x33 << 0x08) + (0x33 << 0x10) + (0x33 << 0x18);
  int of = 0x0F + (0x0F << 0x08) + (0x0F << 0x10) + (0x0F << 0x18);
  int max_num = 0x3F;
  int cond;
  int neg_cond;
  int count;
  
  // if is x negative change it to its positive value
  // otherwise keep original positive value
  // 0x80000000 can stay the same, because it still needs all of bits used
  // izolate first bit
  cond = x & min;
  // from previous task
  neg_cond = !!cond;
  cond = ~(neg_cond) + 1;
  // why ~x and not ~x + 1
  // I know that in every twos complement I need one bit for basically the sign
  // not adding one when negating the number won't change it's size, if I remmember to 
  // add that one bit back to result
  x = ((~x) & cond) | (x & ~cond);
  
  //copy the first 1 bit to the right
  x = x | (x >> 0x01);
  x = x | (x >> 0x02);
  x = x | (x >> 0x04);
  x = x | (x >> 0x08);
  x = x | (x >> 0x10);
  
  
  // count the number of 1s in x
  // this was taken from book Hacket's delight (I believe it's called)
  // Don't know if you can count it as solved but I don't know any other way
  // This is based on divide and conquer
  // each bits knows how many 1's are in it (either 1 or 0)
  // them you compute how many bits set have two next door bits
  // them 4, 8 and so on
  count = x + (~((x >> 0x01) & fives) + 1);
  count = (count & threes) + ((count >> 0x02) & threes);
  count = (count + (count >> 0x04)) & of;
  count = count + (count >> 0x08);
  count = count + (count >> 0x10);
  count = count & max_num;
  // don't forget to add 1
  return count + 1;
}
