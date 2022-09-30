#pragma once
#include <PxPhysicsAPI.h>
#include "core.hpp" // para Vector3
#include "RenderUtils.hpp" // para RenderItem

enum ParticleTypes {
	ProyectileT,
	TargetT,
	FloorT
};

class Particle
{
public:

	Particle() {};
	Particle(ParticleTypes t, float r = 0.0f, 
		Vector3 pos = {0.0, 0.0, 0.0}, Vector3 vel = {0.0, 0.0, 0.0},
		Vector3 a = {0.0, 0.0, 0.0}, float d = 0.0f);

	~Particle();

	void integrate(double t);

	void setPosition(Vector3 p) { pose.p = p; };
	void setVel(Vector3 v) { vel = v; };
	void setAcc(Vector3 a) { acceleration = a; };
	void setDamping(float d) { damping = d; };

protected:

	ParticleTypes myPType;

	Vector3 vel;
	physx::PxTransform pose;
	RenderItem* renderItem;
	float radius;

	Vector3 acceleration;
	float damping; // must be a value between 0 and 1: 0 = max. friction 1 = min. friction

};

