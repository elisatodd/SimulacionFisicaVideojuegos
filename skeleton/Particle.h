#pragma once
#include <PxPhysicsAPI.h>
#include "core.hpp" // para Vector3
#include "RenderUtils.hpp" // para RenderItem

class Particle
{
public:

	Particle(Vector3 pos, Vector3 vel);
	~Particle();

	void integrate(double t);

private:

	Vector3 vel;
	physx::PxTransform pose;
	RenderItem* renderItem;
	float radius = 5.0f;
	Vector4 color = Vector4(0.98, 0.58, 1.0, 1.0);
};

