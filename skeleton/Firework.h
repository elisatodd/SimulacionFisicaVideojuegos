#pragma once
#include <list>
#include <memory>
#include "Particle.h"
#include "ParticleGenerator.h"

using namespace std;

class Firework : public Particle
{
public:

	Firework(Vector3 pos, Vector3 dir, Vector3 acc, double dam, list<shared_ptr<ParticleGenerator>> lG, float radius);
	

	~Firework();

	//int integrate(double t);

	virtual Particle* clone() const;

	list<Particle*> explode();
private:

	list<shared_ptr<ParticleGenerator>> _gens;

};

