#include "Simulation.h"
#include "Random.h"
#include "Matrix.h"

#include <vector>

Simulation::Simulation():
fSigma(0.0),
fElNoise(20.0),
fTotalElectrons(1636),
fScurve(nullptr),
fRootFile(nullptr){
    // 2D gaussian as product of 2 1D gaussians, with norm. factor.
    // The gaussian is defined over the entire matrix.
    const char* gaus2D_formula = "[0]*TMath::Gaus(x,[1],[2])*TMath::Gaus(y,[3],[4])";
    float xMin = -Matrix::Lx;
    float xMax = 2*Matrix::Lx;
    float yMin = -Matrix::Ly;
    float yMax = 2*Matrix::Ly;

    fGaus2D = new TF2("Gaus2D", gaus2D_formula, xMin, xMax, yMin, yMax);
    fGaus2D->SetParName(0, "C");
    fGaus2D->SetParName(1, "mean_x");
    fGaus2D->SetParName(2, "sigma_x");
    fGaus2D->SetParName(3, "mean_y");
    fGaus2D->SetParName(4, "sigma_y");

    // ntuple to store hit data
    fNtuple = new TNtuple("Ntuple","","Hit_x:Hit_y:Cloud_width:HitPix_row:HitPix_col:HitPix_charge");

    fHistAnalog = new TH1F("HistAnalog", "Analog spectrum", 150, 0, 1.5);

    ChipSide side = ChipSide::back;

    if(side == ChipSide::front)
        fWidthProb = new TF1("WidthProb", "[c]*x*exp(-[k]*x*x)", 0, 5);
    else if(side == ChipSide::back){
        fWidthProb = new TF1("WidthProb", "[c]*x*exp(-[k]*([tk]-x*x))", 0, 2.5);
        fWidthProb->SetParameter("tk", 200.0/25.0);
    }

    fWidthProb->SetParameter("c", 1);
    fWidthProb->SetParameter("k", 14.3);

    double integr = fWidthProb->Integral(fWidthProb->GetXmin(), fWidthProb->GetXmax());
    fWidthProb->SetParameter("c", 1/integr);
}

Simulation::~Simulation(){
    fRootFile->Write();
    fRootFile->Close();
}

float Simulation::RandomizeCloudWidth(float mu){

    double pmax = fWidthProb->GetMaximum();

    double r1, r2 = 0;
    double width = 0;

    float a = fWidthProb->GetXmin();
    float b = fWidthProb->GetXmax();

    // rejection method
    r2 = 1.5;
    width = fWidthProb->GetParameter("k");

    while(r2 > (fWidthProb->Eval(width)/pmax)){
        r1 = Random::Uniform(0,1);
        width = r1*(b-a)+a;
        r2 = Random::Uniform(0,1);
    } 

    return width+0.08;    
}

Hit Simulation::GenerateHit(const float sigma){
    // generate Fe55 hit in the continuos space.
    // Hit x and y coordinates are used to set mean of the 2D gaussian.
    fSigma = sigma;
    Hit hit;
    hit.x = Random::Uniform(-3*fSigma, Matrix::Lx+3*fSigma);
    hit.y = Random::Uniform(-3*fSigma, Matrix::Ly+3*fSigma);
    hit.row = hit.y / Pixel::pitch;
    hit.col = hit.x / Pixel::pitch;
    // it is not a radius but the side of a square at this moment.
    // It will become the radius of a circle?
    hit.radius = sigma;

    // set norm. constant, mean and sigma of the 2D gaussian
    fGaus2D->SetParameter("C", 1.0/(2*M_PI*pow(sigma,2)));
    fGaus2D->SetParameter("mean_x", hit.x);
    fGaus2D->SetParameter("mean_y", hit.y);
    fGaus2D->SetParameter("sigma_x", fSigma);
    fGaus2D->SetParameter("sigma_y", fSigma);
    return hit;
}

HitPixels Simulation::GetHitPixels(Hit& hit){
    // First approach: get hit pixels using a square centered in
    // x_hit and y_hit, with side = 2*sigma.
    unsigned int left, right, bottom, top = 0;

    left   = floor(hit.x - 3*hit.radius); // column
    right  = floor(hit.x + 3*hit.radius); // column
    bottom = floor(hit.y - 3*hit.radius); // row
    top    = floor(hit.y + 3*hit.radius); // row

    unsigned int nRows = Matrix::Ly / Pixel::pitch;
    unsigned int nCols = Matrix::Lx / Pixel::pitch;

    // take all possible pairs. Duplicates are automatically removed
    HitPixels hitPixCoords;
    if(bottom < nRows && left < nCols) 
        hitPixCoords.insert(std::make_pair(bottom, left));
    if(bottom < nRows && right < nCols) 
        hitPixCoords.insert(std::make_pair(bottom, right));
    if(top < nRows && left < nCols) 
        hitPixCoords.insert(std::make_pair(top, left));
    if(top < nRows && right < nCols) 
        hitPixCoords.insert(std::make_pair(top, right));

    // store hit pixels also in hit for future operations
    for(auto& coords : hitPixCoords){
        hit.hitPixels.insert(std::make_pair(coords, 0.0));
    }

    // set integral computation accuracy
    float epsilon = 1e-04;
 
    // loop over hit pixels and compute collected charge
    for(auto& hitPix : hit.hitPixels){
        // coords.first = row -> y range
        // coords.second = col -> x range
        auto& coords = hitPix.first;
        // set integral range
        float xMin = coords.second*Pixel::pitch;
        float xMax = (coords.second+1)*Pixel::pitch;
        float yMin = coords.first*Pixel::pitch;
        float yMax = (coords.first+1)*Pixel::pitch;
        
        double charge = fGaus2D->Integral(xMin, xMax, yMin, yMax, epsilon);
        charge *= fTotalElectrons;
        charge += Random::Gauss(0.0, fElNoise);
        charge /= fTotalElectrons;
        hitPix.second = charge;
        if(charge > 0)
            fHistAnalog->Fill(charge);
    }

    return hitPixCoords;
}

void Simulation::ComputeScurve(int NumEvents){
    unsigned int nBins = fHistAnalog->GetNbinsX();
    float sum = 0;

    std::vector<double> threshold; 
    std::vector<double> rate;

    // points to construct the Scurve graph
    for(unsigned int iBin=nBins; iBin > 0; --iBin){
        threshold.push_back( fHistAnalog->GetBinCenter(iBin) );
        sum += fHistAnalog->GetBinContent(iBin);
        rate.push_back(sum/NumEvents);
    }

    fScurve = new TGraph(rate.size(), &threshold[0], &rate[0]);
    fScurve->SetName("Scurve");
}

void Simulation::SaveHitData(Hit& hit){
    return FillNtuple(hit);
}

void Simulation::FillNtuple(Hit& hit){
    
    for(auto& hitPix : hit.hitPixels)
        fNtuple->Fill(hit.x, hit.y, hit.radius, 
                      hitPix.first.first, 
                      hitPix.first.second, 
                      hitPix.second);
    return;
}

void Simulation::SaveOutput(const char* name = ""){
    fRootFile = new TFile(name, "RECREATE");
    fRootFile->cd();

    fHistAnalog->Write();
    fScurve->Write();
    fWidthProb->Write();
    fNtuple->Write();
    return;
}

