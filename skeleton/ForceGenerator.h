#pragma once
#include <list>
#include <random>
#include "Particle.h"

class ForceGenerator
{
public:
	virtual void updateForce(Particle* p, double duration) = 0;

	std::string _name;

	// if start negative -> eternal
	double _t = pow(-1, 10); //= -1^10
};
