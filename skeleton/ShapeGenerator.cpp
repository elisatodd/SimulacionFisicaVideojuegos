#include "ShapeGenerator.h"
#include <random>
#include <math.h>
# define M_PI           3.14159265358979323846  /* pi */

list<Particle*> ShapeGenerator::generateParticles() {

    std::list<Particle*> generations = list<Particle*>();

    float alpha = 0.0f;
    float x, y;

    for (size_t i = 0; i < 30; ++i) {
        auto part = _model->clone();
        generations.push_back(part); // new particle

        // modify its pos. and vel. according to the generator type

        auto radians = (alpha * M_PI) / 180;
        x = std::cos(radians);
        y = std::sin(radians);
        alpha += (360.0 / 30);
            
        part->setPosition({ 10, 40 , 10 });
        part->setVel({ x*20, y*20, 0 });

        // set a rnd life span
        part->setRemainingTime(_max_t);
       
    }

    return generations;
}