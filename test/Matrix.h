#ifndef Matrix_h
#define Matrix_h

#include "Pixel.h"

class Matrix{

public:

    Matrix(unsigned int rows, unsigned int cols);
    ~Matrix();

    Pixel& operator()(unsigned int row, unsigned int col);

    float SetBaseline(float meanBaseline, float sigmaBaseline);
    float SetGain(float meanGain, float sigmaGain);
    float SetNoise(float meanNoise, float sigmaNoise);
    
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

    void SetCoordinates();   // set all the pixel coordinates


};



#endif

