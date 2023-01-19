#include "ParticleSystem.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "ShapeGenerator.h"
#include "Proyectile.h"
#include "Firework.h"

#include "GravityForceGenerator.h"
#include "DragGenerator.h"
#include "UniformWindGenerator.h"
#include "WhirlwindGenerator.h"
#include "ExplosionGenerator.h"

#include "SpringForceGenerator.h"
#include "RubberBandGenerator.h"
#include "BuoyancyForceGenerator.h"

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

    auto r = _forceGenerators.begin();
    while (r != _forceGenerators.end()) {
        r = _forceGenerators.erase(r);
    }

    delete pfr;
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
    if (explosionActive) exp->addConst(t);
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
            pfr->deleteParticleRegistry(*p);
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
        { 0, 25, 0 }, { 0.0, 30.0, 0.0 }, 1.0, 1,
        nullptr, { 3.0, 2.0, 3.0 }, { 0.1, 0.1, 0.1 }, 1.0 ));
    
    addParticleGenerator(g0);

    auto pHumo = new Proyectile(ProyectileTypes::Smoke);
    pHumo->setRemainingTime(1000);

    shared_ptr<ParticleGenerator> g1(new GaussianParticleGenerator("FIREWORKS GAUSSIAN GENERATOR", 
                                                                { 0.0, 10.0, 0.0 }, { 20, 10, 20 }, 1.0, 1,
                                                                pHumo, { 1.0, 1.0, 1.0 }, {1.0, 1.0, 1.0}, 0.5));

    addParticleGenerator(g1);

    auto fBase = new Firework({ -10000.0, -10000.0, -10000.0 }, { 0.0, 0.0, 0.0 }, _gravity, 0.99, { g1 }, 0.05);
    fBase->setRemainingTime(500);
    fBase->setColor({ 1.0, 1.0, 0.5, 1.0 });
    _firework_pool.push_back(fBase);

    shared_ptr<ParticleGenerator> g2(new GaussianParticleGenerator("FIREWORKS GAUSSIAN GENERATOR 1", 
                                                                { 0.0, 20.0, 0.0 }, { 20, 10, 20 }, 1.0, 4,
                                                                _firework_pool[0], {0.5, 0.1, 0.5}, {1.0, 1.0, 1.0}, 0.5));
    
    addParticleGenerator(g2);

    auto p = new Firework({ -10000.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, _gravity, 0.99, { g2, g1 }, 0.1);
    p->setRemainingTime(500);
    p->setColor({ 1.0, 1.0, 0.0, 1.0 });
    _firework_pool.push_back(p);

    shared_ptr<ParticleGenerator> g3(new GaussianParticleGenerator("FIREWORKS GAUSSIAN GENERATOR 2", 
                                                                { 0.0, 20.0, 0.0 }, { 20, 10, 20 }, 1.0, 6,
                                                                _firework_pool[1], { 0.5, 0.1, 0.5 }, {1.0, 1.0, 1.0}, 1.5));
    
    auto p1 = new Firework({ -10000.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, _gravity, 0.99, { g3 }, 0.25);
    p1->setRemainingTime(500);
    p1->setColor({ 1.0, 0.5, 0.0, 1.0 });
    _firework_pool.push_back(p1);

    shared_ptr<ParticleGenerator> g4(new GaussianParticleGenerator("FIREWORKS GAUSSIAN GENERATOR 3", 
                                                                { 0.0, 30.0, 0.0 }, { 20, 10, 20 }, 1.0, 8,
                                                                _firework_pool[2], { 0.5, 0.1, 0.5 }, {1.0, 1.0, 1.0}, 3.0));
    
    auto p2 = new Firework({ -10000.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, _gravity, 0.99, { g4, g2 }, 0.5);
    p2->setRemainingTime(500);
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
    p->setMass(40.0f);
    p->setDamping(0.1f);
    _particles.push_back(p);

    auto p2 = new Proyectile(ProyectileTypes::Bubble);
    p2->setABF(true);
    p2->setPosition({ 45.0, 50.0, 35.0 });
    p2->setAcc({ 0.0, 0.0, 0.0 });
    p2->setVel({ 0.0, 0.0, 0.0 });
    p2->setRemainingTime(MAXINT);
    p2->setMass(20.0f); // aunque tengan masas distintas, esto no afecta
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
    p->setMass(20.0f);
    _particles.push_back(p);

    auto p2 = new Proyectile(ProyectileTypes::Bubble);
    p2->setPosition({ 45.0, 50.0, 35.0 });
    p2->setRemainingTime(MAXINT);
    _particles.push_back(p2);

    auto gfg = new GravityForceGenerator({ 0.0, -9.8, 0.0 });

    pfr->addRegistry(gfg, p);
}

void ParticleSystem::testWind()
{
    auto p = new Proyectile(ProyectileTypes::Bubble);
    p->setABF(true);
    p->setPosition({ 0.0, 0.0, 0.0 });
    // p->setVel({-10.0, 0.0, -10.0});
    p->setVel({0.0, 10.0, 0.0});
   // p->setAcc({ -1.0, 0.0 , -1.0 });
    p->setAcc({0.0 , 1.0, 0.0});
    p->setMass(20.0f);
    p->setRemainingTime(MAXINT);

    _particles.push_back(p);

    auto wfg = new UniformWindGenerator(0.1, 0.2, {-10.0, 1.0, -10.0}, {0.0, 10.0, 0.0},50, 50, 50);
    pfr->addRegistry(wfg, p);
}

void ParticleSystem::testWhirlwind()
{
    auto p = new Proyectile(ProyectileTypes::Bubble);
    p->setABF(true);
    p->setPosition({ 20.0, 10.0, 10.0 });
    p->setVel({ 0.0, 0.0, 0.0 });
    p->setAcc({ 0.0, 0.0 , 0.0 });
    p->setMass(20.0f);
    p->setRemainingTime(MAXINT);

    _particles.push_back(p);

    auto wfg = new WhirlwindGenerator(0.1, 0.2, {0.0, 0.0, 0.0});
    //pfr->addRegistry(wfg, p);

    auto gfg = new GravityForceGenerator({ 0.0, -9.8, 0.0 });
    pfr->addRegistry(gfg, p);

    for (auto pa : _particles) {
        pa->setABF(true);
        pa->setVel({ 0.0, 0.0, 0.0 });
        pa->setAcc({ 0.0, 0.0 , 0.0 });
        pa->setMass(20.0f);
        pfr->addRegistry(wfg, pa);
    }
}

void ParticleSystem::generateParticles()
{
    std::default_random_engine rnd{ std::random_device{}() };
    std::uniform_real_distribution<float> interval(-20, 20);
    for (int i = 0; i < 50; ++i) {
        auto p = new Proyectile(ProyectileTypes::Bubble);


        p->setPosition({ interval(rnd), interval(rnd), interval(rnd)});
        p->setVel({ 0.0, 0.0, 0.0 });
        p->setAcc({ 0.0, 0.0 , 0.0 });
        p->setMass(20.0f);
        p->setDamping(0.99);
        p->setRemainingTime(MAXINT);

        _particles.push_back(p);
    }
}

void ParticleSystem::testExplosion()
{
    explosionActive = true;
    exp = new ExplosionGenerator(100.0, 150.0, {0.0, 0.0, 0.0});

    for (auto p : _particles) {
        p->setABF(true);
        pfr->addRegistry(exp, p);
    }
}

void ParticleSystem::testSpring()
{
    // Caja estática
    Particle* p1 = new Particle(BoxT, 4.0, { 10.0, 50.0, 0.0 });

    Particle* p2 = new Proyectile(BasicStatic, { 10.0, 49.0, 0.0 });
    p2->setMass(1.0);
    p2->setDamping(0.85);
    p2->setABF(true);

    _particles.push_back(p1);
    _particles.push_back(p2);
     
    sfg = new SpringForceGenerator(p1, 10, 20);
    pfr->addRegistry(sfg, p2);

    GravityForceGenerator* gfg = new GravityForceGenerator({0.0, -9.8, 0.0});
    pfr->addRegistry(gfg, p2);

    _forceGenerators.push_back(sfg);
    _forceGenerators.push_back(gfg);

    springActive = true;
}

void ParticleSystem::addKSpring()
{
    if (springActive) {
        sfg->setK(sfg->getK() + 10);
        cout << "Spring constant: " << sfg->getK() << "\n";
    }

    if (waterActive) {
        bfg->setVolume(bfg->getVolume() + 10);
        cout << "Object Volume: " << bfg->getVolume() << "\n";
    }
}

void ParticleSystem::subKSpring()
{
    if (springActive) {
        sfg->setK(sfg->getK() - 10);
        cout << "Spring constant: " << sfg->getK() << "\n";
    }

    if (waterActive) {
        bfg->setVolume(bfg->getVolume() - 10);
        cout << "Object Volume: " << bfg->getVolume() << "\n";
    }
}

void ParticleSystem::testSprings()
{
    // Importante! La separación inicial entre las partículas debe ser mayor que el l_0 que pasamos al generador del muelle
    Particle* p1 = new Proyectile(BasicStatic, { -20.0, 20.0, 0.0 });
    p1->setMass(2.0);
    p1->setDamping(0.85);
    p1->setABF(true);
    p1->setSemiImplicit(true);

    Particle* p2 = new Proyectile(BasicStatic, { 20.0, 20.0, 0.0 });
    p2->setMass(2.0);
    p2->setDamping(0.85);
    p2->setABF(true);
    p2->setColor({0.0, 1.0, 1.0, 1.0}); // cyan
    p2->setSemiImplicit(true);

    _particles.push_back(p1);
    _particles.push_back(p2);

    auto sfg1 = new SpringForceGenerator(p2, 10, 20, 0, 40);
    pfr->addRegistry(sfg1, p1);

    auto sfg2 = new SpringForceGenerator(p1, 10, 20, 0, 40);
    pfr->addRegistry(sfg2, p2);

    //GravityForceGenerator* gfg = new GravityForceGenerator({ 0.0, -9.8, 0.0 });
    //pfr->addRegistry(gfg, p1);
    //pfr->addRegistry(gfg, p2);

    _forceGenerators.push_back(sfg1);
    _forceGenerators.push_back(sfg2);
    //_forceGenerators.push_back(gfg);
}

void ParticleSystem::testRubberBand()
{
    Particle* p1 = new Proyectile(BasicStatic, { -20.0, 20.0, 0.0 });
    p1->setMass(2.0);
    p1->setDamping(0.85);
    p1->setABF(true);
    p1->setVel({-20.0, 0.0, 0.0});
    Particle* p2 = new Proyectile(BasicStatic, { 20.0, 20.0, 0.0 });
    p2->setMass(2.0);
    p2->setDamping(0.85);
    p2->setABF(true);
    p2->setVel({20.0, 0.0, 0.0});
    p2->setColor({ 0.0, 1.0, 1.0, 1.0 }); // cyan

    _particles.push_back(p1);
    _particles.push_back(p2);

    auto sfg1 = new RubberBandGenerator(p2, 15, 10);
    pfr->addRegistry(sfg1, p1);

    auto sfg2 = new RubberBandGenerator(p1, 15, 10);
    pfr->addRegistry(sfg2, p2);

    _forceGenerators.push_back(sfg1);
    _forceGenerators.push_back(sfg2);

    //GravityForceGenerator* gfg = new GravityForceGenerator({ 5.0, 0.0, 5.0 });
    //pfr->addRegistry(gfg, p1);
    //GravityForceGenerator* gfg2 = new GravityForceGenerator({ -5.0, 0.0, -5.0 });
    //pfr->addRegistry(gfg2, p2);
}

void ParticleSystem::testSlinky()
{
    Particle* p0 = new Proyectile(BasicStatic, { 20.0, 65.0, 20.0 });
    p0->setMass(2.0);
    p0->setDamping(0.85);
    p0->setABF(true);
    p0->setColor({ 0.0, 0.5, 1.0, 1.0 }); // blue
    Particle* p1 = new Proyectile(BasicStatic, { 20.0, 60.0, 20.0 });
    p1->setMass(2.0);
    p1->setDamping(0.85);
    p1->setABF(true);
    p1->setColor({ 0.0, 1.0, 0.0, 1.0 }); // green
    Particle* p2 = new Proyectile(BasicStatic, { 20.0, 55.0, 20.0 });
    p2->setMass(2.0);
    p2->setDamping(0.85);
    p2->setABF(true);
    p2->setColor({ 1.0, 1.0, 0.0, 1.0 }); // yellow
    Particle* p3 = new Proyectile(BasicStatic, { 20.0, 50.0, 20.0 });
    p3->setMass(2.0);
    p3->setDamping(0.85);
    p3->setABF(true);
    p3->setColor({ 1.0, 0.5, 0.0, 1.0 }); // orange
    Particle* p4 = new Proyectile(BasicStatic, { 20.0, 45.0, 20.0 });
    p4->setMass(2.0);
    p4->setDamping(0.85);
    p4->setABF(true);
    p4->setColor({ 1.0, 0.0, 0.0, 1.0 }); // red

    _particles.push_back(p0);
    _particles.push_back(p1);
    _particles.push_back(p2);
    _particles.push_back(p3);
    _particles.push_back(p4);

    // Caja estática
    Particle* box = new Particle(BoxT, 2.0, { 20.0, 70.0, 20.0 });

    SpringForceGenerator* base = new SpringForceGenerator(box, 65, 6);
    pfr->addRegistry(base, p0);

    auto sfg0 = new SpringForceGenerator(p0, 60, 6);
    pfr->addRegistry(sfg0, p1);

    auto sfg4 = new SpringForceGenerator(p1, 55, 6);
    pfr->addRegistry(sfg4, p0);

    auto sfg1 = new SpringForceGenerator(p1, 50, 6);
    pfr->addRegistry(sfg1, p2);

    auto sfg5 = new SpringForceGenerator(p2, 45, 6);
    pfr->addRegistry(sfg5, p1);

    auto sfg2 = new SpringForceGenerator(p2, 40, 6);
    pfr->addRegistry(sfg2, p3);

    auto sfg6 = new SpringForceGenerator(p3, 35, 6);
    pfr->addRegistry(sfg6, p2);

    auto sfg3 = new SpringForceGenerator(p3, 30, 6);
    pfr->addRegistry(sfg3, p4);

    auto sfg7 = new SpringForceGenerator(p4, 25, 6);
    pfr->addRegistry(sfg7, p3);

    GravityForceGenerator* gfg = new GravityForceGenerator({ 0.0, -9.8, 0.0 });
    pfr->addRegistry(gfg, p0);
    pfr->addRegistry(gfg, p1);
    pfr->addRegistry(gfg, p2);
    pfr->addRegistry(gfg, p3);
    pfr->addRegistry(gfg, p4);

    _forceGenerators.push_back(sfg0);
    _forceGenerators.push_back(sfg1);
    _forceGenerators.push_back(sfg2);
    _forceGenerators.push_back(sfg3);
    _forceGenerators.push_back(gfg);
}

void ParticleSystem::testFloat()
{
    Particle* water = new Particle(ParticleTypes::WaterT, 20.0, { 10.0, 40.0, 0.0 });
    box = new Particle(BoxT, 3.0, { 10.0, 40.0, 0.0 });
    box->setABF(true);
    box->setDamping(0.85);
    box->setMass(2.0);
    box->setColor({ 0.0, 1.0, 1.0, 0.0 });

    _particles.push_back(water);
    _particles.push_back(box);

    // Si la masa supera el volumen, el objeto se hunde!
    bfg = new BuoyancyForceGenerator(1, 15.0, 10, water); // altura de box, volumen de box, densidad del agua
    pfr->addRegistry(bfg, box);

    GravityForceGenerator* gfg = new GravityForceGenerator({ 0.0, -9.8, 0.0 });
    pfr->addRegistry(gfg, box);

    _forceGenerators.push_back(bfg);
    _forceGenerators.push_back(gfg);

    waterActive = true;
}

void ParticleSystem::addMass() // añade 1/2 kg
{
    if (waterActive) {
        box->setMass(box->getMass() + 0.5);
        cout << "Box Mass: " << box->getMass() << "\n";
    }
}

void ParticleSystem::subMass() // resta 1/2 kg
{
    if (waterActive) {
        box->setMass(box->getMass() - 0.5);
        cout << "Box Mass: " << box->getMass() << "\n";
    }
}
