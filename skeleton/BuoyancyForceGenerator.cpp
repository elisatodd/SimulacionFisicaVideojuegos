#include "BuoyancyForceGenerator.h"
#include <iostream>

#define pi  3.1415926535

BuoyancyForceGenerator::BuoyancyForceGenerator(float h, float V, float d, Particle* lp) : ForceGenerator()
{
	_height = h; _volume = V; _liquid_density = d; _liquid_particle = lp; _name = "Buoyancy";
}

BuoyancyForceGenerator::BuoyancyForceGenerator(float d, Particle* lp)
{
	_height = 0.0; _volume = 0.0; _liquid_density = d; _liquid_particle = lp; _name = "Buoyancy";
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

	// Miro la forma que hay asociada a este RB
	physx::PxShape* s;
	physx::PxU32 u = rb->getNbShapes();
	rb->getShapes(&s, u);

	// Miro su tamaño para calcular altura y densidad

	auto type = s->getGeometryType();
	switch (type) {
	case physx::PxGeometryType::eBOX:
	{
		physx::PxBoxGeometry box;
		s->getBoxGeometry(box);
		_height = box.halfExtents.y * 2;
		// volumen = l * w * h
		_volume = (box.halfExtents.x * 2) * _height * (box.halfExtents.z * 2);

		break;
	}
	case physx::PxGeometryType::eSPHERE:
	{
		physx::PxSphereGeometry sphere;
		s->getSphereGeometry(sphere);
		_height = sphere.radius * 2;
		// volumen = 4/3 * pi * radio^3
		_volume = (4 / 3) * pi * pow((_height / 2), 3);
		break;
	}
		
	}

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

	rb->addForce(F);


	/*if (F.y != 0)
		std::cout << "Buoyancy force: " << F.x << "\t" << F.y << "\t" << F.z << "\n";*/

}

BuoyancyForceGenerator::~BuoyancyForceGenerator()
{
	if (_delete_particle) delete _liquid_particle;
}
