#pragma once
#include "DragGenerator.h"

class UniformWindGenerator : public DragGenerator
{
public:

	UniformWindGenerator(float k1, float k2);
	UniformWindGenerator(float k1, float k2, Vector3 wind);
	UniformWindGenerator(float k1, float k2, Vector3 wind, Vector3 ori, double w, double h, double d);

	virtual void updateForce(Particle* p, double t);

protected:
	Vector3 _wind;

	// Area
	double areaX, areaY, areaZ;
	Vector3 origin;
};

