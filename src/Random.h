#ifndef RANDOM_H
#define RANDOM_H

#include <random>

/*! @class Random
 * @brief Class based on standard generators of c++11.
 */
 
class Random {

public :
/*! @name Initialization
 The generator rng is a Mersenne twister *mt19937*.
 Seeded with a *random_device*.
*/
///@{
    Random(long int s=0);
///@}
    
    
/*! @name Distributions
 * These methods can return either a single number of a container with random numbers according to the distribution.
 */
///@{
    int poisson(int _lambda);
    int uniform_int(int lower=0 , int upper=1);
    double uniform_double(double lower=0, double upper=1);
    double normal(double moy=0, double sd=1);
///@}

private :
	std::mt19937 rng;
    long int seed;

};

extern Random *_RNG;
#endif
