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
    enum class ChipSide: char{
        front = 'f',
        back = 'b'
    };

    Simulation(ChipSide side);
    ~Simulation();

    inline ChipSide GetSide() const { return fSide; }

    float RandomizeCloudWidth(float mu);
    Hit GenerateHit(const float sigma);
    HitPixels GetHitPixels(Hit& hit);
    void SaveHitData(Hit& hit);
    void SaveOutput(std::string& dir, const char* name);
    void ComputeScurve(int nEvents, bool fit = false);
    void Save(TNamed* object);

private:
    void FillNtuple(Hit& hit);
    void FitScurve();
    
    ChipSide fSide;
    float fSigma;
    float fElNoise;
    static const float fTotalElectrons;
    static float fWidthOffset;
    float fNormProb;

    TF2* fGaus2D;
    TH1F* fHistAnalog;
    TGraph* fScurve;
    TF1* fWidthProb;
    TH1F* fHistSigma;
    TF1* fScurveFit;
    TNtuple* fNtuple;
    TFile* fRootFile;

    // list of objects to write to a TFile
    std::vector<TNamed*> fObjToWrite;
};

#endif

