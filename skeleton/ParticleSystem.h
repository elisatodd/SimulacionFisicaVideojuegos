#pragma once
#include <iostream>
#include <list>
#include "Proyectile.h"
#include "ParticleGenerator.h"

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

	void removeParticleGenerator(string name);
	void removeAllParticleGenerators();

	void update(double t);

	ParticleGenerator* getParticleGenerator(string name); // devuelve el P.Gen correspondiente a name

	void generateFireworkSystem();

	void onParticleDeath(Particle* p);
	void shootFirework(int type);

private:
	std::list<Particle*> _particles;
	//std::list<ParticleGenerator*> _particle_generators;
	std::list<shared_ptr<ParticleGenerator>> _particle_generators;

	std::vector<Firework*> _firework_pool;
};

