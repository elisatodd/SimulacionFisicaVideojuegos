#include "RubberBandGenerator.h"

void RubberBandGenerator::updateForce(Particle* p, double t)
{
	/*
	* la fuerza del muelle sólo se aplica cuando la distancia entre ambas partículas
	* supera la distancia de reposo (la goma se estira), pero no cuando es menor (la goma se encoje).
	*/
	Vector3 F = _other->getPose().p - p->getPose().p;

	// longitud
	const float l = F.normalize();
	// variación de X
	const float delta_x = l - _lo;
	// calculamos la fuerza
	F *= delta_x * _K;

	if (l > _lo) {
		p->addForce(F);
	}
}
