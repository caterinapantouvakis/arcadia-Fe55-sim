#include <iostream>
#include "Matrix.h"
#include "Random.h"
#include "CommandLine.h"
#include "Simulation.h"

int main(int argc, char** argv){

    CommandLine cl(argc, argv);


    int size = (int)cl.getValue("size");

    Matrix matrix(size,size);

    float meanBaseline =    cl.contains("meanBaseline")  ? cl.getValue("meanBaseline") : 115;
    float meanNoise =       cl.contains("meanNoise")     ? cl.getValue("meanNoise")    : 0;
    float meanGain =        cl.contains("meanGain")      ? cl.getValue("meanGain")     : 10;
    float sigmaBaseline =   cl.contains("sigmaBaseline") ? cl.getValue("sigmaBaseline"): 5;
    float sigmaNoise =      cl.contains("sigmaNoise")    ? cl.getValue("sigmaNoise")   : 6;
    float sigmaGain =       cl.contains("sigmaGain")     ? cl.getValue("sigmaGain")    : 3;


    // set baseline, noise and gain for each pixel using three gaussian distributions
    for(unsigned int r=0; r < matrix.GetNrows(); ++r){
        for(unsigned int c=0; c < matrix.GetNcols(); ++c){
            matrix(r, c).baseline = matrix.SetBaseline(meanBaseline, sigmaBaseline);
            matrix(r, c).noise = matrix.SetNoise(meanNoise, sigmaNoise);
            matrix(r, c).gain = matrix.SetGain(meanGain, sigmaGain);
        }

    }

    int nEvents = cl.contains("events") ? (int)cl.getValue("events") : 100;
    Simulation simulation;
    Simulation::Hit hit;

    for(int iEv=0; iEv<nEvents; ++iEv){
        hit = simulation.GenerateHit();
    }




    return 0;
}
