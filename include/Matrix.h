#ifndef Matrix_h
#define Matrix_h

#include "Pixel.h"

#include <set>

#include <TH2I.h>

typedef std::set< std::pair<unsigned int, unsigned int> > HitPixels;

class Matrix{

public:

    Matrix(unsigned int rows, unsigned int cols);
    ~Matrix();

    Pixel& operator()(unsigned int row, unsigned int col);

    float SetBaseline(float meanBaseline, float sigmaBaseline);
    float SetGain(float meanGain, float sigmaGain);
    float SetNoise(float meanNoise, float sigmaNoise);
    void UpdateHitPixelsCount(const HitPixels& hitPix);
    TH2I* FillHitMap();
    
    void PrintPixel(unsigned int row, unsigned int col) const;

    inline const unsigned int GetNrows() const { return fNrows; }
    inline const unsigned int GetNcols() const { return fNcols; }

    // continuous matrix dimensions in um (per adesso)
    static float Lx;
    static float Ly;

private:

    const unsigned int fNrows;
    const unsigned int fNcols;
    Pixel* fPixels;

    void SetPixelCoordinates();   // set all the pixel coordinates
    bool CheckCoords(const std::pair<unsigned int, unsigned int>& coords) const;

};

#endif

