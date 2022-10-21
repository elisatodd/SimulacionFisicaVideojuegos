#pragma once
#include <list>
#include "core.hpp"
#include "Particle.h"
#include "ParticleGenerator.h"
#include <string>

class UniformParticleGenerator : public ParticleGenerator {

public:

	UniformParticleGenerator(std::string name, Vector3 mPos, Vector3 mVel, double genProb, int numPart, Particle* m, Vector3 vel, Vector3 pos) : 
		ParticleGenerator(name, mPos, mVel, genProb, numPart, m), _vel_width(vel), _pos_width(pos) {

	};

	virtual list<Particle*> generateParticles() override;


private:

	Vector3 _vel_width, _pos_width;
};

