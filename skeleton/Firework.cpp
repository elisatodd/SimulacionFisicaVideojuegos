#include "Firework.h"

Firework::Firework(int gens) : _generationsLeft(gens), _nFireworks(3)
{
}

Firework::~Firework()
{
    
}

//int Firework::integrate(double t)
//{
//    return 0;
//}

Particle* Firework::clone() const
{
    Firework* f = new Firework(_generationsLeft - 1);
    return f;
}

list<Particle*> Firework::explode()
{
    list<Particle*> parts;
    // No es el que ya no explota
    if (_generationsLeft > 0){

        for (auto g : _gens) {
            auto p = g->generateParticles();
            for (auto a : p)
                parts.push_back(a);
        }
    }

    return parts;
}
