#ifndef Pixel_h
#define Pixel_h

struct Pixel{
    
    static const float pitch;

    unsigned int row, col;      // pixel position in matrix
    float baseline;
    float noise;
    float gain;
    float charge;               // charge related to the hit

    void PrintValues() const;

};

#endif