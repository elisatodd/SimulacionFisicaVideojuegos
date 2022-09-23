#pragma once
#include <PxPhysicsAPI.h>
#include "core.hpp" // para Vector3
#include "RenderUtils.hpp" // para RenderItem

class Particle
{
public:

	Particle(Vector3 pos, Vector3 vel, Vector3 a, float d);
	~Particle();

	void integrate(double t);

private:

	Vector3 vel;
	physx::PxTransform pose;
	RenderItem* renderItem;
	float radius = 5.0f;
	Vector4 color = Vector4(0.98, 0.58, 1.0, 1.0);

	Vector3 acceleration;
	float damping; // must be a value between 0 and 1: 0 = max. friction 1 = min. friction

};

