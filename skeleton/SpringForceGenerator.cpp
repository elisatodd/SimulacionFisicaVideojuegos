#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(Vector3 pos, double K, double lo)
{
	_K = K; _lo = lo;
	_other = new Particle(BoxT, 4.0, { 10.0, 50.0, 0.0 });
	_deleteOther = true;
}

SpringForceGenerator::SpringForceGenerator(Particle* p, double K, double lo)
{
	_other = p; _K = K; _lo = lo;
	_lmax = 40.0; _lmin = 0.0;
}

SpringForceGenerator::SpringForceGenerator(Particle* p, double K, double lo, double lmin, double lmax)
{
	_other = p; _K = K; _lo = lo;
	_lmax = lmax; _lmin = lmin;
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

	if (l <= _lmin || l >= _lmax) {
		// Cuando el muelle llega a su límite, rebota al otro lado
		p->setVel(-p->getVel());
	}
	p->addForce(F);

}
