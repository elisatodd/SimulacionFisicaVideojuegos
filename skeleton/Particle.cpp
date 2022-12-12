#include "Particle.h"
#include "Proyectile.h"

#include <iostream>

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
	
		renderItem = new RenderItem(CreateShape(physx::PxBoxGeometry(r, r, 0.5)), &pose, { 1.0, 0.0, 0.0, 1.0 });

		break;

	case (FloorT):

		renderItem = new RenderItem(CreateShape(physx::PxBoxGeometry(r, 1.0, r)), &pose, { 1.0, 1.0, 1.0, 1.0 });

		break;

	case (FireworkT):

		renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(radius)), &pose, {1.0, 1.0, 0.0, 1.0});
		break;

	case (BoxT):

		renderItem = new RenderItem(CreateShape(physx::PxBoxGeometry(r, r, r)), &pose, { 0.6, 1.0, 0.0, 1.0 });
		break;

	case (WaterT):
		renderItem = new RenderItem(CreateShape(physx::PxBoxGeometry(r, 0.5, r)), &pose, { 0.0, 0.2, 0.8, 1.0 });
	}
	// las particulas tienen tiempo de vida infinito por deefcto
	setRemainingTime(MAXINT);
}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
	delete renderItem;
}

bool Particle::integrate(double t)
{
	if (affectedByForces) {

		float inverseMass = getInvMass();

		// Trivial case, infinite mass --> do nothing
		if (inverseMass > 0.0f) {

			// Update position
			if (!semiImplicit)
				pose.p += vel * t;

			Vector3 totalAcceleration = acceleration;
			totalAcceleration += (force * inverseMass);
			/*std::cout << "Force in y : " << force.y << std::endl;
			std::cout << "Acc in y : " << acceleration.y << std::endl;
			std::cout << "Inverse of mass: " << inverseMass << std::endl;
			std::cout << "Total Acceleration in y: " << totalAcceleration.y << std::endl;*/

			// Update linear velocity
			vel += (totalAcceleration * t);

			// Impose drag (damping)
			vel *= powf(damping, t);

			// Update position
			if (semiImplicit)
				pose.p += vel * t;

			//std::cout << totalAcceleration.x << totalAcceleration.y << totalAcceleration.z << std::endl;
			clearForce();
		};
	}
	else {

		// Versión anterior a las fuerzas
		vel = (vel * pow(damping, t)) + (acceleration*t);
		pose.p = pose.p + vel * t;
	}

	if (remainingTime > 0) remainingTime -= t;

	if (getTime() <= 0 || getPose().p.y <= -100.0 || getPose().p.y >= 1000.0) alive = false;

	return alive;
}

Particle* Particle::clone() const
{
	auto a = new Particle(myPType, radius, pose.p, vel, acceleration, damping);
	return a;
	
}

void Particle::clearForce() {
	force = Vector3{ 0.0, 0.0, 0.0 };
}

void Particle::addForce(const Vector3& f) {
	force += f;
}