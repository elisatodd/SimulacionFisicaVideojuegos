#pragma once
#include <iostream>
#include <list>
#include "core.hpp"
#include "Particle.h"
#include <string>

using namespace std;

enum GeneratorTypes {
	uniform,
	normal,
	firework
};

class ParticleGenerator
{
public:

	ParticleGenerator(string name, Vector3 mPos, Vector3 mVel, double genProb, int numPart, Particle* m);
	~ParticleGenerator();

	void setParticle(Particle* model);

	virtual std::list<Particle*> generateParticles() = 0;

	std::string getGenName() { return _name; }

	inline void setMeanPos(Vector3 p) { _mean_pos = p; };

	inline void setMeanVel(Vector3 v) { _mean_vel = v; };

protected:
	string _name;
	Vector3 _mean_pos, _mean_vel;
	double _generation_probability;
	int _num_particles;
	Particle* _model;
};

