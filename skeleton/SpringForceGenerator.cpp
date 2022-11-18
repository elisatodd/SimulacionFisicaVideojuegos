#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(Vector3 pos, double K, double lo)
{
	_K = K; _lo = lo;
}

SpringForceGenerator::SpringForceGenerator(Particle* p, double K, double lo)
{
	_other = p; _K = K; _lo = lo;
}

void SpringForceGenerator::updateForce(Particle* p, double t)
{
	Vector3 F = _other->getPose().p - p->getPose().p;

	// longitud
	const float l = F.normalize();
	// variación de X
	const float delta_x = l - _lo;
	// calculamos la fuerza
	F *= delta_x * _K;

	p->addForce(F);
}
