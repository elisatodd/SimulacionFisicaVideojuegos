#include "GravityForceGenerator.h"
#include <iostream>

void GravityForceGenerator::updateForce(Particle* p, double t) {
	// Check that the mass is finite
	if (fabs(p->getInvMass()) < 1e-10)
		return;

	/*auto f = _gravity * p->getMass();
	std::cout << "Gravity force: " << f.x << "\t" << f.y << "\t" << f.z << "\n";
	std::cout << "Particle position: " << p->getPose().p.x << "\t" << p->getPose().p.y << "\t" << p->getPose().p.z << "\n";*/

	//Apply the mass scaled gravity
	p->addForce(_gravity * p->getMass());	
}