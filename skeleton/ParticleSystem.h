#pragma once
#include <iostream>
#include <list>
#include "Proyectile.h"
#include "ParticleGenerator.h"
#include "ParticleForceRegistry.h"
#include "ExplosionGenerator.h"
#include "SpringForceGenerator.h"
#include "BuoyancyForceGenerator.h"

class Firework;

using namespace std;

class ParticleSystem
{
public:

	ParticleSystem();
	~ParticleSystem();

	void addParticleGenerator(string name, ProyectileTypes pT, GeneratorTypes gT,
		Vector3 mediavel, Vector3 mediapos,
		Vector3 anchovel, Vector3 anchopos,
		double genProb, int numPart, double time);

	void addParticleGenerator(shared_ptr<ParticleGenerator> pG);
	void addParticle(Particle* p);

	void removeParticleGenerator(string name);
	void removeAllParticleGenerators();

	void deactivateAllParticleGenerators();

	void update(double t);

	ParticleGenerator* getParticleGenerator(string name); // devuelve el P.Gen correspondiente a name

	void generateFireworkSystem();

	void onParticleDeath(Particle* p);
	void shootFirework(int type);

	// FUERZAS
		// gravitatoria
	void addBubbles();
	void addDifferentABF();
		// viento
	void testWind();
		// torbellino
	void testWhirlwind();
		// explosión
	void generateParticles();
	void testExplosion();

	// MUELLES
	void testSpring();
	void addKSpring();
	void subKSpring();

	void testSprings();

	void testRubberBand();

	void testSlinky();

	void testFloat();
	void addMass();
	void subMass();

private:
	std::list<Particle*> _particles;
	//std::list<ParticleGenerator*> _particle_generators;
	std::list<shared_ptr<ParticleGenerator>> _particle_generators;

	std::vector<Firework*> _firework_pool;

	ParticleForceRegistry* pfr;
	std::list<ForceGenerator*> _forceGenerators;
	
	ExplosionGenerator* exp;
	bool explosionActive = false;

	SpringForceGenerator* sfg;
	bool springActive = false;

	BuoyancyForceGenerator* bfg;
	bool waterActive = false;
	Particle* box = nullptr;
};

