#include <iostream>
#include "WhirlwindGenerator.h"

void WhirlwindGenerator::updateForce(Particle* p, double t)
{
	// Check that the particle has finite mass
	if (fabs(p->getInvMass()) < 1e-10)
		return;

	auto pos = p->getPose().p;
	
	//// Check if the particle is inside the mass of wind
	//if (pos.x > origin.x + areaX || pos.x < origin.x - areaX ||
	//	pos.y > origin.y + areaY || pos.y < origin.y - areaY ||
	//	pos.z > origin.y + areaZ || pos.z < origin.y - areaZ)
	//	return;

	// Compute the wind force == drag force + wind
	Vector3 v = p->getVel() - (K * Vector3(
								-(pos.z - origin.z) - 0.9*(pos.x - origin.x),
								20 - (pos.y - origin.y),
								(pos.x - origin.x) - 0.9*(pos.z - origin.z)) );

	//Vector3 v = p->getVel() - (K * Vector3(
	//	-(pos.z - origin.z),
	//	20 - (pos.y - origin.y),
	//	(pos.x - origin.x)));

	float dragCoef = v.normalize();
	Vector3 windForce;
	dragCoef = _k1 * dragCoef + _k2 * dragCoef * dragCoef;
	windForce = -v * dragCoef;

	// apply the wind force
	std::cout << "Wind force: " << windForce.x << "\t" << windForce.y << "\t" << windForce.z << "\n";
	//std::cout << "Particle position: " << p->getPose().p.x << "\t" << p->getPose().p.y << "\t" << p->getPose().p.z << "\n";
	p->addForce(windForce);

}
