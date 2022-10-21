#pragma once
#include <list>
#include <memory>
#include "Particle.h"
#include "ParticleGenerator.h"

using namespace std;

class Firework : public Particle
{
public:

	Firework(int gens);
	~Firework();

	//int integrate(double t);

	virtual Particle* clone() const;

	list<Particle*> explode();
private:

	int _generationsLeft; // explosiones que quedan
	int _nFireworks;// particulas generadas en cada explosion

	list<shared_ptr<ParticleGenerator>> _gens;

};

