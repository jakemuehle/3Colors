// Copywright Sep 2011
// Author: Derek Higgs
// Purpose: Functions to compute mathematical operations.

#include <time.h>
#include <cassert>

#include "my_math.h"

int my_utility::getseed(){
  return (int) time(0);
}

// Gives a random number based on the seed and modulous.
// Uses just some arbitrary function.
// These numbers gives a function that is close to random.
// Performing a chi square test on 1,000 numbers 0-9, the p level was 0.7157.
int my_utility::seededRandom(int seed, int mod){
  return linear_congruence(387,6892,seed,mod);
}

unsigned int my_utility::random(unsigned int mod){

#ifdef DEBUG

  static unsigned int seed = 0;

#else // DEBUG

  static unsigned int seed = seededRandom(getseed(),SEED_MOD);

#endif // DEBUG

  seed = seededRandom(seed,SEED_MOD);
  return seed % mod;//linear_congruence(mod/7,seed/5,seed,mod);
}

/* NOTE: this is commented out because it gets confusing
 * and I do not need it now.

// Get a random number from the range.
// Checks if casting the random number as a int would be a problem.
int my_utility::random(int min, int max){
  assert(min>max);
  unsigned random_unsigned = random(max - min);
  if( random_unsigned > 32767 )
    return (int)(random_unsigned - (unsigned)(-min));
  return (int) random(max - min) + min;
}
*/

double my_utility::random(){
  return ((double) random(SEED_MOD) ) / SEED_MOD;
}

int my_utility::greatestCommonDivisor(int a, int b){
  int remainder;
  while(b != 0){
    remainder = a % b;
    a = b;
    b = remainder;
  }
  return a;
}