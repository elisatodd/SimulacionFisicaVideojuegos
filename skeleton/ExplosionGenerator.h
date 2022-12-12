#pragma once
#include "ForceGenerator.h"
#include "core.hpp"

class ExplosionGenerator : public ForceGenerator
{
public:
	ExplosionGenerator(float radio, float intensidad, Vector3 centro) : 
	_R(radio), _K(intensidad), _centre(centro), _const(0.0) {};

	virtual void updateForce(Particle* p, double t);
	virtual void updateForceRB(physx::PxRigidDynamic* rb, double t);

	inline void addConst(double t) 
	{ 
		if (_enabled) 
			_const += t; 
	};

	inline void changeState() { _enabled = !_enabled; }

protected:
	float _R; // radio de la explosión 
	float _K; // intensidad 
	Vector3 _centre; // centro
	float _const; // constante de tiempo -> a partir de 4 𝜏 la explosión prácticamente se ha desvanecido

	bool _enabled = false;
};

