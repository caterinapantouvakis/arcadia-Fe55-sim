#ifndef Random_h
#define Random_h

#include <chrono>

#include <TRandom3.h>

class Random{

public:
    static Random* GetGenerator();
    ~Random();

    double GenerateUniform(const double min, const double max);
    double GenerateGauss(const double mean, const double sigma);


private:
    Random();
    
    TRandom3 rnd;

    static Random* generator;

};


#endif