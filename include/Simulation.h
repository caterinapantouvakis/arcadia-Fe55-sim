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
    Simulation();
    ~Simulation();

    Hit GenerateHit(const float sigma);
    HitPixels GetHitPixels(Hit& hit);
    void SaveHitData(Hit& hit);
    void SaveOutput(const char* name);
    void ComputeScurve();

    //template<typename T>
    //void Save(const T* object);


private:
    void FillNtuple(Hit& hit);

    float fSigma;
    float fElNoise;
    float fTotalElectrons;

    TF2* fGaus2D;
    TH1F* fHistAnalog;
    TGraph* fScurve;
    TNtuple* fNtuple;
    TFile* fRootFile;
};

#endif

