#pragma once
#include <list>
#include "core.hpp"
#include "Particle.h"
#include "ParticleGenerator.h"
#include <string>

class GaussianParticleGenerator : public ParticleGenerator {

public:

	GaussianParticleGenerator(std::string name, Vector3 mPos, Vector3 mVel, 
								double genProb, int numPart, Particle* m, 
								Vector3 vel, Vector3 pos, double time) :
		ParticleGenerator(name, mPos, mVel, genProb, numPart, m), _std_dev_vel(vel), _std_dev_pos(pos), std_dev_t(time) {

	};

	virtual list<Particle*> generateParticles() override;


private:

	Vector3 _std_dev_vel, _std_dev_pos;
	double std_dev_t;

};