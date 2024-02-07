#ifndef Pixel_h
#define Pixel_h

#include <iostream>

struct Pixel{
    
    static constexpr float pitch = 25.0;

    unsigned int row, col;      // pixel position in matrix
    float baseline;
    float noise;
    float gain;
    bool isHit;
    unsigned int count;         // how many times the pixel is hit
    float charge;               // charge related to the hit
    
    void PrintValues() const {
        std::cout << "Row:      " << row      << std::endl;
        std::cout << "Col:      " << col      << std::endl;
        std::cout << "Baseline: " << baseline << std::endl;
        std::cout << "Noise:    " << noise    << std::endl;
        std::cout << "Gain:     " << gain     << std::endl;
        std::cout << "Is Hit:   " << isHit    << std::endl;
        std::cout << "Count:    " << count    << std::endl;
        std::cout << "Charge:   " << charge   << std::endl;
        return;
    }
};

#endif