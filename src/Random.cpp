#include "Random.h"

Random::Random(){
    unsigned long seed = std::chrono::system_clock::now().time_since_epoch().count();
    fRnd = TRandom3(seed);
}

Random::~Random(){
}

Random& Random::Get(){
    static Random generator;
    return generator;
}

double Random::UniformImpl(const double min, const double max){
    return fRnd.Uniform(min, max);
}

double Random::GaussImpl(const double mean, const double sigma){
    return fRnd.Gaus(mean, sigma);
}

double Random::ExpImpl(const double tau){
    return fRnd.Exp(tau);
}

double Random::PoissonImpl(const double mean){
    return fRnd.PoissonD(mean);
}

