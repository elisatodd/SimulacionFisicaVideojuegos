#include "DragGenerator.h"
#include <iostream>

void DragGenerator::updateForce(Particle* p, double t)
{
	// Check that the particle has finite mass
	if (fabs(p->getInvMass()) < 1e-10)
		return;

	// Compute the drag force
	Vector3 v = p->getVel();
	float dragCoef = v.normalize();
	Vector3 dragForce;
	dragCoef = _k1 * dragCoef + _k2 * dragCoef * dragCoef;
	dragForce = -v * dragCoef;

	// apply the drag force
	std::cout << "Drag force: " << dragForce.x << "\t" << dragForce.y << "\t" << dragForce.z << "\n";
	//std::cout << "Particle position: " << p->getPose().p.x << "\t" << p->getPose().p.y << "\t" << p->getPose().p.z << "\n";
	p->addForce(dragForce);
}
