#include "Pixel.h"

#include <iostream>

const float Pixel::pitch = 25;

void Pixel::PrintValues() const {
    std::cout << "Row:      " << row      << std::endl;
    std::cout << "Col:      " << col      << std::endl;
    std::cout << "Baseline: " << baseline << std::endl;
    std::cout << "Noise:    " << noise    << std::endl;
    std::cout << "Gain:     " << gain     << std::endl;
}