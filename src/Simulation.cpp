#include "Simulation.h"
#include "Random.h"
#include "Matrix.h"
#include "CommandLine.h"

#include <vector>
#include <fstream>

const float Simulation::fTotalElectrons = 1636.0;
float Simulation::fWidthOffset = 0.0;

Simulation::Simulation(ChipSide side):
fSide(side),
fSigma(0.0),
fElNoise(30.0),
fScurve(nullptr),
fScurveFit(nullptr),
fRootFile(nullptr){
    // 2D gaussian as product of 2 1D gaussians, with norm. factor.
    // The gaussian is defined over the entire matrix.
    const char* gaus2D_formula = "[0]*TMath::Gaus(x,[1],[2])*TMath::Gaus(y,[3],[4])";
    float xMin = -Matrix::Lx;
    float xMax = 2*Matrix::Lx;
    float yMin = -Matrix::Ly;
    float yMax = 2*Matrix::Ly;

    // offset set to sqrt(10)
    CommandLine::SetDefaultValue("offset", 3.16);
    fWidthOffset = CommandLine::GetValue("offset")/Pixel::pitch;

    fGaus2D = new TF2("Gaus2D", gaus2D_formula, xMin, xMax, yMin, yMax);
    fGaus2D->SetParName(0, "C");
    fGaus2D->SetParName(1, "mean_x");
    fGaus2D->SetParName(2, "sigma_x");
    fGaus2D->SetParName(3, "mean_y");
    fGaus2D->SetParName(4, "sigma_y");

    // ntuple to store hit data
    Save(fNtuple = new TNtuple("Ntuple","","Hit_x:Hit_y:Cloud_width:HitPix_row:HitPix_col:HitPix_charge"));

    Save(fHistAnalog = new TH1F("HistAnalog", "Analog spectrum", 150, 0.0, 1.5));

    Save(fHistSigma = new TH1F("HistSigma", "Sigma distr.", 2000, 0.0, 50));

    //fSide = ChipSide::back;

    if(fSide == ChipSide::front)
        fWidthProb = new TF1("WidthProb", "[c]*x*exp(-[k]*x*x)", 0, 5);
    else if(fSide == ChipSide::back){
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
    /*
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
    */
    double x = 0;
    double prob = 0;
    double width = 0;
    float k = 0;
    // for backside case it is neccessary to check that x is positive ??
    while(x <= 0){
        prob = Random::Uniform(0,1);
        switch(fSide){
            case ChipSide::front:
                x = -log(prob)/mu;
                k = CommandLine::GetValue("k");
                width = sqrt(x)*k + fWidthOffset;
                break;
            case ChipSide::back:
                x = 200 + log(prob)/mu;
                k = CommandLine::GetValue("k");
                width = sqrt(x)*k + fWidthOffset;
                break;
        }
    }
    fHistSigma->Fill(width*Pixel::pitch);
    return width*Pixel::pitch;
    //return width+0.08;
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

    left   = floor((hit.x - 3*hit.radius)/Pixel::pitch); // column
    right  = floor((hit.x + 3*hit.radius)/Pixel::pitch); // column
    bottom = floor((hit.y - 3*hit.radius)/Pixel::pitch); // row
    top    = floor((hit.y + 3*hit.radius)/Pixel::pitch); // row

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

void Simulation::ComputeScurve(int nEvents, bool fit){
    unsigned int nBins = fHistAnalog->GetNbinsX();
    float sum = 0;

    std::vector<double> threshold; 
    std::vector<double> rate;

    // points to construct the Scurve graph
    for(unsigned int iBin=nBins; iBin > 0; --iBin){
        threshold.push_back( fHistAnalog->GetBinCenter(iBin) );
        sum += fHistAnalog->GetBinContent(iBin);
        rate.push_back(sum/nEvents);
    }

    Save(fScurve = new TGraph(rate.size(), &threshold[0], &rate[0]));
    fScurve->SetName("Scurve");

    if(fit)
        FitScurve();
    return;
}

void Simulation::FitScurve(){
    const char* fitFormula = "[N_0]*(1+[C_s/sigma]*([t_0]-x))*(1+TMath::Erf(([t_0]-x)/[sigma]))";
    Save(fScurveFit = new TF1("fit", fitFormula, 0.7, 1.2));
    fScurveFit->SetTitle("Scurve fit");

    // set default parameters to help fit procedure
    std::ifstream paramsFile;
    switch(fSide){
        case ChipSide::front:
            paramsFile.open("../front_fit.txt", std::ios_base::in);
            break;
        case ChipSide::back:
            paramsFile.open("../back_fit.txt", std::ios_base::in);
            break;
    }

    // set default parameters from file
    float Cs_sigma, N_0, sigma, t_0;
    while( paramsFile >> Cs_sigma){
        paramsFile >> N_0;
        paramsFile >> sigma;
        paramsFile >> t_0;
    }

    fScurveFit->SetParameter("C_s/sigma", Cs_sigma);
    fScurveFit->SetParameter("N_0", N_0);
    fScurveFit->SetParameter("sigma", sigma);
    fScurveFit->SetParameter("t_0", t_0);

    fScurve->Fit(fScurveFit, "R");
    fScurve->SetMarkerStyle(kFullTriangleUp);
    fScurve->SetTitle("Scurve");

    float chargeSharing = fScurveFit->GetParameter("C_s/sigma")*fScurveFit->GetParameter("sigma");
    std::cout << "Charge sharing = " << chargeSharing << std::endl;
    return;
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

void Simulation::SaveOutput(std::string& dir, const char* name){
    fRootFile = new TFile(name, "RECREATE");
    fRootFile->cd();

    typedef CommandLine cl;
    // write sim parameters and fit results
    std::ofstream log(dir + "log.txt");
    log << "---- Simulation parameters ----\n" << std::endl;
    log << "Events = " << (int)cl::GetValue("events") << std::endl;
    log << "Side : " << cl::GetInfo("side") << std::endl;
    log << "Matrix size = " << (int)cl::GetValue("size") << std::endl;
    log << "Pixel pitch = " << Pixel::pitch << std::endl;
    log << "Electronic noise = " << fElNoise << std::endl;
    log << "Cloud offset = " << fWidthOffset << std::endl;
    log << "k-factor = " << cl::GetValue("k") << std::endl;
    log << std::endl;

    log << "---- Fit results ----\n" << std::endl;
    log << "C_s/sigma = " << fScurveFit->GetParameter("C_s/sigma") << std::endl;
    log << "N_0 = " << fScurveFit->GetParameter("N_0") << std::endl;
    log << "sigma = " << fScurveFit->GetParameter("sigma") << std::endl;
    log << "t_0 = " << fScurveFit->GetParameter("t_0") << std::endl;
    log << "C_s = " << fScurveFit->GetParameter("C_s/sigma")*fScurveFit->GetParameter("sigma") << std::endl; 
    log.close();

    for(TNamed* obj : fObjToWrite)
        obj->Write();
    return;
}

void Simulation::Save(TNamed* obj){
    // check superfluo ?
    if(dynamic_cast<TNamed*>(obj)) 
        fObjToWrite.push_back(obj);
    return;
}

