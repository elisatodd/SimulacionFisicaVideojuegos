#pragma once
#include "Particle.h"
#include "ForceGenerator.h"

class SpringForceGenerator : public ForceGenerator
{
public:
	// 2 constructores:

	// uno con la posición donde está fijo el muelle. Crea una partícula en el sitio que nos digan.
	SpringForceGenerator(Vector3 pos, double K, double lo);

	// tenemos la partícula, en lugar del punto
	SpringForceGenerator(Particle* p, double K, double lo);
	SpringForceGenerator(Particle* p, double K, double lo, double lmin, double lmax);

	void updateForce(Particle* p, double t);

	inline void setK(double K) { _K = K; }
	inline double getK() { return _K; }

	virtual ~SpringForceGenerator() { if (_deleteOther) delete _other; }

protected:
	double _K; // elastic coef.
	double _lo; // l sub 0 -> resting lenght

	double _lmin;
	double _lmax;

	bool _deleteOther = false;

	Particle * _other;
};

