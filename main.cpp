#include <iostream>

#include "Matrix.h"
#include "Random.h"
#include "CommandLine.h"
#include "Simulation.h"

int main(int argc, char** argv){

    // create CommandLine and retrieve argv inputs
    typedef CommandLine cl;
    cl::Init(argc, argv);

    const float attenuationLength = 322.52e-04;    // at 6 keV

    int size = (int)cl::GetValue("size");
    Matrix matrix(size,size);
    // per dopo: automatizzare questa cosa dei valori di default
    float meanBaseline =    cl::Contains("meanBaseline")  ? cl::GetValue("meanBaseline") : 115;
    float meanNoise =       cl::Contains("meanNoise")     ? cl::GetValue("meanNoise")    : 0;
    float meanGain =        cl::Contains("meanGain")      ? cl::GetValue("meanGain")     : 10;
    float sigmaBaseline =   cl::Contains("sigmaBaseline") ? cl::GetValue("sigmaBaseline"): 5;
    float sigmaNoise =      cl::Contains("sigmaNoise")    ? cl::GetValue("sigmaNoise")   : 6;
    float sigmaGain =       cl::Contains("sigmaGain")     ? cl::GetValue("sigmaGain")    : 3;

    // set baseline, noise and gain for each pixel using three gaussian distributions
    //for(unsigned int r=0; r < matrix.GetNrows(); ++r){
    //    for(unsigned int c=0; c < matrix.GetNcols(); ++c){
    //        matrix(r, c).baseline = matrix.SetBaseline(meanBaseline, sigmaBaseline);
    //        matrix(r, c).noise = matrix.SetNoise(meanNoise, sigmaNoise);
    //        matrix(r, c).gain = matrix.SetGain(meanGain, sigmaGain);
    //    }
    //}

    int nEvents = cl::Contains("events") ? (int)cl::GetValue("events") : 100;
    Simulation sim;


    // 1. Generate Fe55 hit;
    // 2. Get hit pixels using "square technique";
    // 3. Compute collected charge for every hit pixel; Add noise/baseline/gain contributions
    // 4. Save hit data in a Tree
    
    // variable used to simulate the depth at which X-ray interacts.
    double elCloudWidth = 0.14;
    for(int iEv=0; iEv < nEvents; ++iEv){
        if( !(iEv % 100000) )
            std::cout << "Event : " << iEv << std::endl;

        elCloudWidth = sim.RandomizeCloudWidth(attenuationLength);
        Hit hit = sim.GenerateHit(elCloudWidth);
        auto hitPixels = sim.GetHitPixels(hit);
        matrix.UpdateHitPixelsCount(hitPixels);
        sim.SaveHitData(hit);

    }

    //TH2I* hitMap = matrix.FillHitMap();
    sim.ComputeScurve(nEvents);
    sim.SaveOutput("output.root");
    

    return 0;
}
