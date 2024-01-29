#ifndef Pixel_h
#define Pixel_h

struct Pixel{
    
    static const float pitch;

    unsigned int row, col;      // pixel position in matrix
    float baseline;
    float noise;
    float gain;
    bool isHit;
    unsigned int count;         // how many times the pixel is hit
    float charge;               // charge related to the hit

    void PrintValues() const;
};

#endif