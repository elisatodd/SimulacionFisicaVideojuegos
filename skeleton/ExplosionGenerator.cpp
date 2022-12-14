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

void ExplosionGenerator::updateForceRB(physx::PxRigidDynamic* rb, double t)
{
	if (!_enabled)
		return;

	/*
		OTRA FORMA DE CALCULAR LA EXPLOSIÓN
	
	const double euler = std::exp(1.0);
	auto pos = rb->getGlobalPose().p;
	auto difX = pos.x - _centre.x;
	auto difY = pos.y - _centre.y;
	auto difZ = pos.z - _centre.z;

	auto r2 = pow(difX, 2) + pow(difY, 2) + pow(difZ, 2);

	if (r2 > pow(_R, 2))
	{
		return;
	}
		
	auto x = (_K / r2) * difX * pow(euler, (-t / _const));
	auto y = (_K / r2) * difY * pow(euler, (-t / _const));
	auto z = (_K / r2) * difZ * pow(euler, (-t / _const));

	Vector3 force(x, y, z);*/

	auto pos = rb->getGlobalPose().p;

	// distancia de la explosion
	double r = sqrt(pow((pos.x - _centre.x), 2) + pow((pos.y - _centre.y), 2) + pow((pos.z - _centre.z), 2));
	Vector3 forceDir = { 0, 0, 0 };

	if (r < _R) {
		double first = _K / pow(r, 2);
		double power = -(t / _const);
		double second = pow(e, power);

		forceDir = first * Vector3(pos.x - _centre.x, pos.y - _centre.y, pos.z - _centre.z) * second;
	}

	Vector3 v = rb->getAngularVelocity() - forceDir;
	float coef = v.normalize();
	Vector3 expForce;
	coef = (_K * coef) + _K * coef * coef;
	expForce = -v * coef;

	std::cout << "Explosion force: " << expForce.x << "\t" << expForce.y << "\t" << expForce.z << "\n";

	rb->addForce(expForce);
}
