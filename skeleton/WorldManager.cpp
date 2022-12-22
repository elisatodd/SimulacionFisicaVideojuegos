#include "WorldManager.h"
#include "RigidBodyGenerator.h"
#include "UniformRigidBodyGenerator.h"
#include "PlayerPlatformCollision.h"
#include "Player.h"
WorldManager::WorldManager(PxPhysics* gp, PxScene* s) : _gPhysics(gp), _gScene(s)
{

	_fr = new RigidBodyForceRegistry();
	_torque = new TorqueGenerator(10, 100, {0, 0, 0});
	_explosion = new ExplosionGenerator(30, 40, {0, 0, 0});

	// Create an instance of the collision callback class for the player and the platforms
	PlayerPlatformCollision collisionCallback;
	// Set the collision callback for the scene
	//_gScene->setSimulationEventCallback(&collisionCallback);
}

WorldManager::WorldManager(PxPhysics* gp, PxScene* s, Player* p) : _gPhysics(gp), _gScene(s), _player(p)
{
	_fr = new RigidBodyForceRegistry();
	_torque = new TorqueGenerator(10, 100, {0, 0, 0});
	_explosion = new ExplosionGenerator(30, 40, {0, 0, 0});
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
	PxRigidStatic* Suelo = _gPhysics->createRigidStatic(PxTransform({ 0,-10,0 }));
	PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
	Suelo->attachShape(*shape);
	auto _item_suelo = new RenderItem(shape, Suelo, {0.8, 0.8, 0.8, 1});
	_gScene->addActor(*Suelo);
	_items.push_back(_item_suelo);
	

	//// Wall
	//PxRigidStatic* Pared = _gPhysics->createRigidStatic(PxTransform({ 10,10,-30 }));
	//PxShape* shape_pared = CreateShape(PxBoxGeometry(40, 20, 5));
	//Pared->attachShape(*shape_pared);
	//auto _item_pared = new RenderItem(shape_pared, Pared, { 0.8, 0.8, 0.8, 1 });
	//_gScene->addActor(*Pared);
	//_items.push_back(_item_pared);

	PxRigidStatic* Pared = _gPhysics->createRigidStatic(PxTransform({ 50, 30 , 0 }));
	auto _plat_geo = PxBoxGeometry(20, 1, 20);
	PxShape* shape_pared = CreateShape(_plat_geo);
	Pared->attachShape(*shape_pared);
	auto _item_pared = new RenderItem(shape_pared, Pared, { 0.09, 0.22, 0.84, 1 });
	Pared->setName("Platform");
	_gScene->addActor(*Pared);
	_platforms.push_back(_item_pared);
	_items.push_back(_item_pared);

}

void WorldManager::addRigidDynamic()
{
	PxRigidDynamic* new_solid;
	Vector3 size = { 7.0, 7.0, 7.0 };

	PxQuat q = { 45, PxVec3 {0, 1, 0} }; // 45 grados girado en Y
	PxQuat q2 = { 10, PxVec3 {1, 0, 0} }; // 10 grados girado en X
	new_solid = _gPhysics->createRigidDynamic(PxTransform({0, 30, 0}, q*q2)); // añadir quaternion para generar con giro

	new_solid->setLinearVelocity({0.0, 10.0,0.0}); // velocidad inicial
	new_solid->setAngularVelocity({0.0, 0.0, 0.0}); // velocidad de giro

	auto shape = CreateShape(PxBoxGeometry(size));
	//auto shape = CreateShape(PxSphereGeometry(2.0));
	new_solid->attachShape(*shape);

	new_solid->setMassSpaceInertiaTensor({size.y * size.z, size.x * size.z, size.x * size.y}); // tensor de inercia, marca cómo gira el objeto al chocar

//	new_solid->setMass(1.0);

	auto item = new RenderItem(shape, new_solid, {1.0, 0.0, 1.0, 1.0});
	_items.push_back(item);	

	_gScene->addActor(*new_solid);

	_fr->addRegistry(_torque, new_solid);
	_fr->addRegistry(_explosion, new_solid);
}

void WorldManager::update(double t)
{
	_fr->updateForces(t);

	if (_explosion_active)
		_explosion->addConst(t);
	
	if (_next_generation <= clock()) { // timer para que se generen las partículas cada X tiempo y no en cada update
		//cout << clock() << "\n";
		for (auto p : _rigidbodies_generators) {
			if (p->getActive()) {
				auto l = p->generateRB();
				for (auto q : l) {
					_items.push_back(q->item);
				}
			}
		}
		_next_generation += _generation_frequency;
	}

	//// RECORRER TODOS LOS ACTORES
	//PxActorTypeFlags desiredTypes = PxActorTypeFlag::eRIGID_DYNAMIC; // | PxActorTypeFlag::eRIGID_STATIC; --> concatenar así 
	//PxU32 count = _gScene->getNbActors(desiredTypes);
	//PxActor** buffer = new PxActor * [count];
	//auto actors = _gScene->getActors(desiredTypes, buffer, count);

	//for (int i = 0; i < count; i++) // para cada partícula activa
	//{
	//	// if (buffer[i]->getOwnerClient() == PX_DEFAULT_CLIENT)   //skip actors owned by foreign clients

	//	if (buffer[i]) {
	//		// procesado
	//	}
	//}
	//delete buffer;

	auto rb = _rigid_statics.begin();
	while (rb != _rigid_statics.end()) { 
		if (clock() >= (*rb)->deathTime) { // si se alcanza el momento en que deben destruirse
			deleteItem((*rb)->item);
			deleteActor((*rb)->actor);
			PxRigidDynamic* pr = (PxRigidDynamic*)((*rb)->actor);
			_fr->deleteParticleRegistry(pr);
			delete* rb;
			rb = _rigid_statics.erase(rb);
		}
		else
			rb++;
	}

	// COLISIÓN DE PLAYER Y PLATAFORMAS	
	/*for (int i = 0; i < _platforms.size(); ++i) {
		const PxTransform tPlayer = _player->getItem()->actor->getGlobalPose();
		const PxTransform tPlat = _platforms[i]->actor->getGlobalPose();
		bool collision = PxGeometryQuery::overlap(_platforms[i]->shape->getGeometry().box(), tPlat, 
												  _player->getItem()->shape->getGeometry().sphere(), tPlayer);
		//PxSweepHit hit;
		//bool collision = PxGeometryQuery::sweep({10,10,10}, 100.0, _player->getGeometry(), tPlayer, _platforms_geo[i], tPlat, hit, PxHitFlag::eDEFAULT);
		if (collision) {
			cout << collision << endl;
		}
	}*/
}

void WorldManager::addRBGenerator()
{
	PxRigidDynamic* new_solid;
	Vector3 size = { 2.0, 2.0, 2.0 };

	PxQuat q = { 45, PxVec3 {0, 1, 0} }; // 45 grados girado en Y
	PxQuat q2 = { 10, PxVec3 {1, 0, 0} }; // 10 grados girado en X
	new_solid = _gPhysics->createRigidDynamic(PxTransform({ 0, 30, 0 }, q * q2)); // añadir quaternion para generar con giro

	new_solid->setLinearVelocity({ 0.0, 10.0,0.0 }); // velocidad inicial
	new_solid->setAngularVelocity({ 0.0, 0.0, 0.0 }); // velocidad de giro

	//auto shape = CreateShape(PxBoxGeometry(size));
	auto shape = CreateShape(PxSphereGeometry(2.0));
	new_solid->attachShape(*shape);

	new_solid->setMassSpaceInertiaTensor({ size.y * size.z, size.x * size.z, size.x * size.y }); // tensor de inercia, marca cómo gira el objeto al chocar

	auto item = new RenderItem(shape, new_solid, { 1.0, 0.0, 1.0, 1.0 });

	_items.push_back(item);

	_gScene->addActor(*new_solid);

	RigidBody* rs = new RigidBody();
	rs->item = item;
	rs->lifeSpan = MAXINT; // este actor debe estar siempre pues es la base de clonación
	rs->deathTime = clock() + rs->lifeSpan;
	rs->actualLife = clock();
	rs->actor = new_solid;

	_fr->addRegistry(_explosion, new_solid);
	rs->forces.push_back("Explosion");

	_rigid_statics.push_back(rs);

	auto gen = new UniformRigidBodyGenerator("Boxes", { 0.0, 10.0, 0.0 }, { 1.0, 1.0, 1.0 }, 1.0, 1, rs, { 5.0, 5.0, 5.0 }, {20.0, 20.0, 20.0}, this);
	_rigidbodies_generators.push_back(gen);
}

void WorldManager::deleteItem(RenderItem* i)
{
	bool found = false;
	auto it = _items.begin();
	while (!found && it != _items.end()) {
		if ((*it) == i) {
			found = true;
			DeregisterRenderItem(*it);
			delete* it;
			it = _items.erase(it);
		}
		else
			it++;
	}
}

void WorldManager::deleteActor(PxRigidActor* ac)
{
	_gScene->removeActor(*ac);
}
