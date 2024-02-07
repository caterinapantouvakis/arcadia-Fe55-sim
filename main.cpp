#include <iostream>
#include <unordered_map>

#include "Matrix.h"
#include "Random.h"
#include "CommandLine.h"
#include "Simulation.h"

int main(int argc, char** argv){

    // create CommandLine and retrieve argv inputs
    typedef CommandLine cl;
    cl::Init(argc, argv);

    cl::SetDefaultInfo("outDir", "../results/");
    std::string outputDir = cl::GetInfo("outDir");
    system( ("mkdir -p " + outputDir).c_str() );

    // attenuation lenghts
    const float attenuationLength_Fe55 = 322.52e-04;    // at 6 keV
    const float attenuationLength_Cu = 150.70e-04;      // at 8 keV
    const float attenuationLength_Ti = 682.95e-04;      // at 4.63 keV

    cl::SetDefaultValue("size", 1);
    int size = (int)cl::GetValue("size");

    Matrix matrix(size,size);
    //cl::SetDefaultValue("meanBaseline", 115);
    //float meanBaseline = cl::GetValue("meanBaseline");
    //cl::SetDefaultValue("meanNoise", 0);
    //float meanNoise = cl::GetValue("meanNoise");
    //cl::SetDefaultValue("meanGain", 10);
    //float meanBaseline = cl::GetValue("meanGain");
    //cl::SetDefaultValue("sigmaBaseline", 5);
    //float meanBaseline = cl::GetValue("sigmaBaseline");
    //cl::SetDefaultValue("sigmaNoise", 6);
    //float meanBaseline = cl::GetValue("sigmaNoise");
    //cl::SetDefaultValue("sigmaGain", 3);
    //float meanBaseline = cl::GetValue("sigmaGain");

    // set baseline, noise and gain for each pixel using three gaussian distributions
    //for(unsigned int r=0; r < matrix.GetNrows(); ++r){
    //    for(unsigned int c=0; c < matrix.GetNcols(); ++c){
    //        matrix(r, c).baseline = matrix.SetBaseline(meanBaseline, sigmaBaseline);
    //        matrix(r, c).noise = matrix.SetNoise(meanNoise, sigmaNoise);
    //        matrix(r, c).gain = matrix.SetGain(meanGain, sigmaGain);
    //    }
    //}

    cl::SetDefaultValue("events", 300000);
    int nEvents = (int)cl::GetValue("events");
    const std::unordered_map<std::string, Simulation::ChipSide> sides = { 
        {"front", Simulation::ChipSide::front}, 
        {"back ", Simulation::ChipSide::back } 
    };
    
    Simulation::ChipSide side;
    side = sides.at(cl::GetInfo("side"));
    Simulation sim(side);

    // 1. Generate Fe55 hit;
    // 2. Get hit pixels using "square technique";
    // 3. Compute collected charge for every hit pixel; Add noise/baseline/gain contributions
    // 4. Save hit data in a Tree
    
    // variable used to simulate the depth at which X-ray interacts.
    double elCloudWidth = 0.08*Pixel::pitch;
    for(int iEv=0; iEv < nEvents; ++iEv){
        if( !(iEv % 100000) )
            std::cout << "Event : " << iEv << std::endl;

        elCloudWidth = sim.RandomizeCloudWidth(attenuationLength_Fe55);
        Hit hit = sim.GenerateHit(elCloudWidth);
        auto hitPixels = sim.GetHitPixels(hit);
        matrix.UpdateHitPixelsCount(hitPixels);
        sim.SaveHitData(hit);
    }

    //TH2I* hitMap = matrix.FillHitMap();
    //sim.Save(hitMap);
    sim.ComputeScurve(nEvents, true);
    

    // save output
    cl::SetDefaultInfo("outFile", "output.root");
    std::string fileName = outputDir + cl::GetInfo("outFile") + ".root";
    sim.SaveOutput(outputDir, fileName.c_str());
    system( ("cat " + outputDir + "log.txt").c_str() );

    return 0;
}
