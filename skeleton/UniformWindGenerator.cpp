#include "UniformWindGenerator.h"
#include <iostream>

UniformWindGenerator::UniformWindGenerator(float k1, float k2) : DragGenerator(k1, k2)
{
}

UniformWindGenerator::UniformWindGenerator(float k1, float k2, Vector3 wind) : DragGenerator(k1, k2), _wind(wind)
{
}

UniformWindGenerator::UniformWindGenerator(float k1, float k2, Vector3 wind, Vector3 ori, double w, double h, double d) : DragGenerator(k1, k2), 
_wind(wind), areaX(w), areaY(h), areaZ(d), origin(ori)
{
}

void UniformWindGenerator::updateForce(Particle* p, double t)
{
	// Check that the particle has finite mass
	if (fabs(p->getInvMass()) < 1e-10)
		return;

	// Check if the particle is inside the mass of wind
	auto pos = p->getPose().p;
	if (pos.x > origin.x+areaX || pos.x < origin.x-areaX || 
		pos.y > origin.y+areaY || pos.y < origin.y-areaY || 
		pos.z > origin.y+areaZ || pos.z < origin.y-areaZ)
		return;	

	// Compute the wind force == drag force + wind
	Vector3 v = p->getVel() - _wind; // lo mismo que drag pero ahora sumamos el viento
	float dragCoef = v.normalize();
	Vector3 windForce;
	dragCoef = _k1 * dragCoef + _k2 * dragCoef * dragCoef;
	windForce = -v * dragCoef;

	// apply the wind force
	std::cout << "Wind force: " << windForce.x << "\t" << windForce.y << "\t" << windForce.z << "\n";
	//std::cout << "Particle position: " << p->getPose().p.x << "\t" << p->getPose().p.y << "\t" << p->getPose().p.z << "\n";
	p->addForce(windForce);

}

void UniformWindGenerator::updateForceRB(physx::PxRigidDynamic* rb, double t)
{
	// Check that the RB has finite mass
	if (fabs(rb->getInvMass()) < 1e-10)
		return;

	// Check if the particle is inside the mass of wind
	auto pos = rb->getGlobalPose().p;
	if (pos.x > origin.x + areaX || pos.x < origin.x - areaX ||
		pos.y > origin.y + areaY || pos.y < origin.y - areaY ||
		pos.z > origin.z + areaZ || pos.z < origin.z - areaZ)
		return;

	// Compute the wind force == drag force + wind
	Vector3 v = rb->getLinearVelocity() - _wind; // lo mismo que drag pero ahora sumamos el viento
	float dragCoef = v.normalize();
	Vector3 windForce;
	dragCoef = _k1 * dragCoef + _k2 * dragCoef * dragCoef;
	windForce = -v * dragCoef;

	// apply the wind force
	std::cout << "Wind force: " << windForce.x << "\t" << windForce.y << "\t" << windForce.z << "\n";
	//std::cout << "Particle position: " << p->getPose().p.x << "\t" << p->getPose().p.y << "\t" << p->getPose().p.z << "\n";
	rb->addForce(windForce);
}
