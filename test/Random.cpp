#include "Random.h"

Random* Random::generator = nullptr;

Random::Random(){
    unsigned long seed = std::chrono::system_clock::now().time_since_epoch().count();
    rnd = TRandom3(seed);
}

Random::~Random(){}

Random* Random::GetGenerator(){
    if(generator == nullptr){
        generator = new Random;
        return generator;
    }

    else return generator;
}


double Random::GenerateUniform(const double min, const double max){
    return rnd.Uniform(min, max);
}

double Random::GenerateGauss(const double mean, const double sigma){
    return rnd.Gaus(mean, sigma);
}

