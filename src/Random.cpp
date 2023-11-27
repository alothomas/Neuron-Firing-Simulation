#include "Random.h"

Random::Random(long int s)
    :seed(s)
{
    if (seed == 0) {
        std::random_device rd;
        seed = rd();
    }
    rng = std::mt19937(seed);
}

int Random::uniform_int(int lower,int upper) {
    std::uniform_int_distribution<> unif(lower, upper);
    return unif(rng); 
}

double Random::uniform_double(double lower, double upper){
    std::uniform_real_distribution<> unif(lower, upper);
    return unif(rng);
}

double Random::normal(double moy, double sd) {
    std::normal_distribution<> norm(moy, sd);
    return norm(rng);
}

int Random::poisson(int lambda){
	std::poisson_distribution<> pd(lambda);
	return pd(rng);
}
    
