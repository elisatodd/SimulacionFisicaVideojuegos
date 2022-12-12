#include "WorldManager.h"

WorldManager::WorldManager(PxPhysics* gp, PxScene* s) : _gPhysics(gp), _gScene(s)
{

	_fr = new RigidBodyForceRegistry();
	_torque = new TorqueGenerator(10, 100, {0, 0, 0});
	_explosion = new ExplosionGenerator(50, 1000, {0, -1, 0});
}

WorldManager::~WorldManager()
{
	for (auto e : _items) {
		DeregisterRenderItem(e);
		delete e;
	}
}

void WorldManager::createBaseScene()
{
	// Plane as a solid rigid
	PxRigidStatic* Suelo = _gPhysics->createRigidStatic(PxTransform({ 0,0,0 }));
	PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
	Suelo->attachShape(*shape);
	auto _item_suelo = new RenderItem(shape, Suelo, {0.8, 0.8, 0.8, 1});
	_gScene->addActor(*Suelo);
	_items.push_back(_item_suelo);

	// Wall
	PxRigidStatic* Pared = _gPhysics->createRigidStatic(PxTransform({ 10,10,-30 }));
	PxShape* shape_pared = CreateShape(PxBoxGeometry(40, 20, 5));
	Pared->attachShape(*shape_pared);
	auto _item_pared = new RenderItem(shape_pared, Pared, { 0.8, 0.8, 0.8, 1 });
	_gScene->addActor(*Pared);
	_items.push_back(_item_pared);

}

void WorldManager::addRigidDynamic()
{
	PxRigidDynamic* new_solid;
	Vector3 size = { 5.0, 5.0, 5.0 };

	new_solid = _gPhysics->createRigidDynamic(PxTransform({0, 10, 0}));

	new_solid->setLinearVelocity({0.0, 10.0,0.0}); // velocidad inicial
	new_solid->setAngularVelocity({0.0, 0.0, 0.0}); // velocidad de giro

	auto shape = CreateShape(PxBoxGeometry(size));
	new_solid->attachShape(*shape);

	new_solid->setMassSpaceInertiaTensor({size.y * size.z, size.x * size.z, size.x * size.y}); // tensor de inercia, marca cómo gira el objeto al chocar

	auto item = new RenderItem(shape, new_solid, {1.0, 0.0, 1.0, 1.0});
	_items.push_back(item);

	_gScene->addActor(*new_solid);

	_fr->addRegistry(_torque, new_solid);
	_fr->addRegistry(_explosion, new_solid);
}

void WorldManager::update(double t)
{
	_fr->updateForces(t);
	_explosion->addConst(t);
}
