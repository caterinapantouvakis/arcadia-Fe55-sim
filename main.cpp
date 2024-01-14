#include <iostream>

#include "Matrix.h"
#include "Random.h"
#include "CommandLine.h"
#include "Simulation.h"

int main(int argc, char** argv){

    // create CommandLine and retrieve argv inputs
    typedef CommandLine cl;
    cl::Init(argc, argv);

    int size = (int)cl::GetValue("size");
    Matrix matrix(size,size);

    float meanBaseline =    cl::Contains("meanBaseline")  ? cl::GetValue("meanBaseline") : 115;
    float meanNoise =       cl::Contains("meanNoise")     ? cl::GetValue("meanNoise")    : 0;
    float meanGain =        cl::Contains("meanGain")      ? cl::GetValue("meanGain")     : 10;
    float sigmaBaseline =   cl::Contains("sigmaBaseline") ? cl::GetValue("sigmaBaseline"): 5;
    float sigmaNoise =      cl::Contains("sigmaNoise")    ? cl::GetValue("sigmaNoise")   : 6;
    float sigmaGain =       cl::Contains("sigmaGain")     ? cl::GetValue("sigmaGain")    : 3;


    // set baseline, noise and gain for each pixel using three gaussian distributions
    for(unsigned int r=0; r < matrix.GetNrows(); ++r){
        for(unsigned int c=0; c < matrix.GetNcols(); ++c){
            matrix(r, c).baseline = matrix.SetBaseline(meanBaseline, sigmaBaseline);
            matrix(r, c).noise = matrix.SetNoise(meanNoise, sigmaNoise);
            matrix(r, c).gain = matrix.SetGain(meanGain, sigmaGain);
        }
    }

    int nEvents = cl::Contains("events") ? (int)cl::GetValue("events") : 100;
    Simulation simulation;
    Simulation::Hit hit;

    for(int iEv=0; iEv<nEvents; ++iEv){
        hit = simulation.GenerateHit();
    }

    return 0;
}
