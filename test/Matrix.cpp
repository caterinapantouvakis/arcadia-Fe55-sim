#include "Matrix.h"
#include "Random.h"

float Matrix::Lx = 0;
float Matrix::Ly = 0;

Matrix::Matrix(unsigned int rows, unsigned int cols):
fNrows(rows), fNcols(cols){

    // create the matrix as a 1D array of rows times cols pixels
    fPixels = new Pixel[fNrows*fNcols];

    Matrix::Ly = fNrows * Pixel::pitch;
    Matrix::Lx = fNcols * Pixel::pitch;

    SetCoordinates();


}

float Matrix::SetBaseline(float meanBaseline, float sigmaBaseline){
    float baseline = Random::GetGenerator()->GenerateGauss(meanBaseline, sigmaBaseline);
    return baseline;
}

float Matrix::SetNoise(float meanNoise, float sigmaNoise){
    float noise = Random::GetGenerator()->GenerateGauss(meanNoise, sigmaNoise);
    return noise;
}

float Matrix::SetGain(float meanGain, float sigmaGain){
    float gain = Random::GetGenerator()->GenerateGauss(meanGain, sigmaGain);
    return gain;
}

void Matrix::PrintPixel(unsigned int row, unsigned int col) const{
    fPixels[col*fNrows + row].PrintValues();
}

Matrix::~Matrix(){
    delete fPixels;
}

Pixel& Matrix::operator()(unsigned int row, unsigned int col){
    return fPixels[col*fNrows + row];
}

void Matrix::SetCoordinates(){

    for(unsigned int r=0; r<fNrows; ++r){
        for(unsigned int c=0; c<fNcols; ++c){
            fPixels[c*fNrows + r].row = r;
            fPixels[c*fNrows + r].col = c;
        }
    }

    return;
}