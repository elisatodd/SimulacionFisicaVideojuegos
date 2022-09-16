#include "Particle.h"

Particle::Particle(Vector3 p, Vector3 v) : pose(p), vel(v)
{
	// pose.q = { 0.0, 0.0, 0.0, 1.0 }; -> en caso de que diese error el q.
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(radius)), &pose, color);
}

Particle::~Particle()
{
	delete renderItem;
}

void Particle::integrate(double t)
{
	pose.p = pose.p + vel * t;
}
