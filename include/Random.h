#ifndef Random_h
#define Random_h

#include <chrono>

#include <TRandom3.h>

class Random{

public:
    static Random& Get();
    ~Random();

    static double Uniform(const double min, const double max){ 
        return Get().UniformImpl(min, max); 
    }
    static double Gauss(const double mean, const double sigma){ 
        return Get().GaussImpl(mean, sigma); 
    }

    // delete copy constructor and assignment operator to avoid copies
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;

private:
    Random();

    double UniformImpl(const double min, const double max);
    double GaussImpl(const double mean, const double sigma);

    TRandom3 fRnd;
};

#endif