#include "ParticleSystem.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "Proyectile.h"
#include "Firework.h"

ParticleSystem::ParticleSystem() : _particles(0)
{
}

ParticleSystem::~ParticleSystem()
{
    auto p = _particles.begin();
    while (p != _particles.end()) {
        delete* p;
        p = _particles.erase(p);
    }

    auto a = _particle_generators.begin();
    while (a != _particle_generators.end()) {
        delete* a;
        a = _particle_generators.erase(a);
    }
}

void ParticleSystem::addParticleGenerator(string name, ProyectileTypes pT, GeneratorTypes gT, Vector3 mediavel, Vector3 mediapos, Vector3 anchovel, Vector3 anchopos, double genProb, int numPart, double time)
{
    Particle* m;
    switch (pT) {// La part. que definamos aquí será la que se genere en el generador ->

    case Ball:
        m = new Proyectile(ProyectileTypes::Ball);
        break;

    case Bubble:
        m = new Proyectile(ProyectileTypes::Bubble);

        break;

    case Bullet:
        m = new Proyectile(ProyectileTypes::Bullet);

        break;
    }
    m->setPosition({-100000.0,  -100000.0 , -100000.0 });

    switch (gT) {

    case uniform:
    {
        ParticleGenerator* upg = new UniformParticleGenerator(name, mediapos, mediavel, genProb, numPart, m, anchovel, anchopos);
        _particle_generators.push_back(upg);

        break;
    }

    case normal: {

        ParticleGenerator* gpg = new GaussianParticleGenerator(name, mediapos, mediavel, genProb, numPart, m, anchovel, anchopos, time);
        _particle_generators.push_back(gpg);

        break;
    }

    }
}



void ParticleSystem::removeParticleGenerator(string name)
{
    bool found = false;
    auto p = _particle_generators.begin();
    while (!found && p != _particle_generators.end()) {
        if ((*p)->getGenName() == name) {
            delete* p;
            p = _particle_generators.erase(p);
            found = true;
        }
        else
            p++;
    }
}

void ParticleSystem::removeAllParticleGenerators()
{
    auto a = _particle_generators.begin();
    while (a != _particle_generators.end()) {
        delete* a;
        a = _particle_generators.erase(a);
    }
}

void ParticleSystem::update(double t)
{
    for (auto p : _particle_generators) {
        auto l = p->generateParticles();
        for (auto q : l)
            _particles.push_back(q);
    }

    for (auto p : _particles) {
        // DELETE CONDITION
        // p->getTime() <= 0 || p->getPose().p.y <= 0 || p->getPose().p.y >= 200.0
        if (!p->integrate(t))  
        {
            onParticleDeath(p);
        }
    }

    auto p = _particles.begin();
    while (p != _particles.end()) {
        if (!(*p)->isAlive()) {
            delete* p;
            p = _particles.erase(p);
        }
        else
            p++;
    }


}

ParticleGenerator* ParticleSystem::getParticleGenerator(string name)
{
    for (auto p : _particle_generators)
        if (p->getGenName() == name)
            return p;
    return nullptr;
}

void ParticleSystem::generateFireworkSystem()
{
    Particle base_p = new Particle({0.0, 0.0, 0.0}, {0.0, 2.0, 0.0}, _gravity, 0.999, 0.5, 0.0, FOG);

    std::shared_ptr<ParticleGenerator*> gen1(new GaussianParticleGenerator({0, 0, 0}, {0.3, 0.2, 0.2}, 0.2, 14));
    gen1->setParticle(base_p);
    gen1->setName("Base Particle generator");
    _firework_pool.push_back();

}

void ParticleSystem::onParticleDeath(Particle* p)
{

   // p->setAlive(false);

    // Comprobar si es un firework
    Firework* f;
    f = dynamic_cast<Firework*>(p);

    if (f != nullptr) {
        f->explode();
    }
}

void ParticleSystem::shootFirework(int type)
{
}
