#include "Simulation.h"
#include "Random.h"
#include "Matrix.h"

Simulation::Hit Simulation::GenerateHit(){
    Hit hit;
    hit.x = Random::GetGenerator()->GenerateUniform(0, Matrix::Lx);
    hit.y = Random::GetGenerator()->GenerateUniform(0, Matrix::Ly);
    hit.radius = sqrt(Random::GetGenerator()->GenerateUniform(0, 15)); //Fix the extrema of the range ??
    return hit;
}


void Simulation::ComputeElectrons(Hit& hit){
    const float nElectrons = 1640;

    float hitRow = hit.y/Pixel::pitch;
    float hitCol = hit.x/Pixel::pitch;
    

    //hit.hitPixels.push_back(std::make_pair(hitRow,hitCol));


    // check if electrons are collected by a single pixel
    // otherwise look for other pixels involved 

    //hit.hitPixels.insert(std::make_pair(std::make_pair(hitRow,hitCol), 0));

    // eight points top bottom left right and "diagonal"
    Point t, b, l, r, tl, tr, bl, br;
    t.x = hit.x;
    t.y = hit.y + hit.radius;
    b.x = hit.x;
    b.y = hit.y - hit.radius;
    l.x = hit.x - hit.radius;
    l.y = hit.y;
    r.x = hit.x  + hit.radius;
    r.y = hit.y;
    tl.x = hit.x - hit.radius*cos(M_PI/4);
    tl.y = hit.y + hit.radius*sin(M_PI/4);
    tr.x = hit.x + hit.radius*cos(M_PI/4);
    tr.y = tl.y;
    bl.x = tl.x;
    bl.y = hit.y - hit.radius*sin(M_PI/4);
    br.x = tr.x;
    br.y = bl.y;









    if((unsigned int) left/Pixel::pitch == hitCol && (unsigned int) right/Pixel::pitch == hitCol && (unsigned int) up/Pixel::pitch == hitRow && (unsigned int) bottom/Pixel::pitch == hitRow)
        hit.hitPixels.insert(std::make_pair(std::make_pair(hitRow,hitCol), 1640.0f));
        return;
    }
    else{



    }
}