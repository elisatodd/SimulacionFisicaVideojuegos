#include "Proyectile.h"

Particle* Proyectile::clone() const
{
	auto a = new Proyectile(myType);
	a->setPosition(pose.p);
	a->setVel(vel);
	a->setAcc(acceleration);
	a->setDamping(damping);
	return a;
}
