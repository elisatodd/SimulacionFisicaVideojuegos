#pragma once
#include "ForceGenerator.h"
#include "Particle.h"
#include "core.hpp"

class BuoyancyForceGenerator : public ForceGenerator {
public:
	BuoyancyForceGenerator(float h, float V, float d, Particle* lp);
	BuoyancyForceGenerator(float d, Particle* lp);

	void updateForce(Particle* p, double t);
	virtual void updateForceRB(physx::PxRigidDynamic* rb, double t);

	inline void setVolume(float v) { _volume = v; };
	inline float getVolume() { return _volume; };

	virtual ~BuoyancyForceGenerator();

protected:
	float _height;
	float _volume;
	float _liquid_density;
	float _gravity = 9.8;

	Particle* _liquid_particle; // Para la representación
	bool _delete_particle = true;
};

