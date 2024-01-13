#ifndef Simulation_h
#define Simulation_h

#include "Pixel.h"
#include <map>

class Simulation{

public:    
    Simulation(){};

    struct Hit{
        float x,y;
        float radius;
        std::map<std::pair<unsigned int,unsigned int>, float> hitPixels;

    };

    struct Point{
        float x,y;
    };

    Hit GenerateHit();
    void ComputeElectrons(Hit& hit); //compute amount of charge deposited and collected (randomize!!) 

private:

};


#endif

