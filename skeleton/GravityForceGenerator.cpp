#include "GravityForceGenerator.h"

void GravityForceGenerator::updateForce(Particle* p, double t) {
	// Check that the mass is finite
	if (fabs(p->getInvMass()) < 1e-10)
		return;

	//Apply the mass scaled gravity
	p->addForce(_gravity * p->getMass());	
}