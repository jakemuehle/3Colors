// Copywright Sep 2011
// Author: Derek Higgs
// Purpose: Functions to compute mathematical operations.

#ifndef __MY_UTILITY__MY_MATH_H
#define __MY_UTILITY__MY_MATH_H

namespace my_utility
{
    // Calculates ( a * x + b ) mod m, linear congruence.
    inline int linear_congruence(int a, int b, int x, int m){
        return ( a * x + b ) % m;
    }
    
    // The modulous of the seed, 2^16-3.
    const unsigned SEED_MOD = 65533;
    
    // Function for obtaining a random seed.
    int getseed();
    
    // Give a sudo random number based on the seed in the modululus.
    int seededRandom(int seed, int mod);
    
    // Get a random integer in the modululus.
    unsigned int random(unsigned int mod);
    
    
    /* NOTE: this is commented out because it gets confusing
     * and I do not need it now.
     
     // Get a random integer in the range, includes the min, excludes the max.
     ENGINE_SHARED int random(int min, int max);
     */
    
    // Get a random double
    double random();
    
    // Calculate the greatest common divider/factor of two integers
    int greatestCommonDivisor(int a, int b);
    inline int gcd(int a, int b) { return greatestCommonDivisor(a,b); }
    inline int gcf(int a, int b) { return greatestCommonDivisor(a,b); }
    inline int greatestCommonFactor(int a, int b) {
        return greatestCommonDivisor(a,b);
    }
    
}

#endif // __MY_UTILITY__MY_MATH_H


