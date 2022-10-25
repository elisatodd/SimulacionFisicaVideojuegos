#pragma once
#include <PxPhysicsAPI.h>
#include "core.hpp" // para Vector3
#include "RenderUtils.hpp" // para RenderItem

enum ParticleTypes {
	ProyectileT,
	TargetT,
	FloorT,
	FireworkT
};

class Particle
{
public:

	Particle() {};
	Particle(ParticleTypes t, float r = 0.0f, 
		Vector3 pos = {0.0, 0.0, 0.0}, Vector3 vel = {0.0, 0.0, 0.0},
		Vector3 a = {0.0, 0.0, 0.0}, float d = 0.0f);

	~Particle();

	bool integrate(double t);

	void setPosition(Vector3 p) { pose.p = p; };
	void setVel(Vector3 v) { vel = v; };
	void setAcc(Vector3 a) { acceleration = a; };
	void setDamping(float d) { damping = d; };
	void setRemainingTime(double t) { remainingTime = t; };
	void setColor(Vector4 c) { renderItem->color = c; };

	inline physx::PxTransform getPose() { return pose; }
	inline Vector3 getVel() { return vel; }
	inline double getTime() { return remainingTime; };

	inline bool isAlive() { return alive; }
	inline void setAlive(bool a) { alive = a; }



	virtual Particle* clone() const;

protected:

	ParticleTypes myPType;

	Vector3 vel;
	physx::PxTransform pose;
	RenderItem* renderItem;
	float radius;

	Vector3 acceleration;
	float damping; // must be a value between 0 and 1: 0 = max. friction 1 = min. friction

	bool alive = true;

	double remainingTime;
};

