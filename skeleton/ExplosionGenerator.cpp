#include <iostream>
#include "ExplosionGenerator.h"

#define e 2.71828182846

void ExplosionGenerator::updateForce(Particle* p, double t)
{
	// Check that the particle has finite mass
	if (fabs(p->getInvMass()) < 1e-10)
		return;
	
	auto pos = p->getPose().p;

	// distancia de la explosion
	double r = sqrt(pow((pos.x - _centre.x), 2) + pow((pos.y - _centre.y), 2) + pow((pos.z - _centre.z), 2));
	Vector3 forceDir = { 0, 0, 0 };

	if (r < _R) {
		double first = _K / pow(r, 2);
		double power = -(t / _const);
		double second = pow(e, power);

		forceDir = first * Vector3(pos.x - _centre.x, pos.y - _centre.y, pos.z - _centre.z) * second;
	}

	Vector3 v = p->getVel() - forceDir;
	float coef = v.normalize();
	Vector3 expForce;
	coef = (_K * coef) + _K * coef * coef;
	expForce = -v * coef;

	// apply the force
	std::cout << "Explosion force: " << expForce.x << "\t" << expForce.y << "\t" << expForce.z << "\n";
	//std::cout << "Particle position: " << p->getPose().p.x << "\t" << p->getPose().p.y << "\t" << p->getPose().p.z << "\n";
	p->addForce(expForce);
}