#include "RubberBandGenerator.h"

void RubberBandGenerator::updateForce(Particle* p, double t)
{
	/*
	* la fuerza del muelle s�lo se aplica cuando la distancia entre ambas part�culas
	* supera la distancia de reposo (la goma se estira), pero no cuando es menor (la goma se encoje).
	*/
	Vector3 F = _other->getPose().p - p->getPose().p;

	// longitud
	const float l = F.normalize();
	// variaci�n de X
	const float delta_x = l - _lo;
	// calculamos la fuerza
	F *= delta_x * _K;

	if (l > _lo) {
		p->addForce(F);
	}
}
