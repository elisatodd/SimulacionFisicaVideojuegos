#pragma once
#include <PxScene.h>
#include <PxPhysics.h>
#include <list>
#include <vector>
#include <string>

#include "RigidBodyForceRegistry.h"
#include "TorqueGenerator.h"
#include "ExplosionGenerator.h"
#include "RenderUtils.hpp"

class UniformRigidBodyGenerator;
class RigidBodyGenerator;

using namespace physx;

struct RigidBody {

	PxRigidActor* actor;
	RenderItem* item;
	float lifeSpan; // tiempo de vida
	float actualLife;
	float deathTime;
	std::vector<std::string> forces;
};

class WorldManager
{
public:

	WorldManager(PxPhysics* gp, PxScene* s);
	~WorldManager();

	void createBaseScene();
	void addRigidDynamic();

	void update(double t);

	inline void changeTorque() { _torque->changeState(); };
	inline void changeExplosion() { _explosion->changeState(); _explosion_active = true; };

	RigidBody* clone(RigidBody* rs) {
		RigidBody* newRS = new RigidBody();
		newRS->lifeSpan = 1000; // 1 segundos de vida a partir del momento actual;
		newRS->deathTime = clock() + newRS->lifeSpan; 
		newRS->actualLife = clock();

		//auto pos = rs->item->actor->getGlobalPose();
		auto new_solid = _gPhysics->createRigidDynamic({0,0,0});
		newRS->actor = new_solid;
		auto geo = rs->item->shape->getGeometryType();

		Vector3 size = { 2.0, 2.0, 2.0 };
		new_solid->setMassSpaceInertiaTensor({ size.y * size.z, size.x * size.z, size.x * size.y });
		
		PxShape* shape;
		switch (geo) {
			case PxGeometryType::eBOX:
			{
				shape = CreateShape(PxBoxGeometry(size));
				break;
			}
			case PxGeometryType::eSPHERE:
			{
				shape = CreateShape(PxSphereGeometry(size.x));
				break;
			}
				break;
		}
		new_solid->attachShape(*shape);
		newRS->item = new RenderItem(shape, new_solid, rs->item->color);

		_gScene->addActor(*new_solid);

		// a�adir las fuerzas
		auto aux = static_cast<PxRigidDynamic*>(newRS->actor);
		for (int i = 0; i < rs->forces.size(); ++i) {
			newRS->forces.push_back(rs->forces[i]);
			auto newf = _fr->getGenerator(rs->forces[i]);
			if (newf != nullptr)
				_fr->addRegistry(newf, aux);
		}

		_rigid_statics.push_back(newRS);

		return newRS;
	}

	void addRBGenerator();

	void deleteItem(RenderItem* i);
	void deleteActor(PxRigidActor* ac);

protected:
	// lista de items de s�lidos r�gidos
	std::list<RenderItem*> _items;
	std::list<RigidBody*> _rigid_statics;

	PxPhysics* _gPhysics;
	PxScene* _gScene;

	// Generadores de Fuerzas
	RigidBodyForceRegistry* _fr;
	TorqueGenerator* _torque;
	ExplosionGenerator* _explosion;
	bool _explosion_active = false;

	// Generadores de part�culas
	std::list<RigidBodyGenerator*> _rigidbodies_generators;
	float _generation_frequency = 20.0; // 100.0 == 1 segundo
	float _next_generation = 0.0;
	float _current_time = 0.0;

};

