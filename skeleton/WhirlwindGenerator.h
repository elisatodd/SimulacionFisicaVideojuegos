#pragma once
#include "UniformWindGenerator.h"

class WhirlwindGenerator : public UniformWindGenerator
{
public:

	WhirlwindGenerator(float k1, float k2, Vector3 ori) : UniformWindGenerator(k1, k2) {
	
		K = 5.0f;
		origin = ori;
	};
	
	virtual void updateForce(Particle* p, double t);


protected:
	float K; // constante proporcional a la fuerza del torbellino
	Vector3 origin;
};

