#include <stdio.h>
#include <math.h>

#define INVALID_STR_SCORE -target //Penalty for an invalid string
//TODO: implement roulette wheel sampling (stochastic with replacement)
// as well as remainder stochastic sampling
//TODO: Vectorize/ parallelization possible?
//TODO: Make a better implementation that uses parentheses, tokenizer and interpreter. 
  //Also experiment with adding more restrictions for optimization (only even numbers, etc.)
//Does the fitness score need represent how many changes need to occur in the chromosome for
  //it to be closer to the target? It seems like a good fitness score does not necessarily
  //mean an operator expression can reach the target with minor changes.
//What should I do with unused chromosome mappings?

/* Problem: given digits from 0-9 and operators -,+,*,/, make a genetic
 * algorithm that will give an expression necessary to reach a target number.
 *
 * ex. target number is 11, a possible expression is 4 + 5*2 - 3 following
 * the pattern of numbers and operands.
 * 
 * expression will be represented by a 28 bit string, using four bits to 
 * represent each key from the following table.
 *
 * 0 : 0000
 * 1 : 0001
 * 2 : 0010
 * 3 : 0011
 * 4 : 0100
 * 5 : 0101
 * 6 : 0110
 * 7 : 0111
 * 8 : 1000
 * 9 : 1001
 * + : 1010 
 * - : 1011 
 * / : 1100
 * * : 1101
 * codes 1110,1111 are not used and will be disregarded by the algorithm. For 
 * example, the bit string 0011 1010 1110 1011 0100 1101 0101 decodes to 
 * 3 + - 4 * 5, which is an invalid bit string. 
 *
 */

//first generate n 28 bit string chromosomes for the pool

/* These are the operator functions. Standard
 * function definitions are necessary for 
 * using function pointers with the function
 * addresses.
 */

#define add(a,b) (a + b)
#define sub(a,b) (a - b)
#define mul(a,b) (a*b)
#define div(a,b) (a/b)

int add(int a, int b) { add(a,b); }
int sub(int a, int b) { sub(a,b); }
int mul(int a, int b) { mul(a,b); }
int div(int a, int b) { div(a,b); }

int (*addPtr)(int, int) = &add;
int (*subPtr)(int, int) = &sub;
int (*mulPtr)(int, int) = &mul;
int (*divPtr)(int, int) = &div;

int* operator_func_array = { (*addPtr)(int,int), (*subPtr)(int,int), (*subPtr)(int,int), (*divPtr)(int,int) };

int operation(int (*funcPtr)(int,int), int a, int b) {
  return (*funcPtr)(a,b);
}

/* generates an intial pool of random binary strings, flipping a coin for each bit. 
 */
int* generate_binary_strings(int length, int num_strings, int* strings) {
  int i;
  int j;
  int new_string;
  for (i = 0; i < num_strings; i++) {
    new_string = 0; 
    for (j = 0; j < length; j++) {
      int rand = 0_or_1(); //0_or_1 returns 0 or 1 at random every time it is called. 
      new_string = (new_string | rand) << 1;
    }
    strings[i] = new_string;
  }
  return strings;
}


//function to compute fitness score
int calc_fitness(int string, int target) {
 int eval_str = decode_and_eval_bit_strings(string);
 return 1/(abs(decoded_str - target));

}

/* Takes in a 28-bit chromosome, decodes it into 
 * operator and operands, and returns the evaluated
 * expression, or INVALID_STR_SCORE for an invalid
 * operator and operand combination
 */

#define NUM_ELEMS 7
#define OPERAND 0
#define OPERATOR 1
int decode_and_eval_bit_strings(int string) {
  int mask = 0xf;
  int i;
  int prev_type = 1; //expression must start with an operand
  int this_type;
  for (i = 0; i < NUM_ELEMS; i++) {
    int elem = (mask << (NUM_ELEMS - i)*4 & string) >> i*4;
    this_type = elem / 10; //0-9 is digits 
    if (this_type != (prev_type + 1) % 2) //if not a valid string
      return INVALID_STR_SCORE;
    prev_type = this_type;
    
}

