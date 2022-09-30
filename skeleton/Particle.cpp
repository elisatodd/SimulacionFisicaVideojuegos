#include "Particle.h"

Particle::Particle(ParticleTypes t, float r, Vector3 p, Vector3 v, Vector3 a, float d) : 
	myPType(t), pose(p), vel(v), radius(r), damping(d), acceleration(a)
{
	// pose.q = { 0.0, 0.0, 0.0, 1.0 }; -> en caso de que diese error el q.

	switch (myPType) {
	case (ProyectileT):
		// Hago esto en la clase proyectil -> para que cada uno pueda elegir su forma y tamaño
		// renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(radius)), &pose, color);
	break;

	case (TargetT):
	
		//Vector4 red = {1.0, 0.0, 0.0, 1.0};
		//renderItem = new RenderItem(CreateShape(physx::P), &pose, red);
		break;

	case (FloorT):
		//Vector4 blue = { 0.0, 0.0, 1.0, 1.0 };

		renderItem = new RenderItem(CreateShape(physx::PxPlaneGeometry()), &pose, { 0.0, 0.0, 1.0, 1.0 });

		break;

	}
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
