#include "ParticleSystem.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "ShapeGenerator.h"
#include "Proyectile.h"
#include "Firework.h"

#include "GravityForceGenerator.h"

ParticleSystem::ParticleSystem() : _particles(0)
{
    pfr = new ParticleForceRegistry();


    addParticleGenerator("Bubble_Cannon", ProyectileTypes::Bubble, GeneratorTypes::normal,
        GetCamera()->getDir() * 10, GetCamera()->getEye() + GetCamera()->getDir() * 3,
        { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, 0.5, 1, 5.0);

    addParticleGenerator("Ball_Fountain", ProyectileTypes::Ball, GeneratorTypes::uniform,
        { 0.0, 20.0, 0.0 }, { 20, 10, 20 }, { 6.0, 3.0, 6.0 }, { 1.0, 1.0, 1.0 }, 0.5, 5, 10.0);

    addParticleGenerator("Beehive", ProyectileTypes::Bullet, GeneratorTypes::normal,
        { 0.0, 50.0, 0.0 }, { 20, 10, 20 }, { 4.0, 3.0, 4.0 }, { 1.0, 1.0, 1.0 }, 0.5, 10, 5.0);

    addParticleGenerator("Bullet_Rain", ProyectileTypes::Bullet, GeneratorTypes::normal,
        	{ 0.0, 50.0, 0.0 }, { 20, 10, 20 }, { 4.0, 3.0, 4.0 }, { 1.0, 1.0, 1.0 }, 0.5, 10, 5.0);

    addParticleGenerator("ShapeTest", ProyectileTypes::Firework0, GeneratorTypes::shape,
        { 0.0, 50.0, 0.0 }, { 20, 10, 20 }, { 4.0, 3.0, 4.0 }, { 1.0, 1.0, 1.0 }, 1.0, 10, 5.0);
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
    //    delete* a;
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

    case Firework0:
        m = new Proyectile(ProyectileTypes::Firework0);
        break;
    }
    m->setPosition({ -100000.0,  -100000.0 , -100000.0 });

    switch (gT) {

    case uniform:
    {
       // ParticleGenerator* upg = new UniformParticleGenerator(name, mediapos, mediavel, genProb, numPart, m, anchovel, anchopos);
        shared_ptr<ParticleGenerator> upg(new UniformParticleGenerator(name, mediapos, mediavel, genProb, numPart, m, anchovel, anchopos));
        _particle_generators.push_back(upg);

        break;
    }

    case normal: {

      //  ParticleGenerator* gpg = new GaussianParticleGenerator(name, mediapos, mediavel, genProb, numPart, m, anchovel, anchopos, time);
        shared_ptr<ParticleGenerator> gpg(new GaussianParticleGenerator(name, mediapos, mediavel, genProb, numPart, m, anchovel, anchopos, time));
        _particle_generators.push_back(gpg);
        break;
    }

    case shape: {
        shared_ptr<ParticleGenerator> spg(new ShapeGenerator(name, mediapos, mediavel, genProb, numPart, m, anchovel, anchopos, time));
        _particle_generators.push_back(spg);
        break;
    }

    }
}

void ParticleSystem::addParticleGenerator(shared_ptr<ParticleGenerator> gen)
{
    _particle_generators.push_back(gen);
}

void ParticleSystem::addParticle(Particle* p)
{
    _particles.push_back(p);
}



void ParticleSystem::removeParticleGenerator(string name)
{
    bool found = false;
    auto p = _particle_generators.begin();
    while (!found && p != _particle_generators.end()) {
        if ((*p)->getGenName() == name) {
            //delete* p; // cuando no es shared ptr
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
     //   delete* a;
        a = _particle_generators.erase(a);
    }
}

void ParticleSystem::deactivateAllParticleGenerators()
{
    for (auto p : _particle_generators) {
        p->setActive(false);
    }
}

void ParticleSystem::update(double t)
{
    pfr->updateForces(t);

    for (auto p : _particle_generators) {
        if (p->getActive()) {
            auto l = p->generateParticles();
            for (auto q : l) {
                _particles.push_back(q);
            }
        }
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
            return p.get();
    return nullptr;
}

void ParticleSystem::generateFireworkSystem()
{
    physx::PxVec3 _gravity = { 0.0,  -10.0, 0.0 };

    shared_ptr<ParticleGenerator> g0(new GaussianParticleGenerator("FIREWORKS GAUSSIAN GENERATOR initial",
        { -20, 0, -20 }, { 0.0, 60.0, 0.0 }, 1.0, 1,
        nullptr, { 3.0, 2.0, 3.0 }, { 0.1, 0.1, 0.1 }, 1.0 ));
    
    addParticleGenerator(g0);

    auto pHumo = new Proyectile(ProyectileTypes::Smoke);

    shared_ptr<ParticleGenerator> g1(new GaussianParticleGenerator("FIREWORKS GAUSSIAN GENERATOR", 
                                                                { 0.0, 10.0, 0.0 }, { 20, 10, 20 }, 1.0, 1,
                                                                pHumo, { 2.0, 1.0, 2.0 }, {1.0, 1.0, 1.0}, 0.5));

    addParticleGenerator(g1);

    auto fBase = new Firework({ -10000.0, -10000.0, -10000.0 }, { 0.0, 0.0, 0.0 }, _gravity, 0.99, { g1 }, 0.5);
    fBase->setColor({ 1.0, 1.0, 0.5, 1.0 });
    _firework_pool.push_back(fBase);

    shared_ptr<ParticleGenerator> g2(new GaussianParticleGenerator("FIREWORKS GAUSSIAN GENERATOR 1", 
                                                                { 0.0, 20.0, 0.0 }, { 20, 10, 20 }, 1.0, 4,
                                                                _firework_pool[0], {2.0, 1.0, 2.0}, {1.0, 1.0, 1.0}, 0.5));
    
    addParticleGenerator(g2);

    auto p = new Firework({ -10000.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, _gravity, 0.99, { g2, g1 }, 1.0);
    p->setColor({ 1.0, 1.0, 0.0, 1.0 });
    _firework_pool.push_back(p);

    shared_ptr<ParticleGenerator> g3(new GaussianParticleGenerator("FIREWORKS GAUSSIAN GENERATOR 2", 
                                                                { 0.0, 20.0, 0.0 }, { 20, 10, 20 }, 1.0, 6,
                                                                _firework_pool[1], {2.0,1.0, 2.0}, {1.0, 1.0, 1.0}, 1.5));
    
    auto p1 = new Firework({ -10000.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, _gravity, 0.99, { g3 }, 1.5);
    p1->setColor({ 1.0, 0.5, 0.0, 1.0 });
    _firework_pool.push_back(p1);

    shared_ptr<ParticleGenerator> g4(new GaussianParticleGenerator("FIREWORKS GAUSSIAN GENERATOR 3", 
                                                                { 0.0, 30.0, 0.0 }, { 20, 10, 20 }, 1.0, 8,
                                                                _firework_pool[2], {2.0, 1.0, 2.0}, {1.0, 1.0, 1.0}, 3.0));
    
    auto p2 = new Firework({ -10000.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, _gravity, 0.99, { g4, g2 }, 2.0);
    p2->setColor({1.0, 0.0, 0.0, 1.0});
    _firework_pool.push_back(p2);
}

void ParticleSystem::onParticleDeath(Particle* p)
{
   // p->setAlive(false);

    // Comprobar si es un firework
    Firework* f;
    f = dynamic_cast<Firework*>(p);

    if (f != nullptr) {
        for (auto fire : f->explode()) {
            _particles.push_back(fire);
        }
    }
}

void ParticleSystem::shootFirework(int type)
{
    generateFireworkSystem();

    auto model = _firework_pool[type]->clone();
    auto gen = getParticleGenerator("FIREWORKS GAUSSIAN GENERATOR initial");
    gen->setParticle(model);

    auto p = gen->generateParticles().front();

    _particles.push_back(p);
}

void ParticleSystem::addBubbles()
{
    auto p = new Proyectile(ProyectileTypes::Bubble);
    p->setABF(true);
    p->setPosition({ 40.0, 50.0, 40.0 });
    p->setAcc({ 0.0, 0.0, 0.0 });
    p->setVel({ 0.0, 0.0, 0.0 });
    p->setRemainingTime(MAXINT);
    p->setMass(50000.0f);
    p->setDamping(0.1f);
    _particles.push_back(p);

    auto p2 = new Proyectile(ProyectileTypes::Bubble);
    p2->setABF(true);
    p2->setPosition({ 45.0, 50.0, 35.0 });
    p2->setAcc({ 0.0, 0.0, 0.0 });
    p2->setVel({ 0.0, 0.0, 0.0 });
    p2->setRemainingTime(MAXINT);
    p2->setMass(50000.0f);
    p2->setDamping(0.9f); // caerá más rápido que p1
    _particles.push_back(p2);

    auto gfg = new GravityForceGenerator({ 0.0, -9.8, 0.0 });

    pfr->addRegistry(gfg, p);
    pfr->addRegistry(gfg, p2);
}

void ParticleSystem::addDifferentABF() { // dispara 2 burbujas, una a la que le afecta la G y otra no
    // DUDA -> CAE MUY RÀPIDO
    auto p = new Proyectile(ProyectileTypes::Bubble);
    p->setABF(true);
    p->setPosition({ 40.0, 50.0, 40.0 });
    p->setRemainingTime(MAXINT);
    p->setMass(500000.0f);
    _particles.push_back(p);

    auto p2 = new Proyectile(ProyectileTypes::Bubble);
    p2->setPosition({ 45.0, 50.0, 35.0 });
    p2->setRemainingTime(MAXINT);
    _particles.push_back(p2);

    auto gfg = new GravityForceGenerator({ 0.0, -9.8, 0.0 });

    pfr->addRegistry(gfg, p);
}