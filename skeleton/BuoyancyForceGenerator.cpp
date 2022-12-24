#include "BuoyancyForceGenerator.h"
#include <iostream>

BuoyancyForceGenerator::BuoyancyForceGenerator(float h, float V, float d, Particle* lp) : ForceGenerator()
{
	_height = h; _volume = V; _liquid_density = d; _liquid_particle = lp;
}

void BuoyancyForceGenerator::updateForce(Particle* p, double t)
{
	float h = p->getPose().p.y;
	float h0 = _liquid_particle->getPose().p.y;

	Vector3 F(0, 0, 0);
	float immersed = 0.0;
	if (h - h0 > _height * 0.5) {
		//Fuera del agua
		immersed = 0.0;
	}
	else if (h0 - h > _height * 0.5) {
		//Totalmente hundido
		immersed = 1.0;
	}
	else {
		immersed = (h0 - h) / _height + 0.5;
	}

	F.y = _liquid_density * _volume * immersed; // Cuanto mayor sea el volumen, más "rebota" el objeto en el agua

	/*if (F.y != 0)
		std::cout << "Buoyancy force: " << F.x << "\t" << F.y << "\t" << F.z << "\n";*/

	p->addForce(F);
}

void BuoyancyForceGenerator::updateForceRB(physx::PxRigidDynamic* rb, double t)
{
	float h = rb->getGlobalPose().p.y;
	float h0 = _liquid_particle->getPose().p.y;

	Vector3 F(0, 0, 0);
	float immersed = 0.0;
	if (h - h0 > _height * 0.5) {
		//Fuera del agua
		immersed = 0.0;
	}
	else if (h0 - h > _height * 0.5) {
		//Totalmente hundido
		immersed = 1.0;
	}
	else {
		immersed = (h0 - h) / _height + 0.5;
	}

	F.y = _liquid_density * _volume * immersed; // Cuanto mayor sea el volumen, más "rebota" el objeto en el agua

	/*if (F.y != 0)
		std::cout << "Buoyancy force: " << F.x << "\t" << F.y << "\t" << F.z << "\n";*/

	rb->addForce(F);
}

BuoyancyForceGenerator::~BuoyancyForceGenerator()
{
	if (_delete_particle) delete _liquid_particle;
}
