#include "Particle.h"

Particle::Particle(Vector3 p, Vector3 v, Vector3 a, float d) : pose(p), vel(v)
{
	// pose.q = { 0.0, 0.0, 0.0, 1.0 }; -> en caso de que diese error el q.
	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(radius)), &pose, color);
	damping = d;
	acceleration = a;
}

Particle::~Particle()
{
	delete renderItem;
}

void Particle::integrate(double t)
{
	vel = (vel * pow(damping, t)) + (acceleration*t);
	pose.p = pose.p + vel * t;

}
