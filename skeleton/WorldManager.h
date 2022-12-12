#pragma once
#include <PxScene.h>
#include <PxPhysics.h>
#include <list>

#include "RigidBodyForceRegistry.h"
#include "TorqueGenerator.h"
#include "ExplosionGenerator.h"
#include "RenderUtils.hpp"
using namespace physx;

class WorldManager
{
public:

	WorldManager(PxPhysics* gp, PxScene* s);
	~WorldManager();

	void createBaseScene();
	void addRigidDynamic();

	void update(double t);

	inline void changeTorque() { _torque->changeState(); };
	inline void changeExplosion() { _explosion->changeState(); };

protected:
	// lista de items de sólidos rígidos
	std::list<RenderItem*> _items;

	PxPhysics* _gPhysics;
	PxScene* _gScene;

	// Generadores de Fuerzas
	RigidBodyForceRegistry* _fr;
	TorqueGenerator* _torque;
	ExplosionGenerator* _explosion;
};

