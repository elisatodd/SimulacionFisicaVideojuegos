#pragma once
#include "Particle.h"
#include "SpringForceGenerator.h"

class RubberBandGenerator : public SpringForceGenerator
{
public:
	RubberBandGenerator(Particle* p, double K, double lo) : SpringForceGenerator(p, K, lo)
	{};

	void updateForce(Particle* p, double t);
};

