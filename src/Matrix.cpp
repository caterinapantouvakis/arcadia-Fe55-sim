#include "Matrix.h"
#include "Random.h"

float Matrix::Lx;
float Matrix::Ly;

Matrix::Matrix(unsigned int rows, unsigned int cols):
fNrows(rows),
fNcols(cols){

    // create the matrix as a 1D array of rows times cols pixels
    fPixels = new Pixel[fNrows*fNcols];

    Matrix::Ly = fNrows * Pixel::pitch;
    Matrix::Lx = fNcols * Pixel::pitch;

    SetPixelCoordinates();
}

Matrix::~Matrix(){
    delete[] fPixels;
}

Pixel& Matrix::operator()(unsigned int row, unsigned int col){
    return fPixels[col*fNrows + row];
}

bool Matrix::CheckCoords(const std::pair<unsigned int, unsigned int>& coords) const{
    if( coords.first < fNrows && coords.second < fNcols )
        return true;
    else
        return false;
}

float Matrix::SetBaseline(float meanBaseline, float sigmaBaseline){
    float baseline = Random::Gauss(meanBaseline, sigmaBaseline);
    return baseline;
}

float Matrix::SetNoise(float meanNoise, float sigmaNoise){
    float noise = Random::Gauss(meanNoise, sigmaNoise);
    return noise;
}

float Matrix::SetGain(float meanGain, float sigmaGain){
    float gain = Random::Gauss(meanGain, sigmaGain);
    return gain;
}

void Matrix::PrintPixel(unsigned int row, unsigned int col) const{
    fPixels[col*fNrows + row].PrintValues();
    return;
}

void Matrix::SetPixelCoordinates(){
    for(unsigned int r=0; r<fNrows; ++r){
        for(unsigned int c=0; c<fNcols; ++c){
            fPixels[c*fNrows + r].row = r;
            fPixels[c*fNrows + r].col = c;
            fPixels[c*fNrows + r].isHit = false;
            fPixels[c*fNrows + r].count = 0;
        }
    }
    return;
}

void Matrix::UpdateHitPixelsCount(const HitPixels& hitPixels){
    // hitPix.first -> row
    // hitPix.second -> col
    for(auto& hitPix : hitPixels){
        fPixels[hitPix.second*fNrows + hitPix.first].count += 1;
        fPixels[hitPix.second*fNrows + hitPix.first].isHit = true;
    }
    return;
}

TH2I* Matrix::FillHitMap(){
    TH2I* hitsMap = new TH2I("HitsMap", "Hit distribution", fNcols, 0, fNcols, fNrows, 0, fNrows);

    // fare che setto il bin content con le righe e le conne e il contenuto con count

    return hitsMap;
}