#ifndef Simulation_h
#define Simulation_h

#include "Pixel.h"

#include <map>
#include <set>

#include <TF2.h>
#include <TFile.h>
#include <TH1F.h>
#include <TNtuple.h>
#include <TGraph.h>

typedef std::set< std::pair<unsigned int, unsigned int> > HitPixels;

struct Hit{
    float x, y;
    unsigned int row, col;
    float radius;
    std::map< std::pair<unsigned int, unsigned int>, double > hitPixels;
};

class Simulation{

public:
    enum class ChipSide{
        front,
        back
    };

    Simulation();
    ~Simulation();

    float RandomizeCloudWidth(float mu);
    Hit GenerateHit(const float sigma);
    HitPixels GetHitPixels(Hit& hit);
    void SaveHitData(Hit& hit);
    void SaveOutput(const char* name);
    void ComputeScurve(int NumEvents);

    //template<typename T>
    //void Save(const T* object);


private:
    void FillNtuple(Hit& hit);
    
    float fSigma;
    float fElNoise;
    float fTotalElectrons;
    float fNormProb;

    TF2* fGaus2D;
    TH1F* fHistAnalog;
    TGraph* fScurve;
    TF1* fWidthProb;
    TNtuple* fNtuple;
    TFile* fRootFile;
};

#endif

