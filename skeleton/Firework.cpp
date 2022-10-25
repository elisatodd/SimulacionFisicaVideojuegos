#include "Firework.h"

Firework::Firework(Vector3 pos, Vector3 dir, Vector3 acc, double dam, list<shared_ptr<ParticleGenerator>> lG, float radius) : 
                                            Particle(ParticleTypes::FireworkT, radius, pos, dir, acc, dam), _gens(lG)
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
    Firework* f = new Firework(pose.p, vel, acceleration, damping, _gens, radius);
    f->setColor(renderItem->color);
    f->setRemainingTime(remainingTime);

    return f;
}

list<Particle*> Firework::explode()
{
    list<Particle*> parts;

    for (auto g : _gens) { // Para cada nueva generación fruto de la explosión
        g->setMeanPos(pose.p);
        g->setMeanVel(vel);

        auto p = g->generateParticles();
        for (auto a : p)
            parts.push_back(a);
    }

    return parts;
}
