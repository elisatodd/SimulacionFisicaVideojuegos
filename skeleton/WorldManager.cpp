#include "WorldManager.h"
#include "RigidBodyGenerator.h"
#include "UniformRigidBodyGenerator.h"
#include "PlayerPlatformCollision.h"
#include "Player.h"

WorldManager::WorldManager(PxPhysics* gp, PxScene* s) : _gPhysics(gp), _gScene(s)
{
	// Create an instance of the collision callback class for the player and the platforms
	PlayerPlatformCollision collisionCallback;
	// Set the collision callback for the scene
	//_gScene->setSimulationEventCallback(&collisionCallback);
}

WorldManager::WorldManager(PxPhysics* gp, PxScene* s, Player* p) : _gPhysics(gp), _gScene(s), _player(p)
{
	_fr = new RigidBodyForceRegistry();
	_torque = new TorqueGenerator(10, 100, {0, 0, 0});
	_explosion = new ExplosionGenerator(5, 10, { -10, 11 , 0 });
	//_wind = new UniformWindGenerator(10, 20, { -10.0, 0.0, 0.0 }, { 50, 100, 0 }, 30, 10, 50);
	_wind = new UniformWindGenerator(0.1, 0.2, { -50.0, 0.0, 0.0 }, { 10, 16, 0 }, 3.5, 1, 3);

	Particle* water = new Particle(ParticleTypes::WaterT, 200.0, { 0, -10 , 0 });
	_buoyancy = new BuoyancyForceGenerator(1.0, (4/3) * 3.14 * pow(0.5,3), 997, water); // densidad del agua = 997kg/m^3
	

	PxRigidDynamic* pAc = (PxRigidDynamic*)_player->getItem()->actor;
	_fr->addRegistry(_buoyancy, pAc);
	_fr->addRegistry(_explosion, pAc);
	_fr->addRegistry(_wind, pAc);
}

WorldManager::~WorldManager()
{
	_player = nullptr;

	for (auto e : _items) {
		DeregisterRenderItem(e);
		delete e;
	}

	for (auto e : _platforms) {
		DeregisterRenderItem(e);
		delete e;
	}

	delete _fr;
	delete _torque;
	delete _explosion;
	delete _wind;
	delete _buoyancy;

	for (auto p : _rigidbodies_generators) {
		delete p;
	}

	for (auto p : _rigid_statics) {
		delete p;
	}

	for (auto p : _npcs) {
		delete p;
	}

}


void WorldManager::createBaseScene()
{
	// Plane as a solid rigid
	PxRigidStatic* Suelo = _gPhysics->createRigidStatic(PxTransform({ 0, 0, 0 }));
	PxShape* shape = CreateShape(PxBoxGeometry(1.5, 0.5, 1.5));
	Suelo->attachShape(*shape);
	Suelo->setName("Suelo");
	auto _item_suelo = new RenderItem(shape, Suelo, {0.7, 0.4, 1.0, 1});
	_gScene->addActor(*Suelo);
	_items.push_back(_item_suelo);

	PxRigidStatic* Plat = _gPhysics->createRigidStatic(PxTransform({ 10, 5, 0 }));
	auto _plat_geo = PxBoxGeometry(3, 0.5, 3);
	PxShape* shape_plat = CreateShape(_plat_geo);
	Plat->attachShape(*shape_plat);
	auto _item_pared = new RenderItem(shape_plat, Plat, { 1.0, 0.22, 0.84, 1 });
	Plat->setName("Platform");
	_gScene->addActor(*Plat);
	_platforms.push_back(_item_pared);

	PxRigidStatic* Plat2 = _gPhysics->createRigidStatic(PxTransform({ -10, 10 , 0 }));
	PxShape* shape_plat2 = CreateShape(_plat_geo);
	Plat2->attachShape(*shape_plat2);
	auto _item_pared2 = new RenderItem(shape_plat2, Plat2, { 1.0, 0.22, 0.84, 1 });
	Plat2->setName("Platform");
	_gScene->addActor(*Plat2);
	_platforms.push_back(_item_pared2);

	PxRigidStatic* BotonBomba = _gPhysics->createRigidStatic(PxTransform({ -10, 11 , 0 }));
	auto _bot_geo = PxBoxGeometry(1, 1, 1);
	PxShape* shape_bot = CreateShape(_bot_geo);
	BotonBomba->attachShape(*shape_plat2);
	auto _item_boton = new RenderItem(shape_bot, BotonBomba, { 1.0, 0.0, 0.0, 1 });
	BotonBomba->setName("BotonBomba");
	_gScene->addActor(*BotonBomba);
	_platforms.push_back(_item_boton);

	//PxRigidStatic* Water = _gPhysics->createRigidStatic(PxTransform({ 0, -50 , 0 }));
	//auto _water_geo = PxBoxGeometry(200, 10, 200);
	//PxShape* shape_water = CreateShape(_water_geo);
	//Water->attachShape(*shape_water);
	//auto _item_water = new RenderItem(shape_water, Water, { 0.0, 0.1, 0.5, 1 });
	//Water->setName("Water");
	//_gScene->addActor(*Water);
	//_items.push_back(_item_water);

	PxRigidStatic* Plat3 = _gPhysics->createRigidStatic(PxTransform({ 10, 15, 0 }));
	PxShape* shape_plat3 = CreateShape(_plat_geo);
	Plat3->attachShape(*shape_plat3);
	auto _item_pared3 = new RenderItem(shape_plat3, Plat3, { 1.0, 0.22, 0.84, 1 });
	Plat3->setName("Platform");
	_gScene->addActor(*Plat3);
	_platforms.push_back(_item_pared3);

	PxRigidStatic* Plat4 = _gPhysics->createRigidStatic(PxTransform({ -10, 20, 0 }));
	PxShape* shape_plat4 = CreateShape(_plat_geo);
	Plat4->attachShape(*shape_plat3);
	auto _item_pared4 = new RenderItem(shape_plat4, Plat4, { 1.0, 0.22, 0.84, 1 });
	Plat4->setName("Platform");
	_gScene->addActor(*Plat4);
	_platforms.push_back(_item_pared4);

	// Obstáculo: generador de pelotas
	addRBGenerator();

	//PxRigidStatic* prueba = _gPhysics->createRigidStatic(PxTransform({ 10, 16, 0 }));
	//auto prueba_geo = PxBoxGeometry(3, 1, 3);
	//PxShape* shape_prueba = CreateShape(prueba_geo);
	//prueba->attachShape(*shape_prueba);
	//auto _item_prueba = new RenderItem(shape_prueba, prueba, { 1.0, 1.0, 0.84, 1 });
	//_gScene->addActor(*prueba);
	//_platforms.push_back(_item_prueba);
	//_items.push_back(_item_prueba);

	PxRigidStatic* Meta = _gPhysics->createRigidStatic(PxTransform({ 0, 25, 0 }));
	shape = CreateShape(PxBoxGeometry(1.5, 0.2, 1.5));
	Meta->attachShape(*shape);
	Meta->setName("Meta");
	auto _item_meta = new RenderItem(shape, Meta, { 0.7, 0.4, 1.0, 1 });
	_gScene->addActor(*Meta);
	_items.push_back(_item_meta);

	addNPC(1);
	addNPC(2);
	

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

	if (_explosion_active) {
		_explosion->addConst(t);
		if (clock() >= _exp_timer) {
			changeExplosion();
			cout << "Explosion state: " << _explosion_active << endl;
		}
	}
	
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
	Vector3 size = { 0.25, 0.25, 0.25 };

	PxQuat q = { 45, PxVec3 {0, 1, 0} }; // 45 grados girado en Y
	PxQuat q2 = { 10, PxVec3 {1, 0, 0} }; // 10 grados girado en X
	new_solid = _gPhysics->createRigidDynamic(PxTransform({ 0, -100, 0 }, q * q2)); // añadir quaternion para generar con giro

	new_solid->setLinearVelocity({ 0.0, 10.0,0.0 }); // velocidad inicial
	new_solid->setAngularVelocity({ 0.0, 0.0, 0.0 }); // velocidad de giro

	//auto shape = CreateShape(PxBoxGeometry(size));
	auto shape = CreateShape(PxSphereGeometry(0.25));
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

	_rigid_statics.push_back(rs);
	
	auto gen = new UniformRigidBodyGenerator("Obstacle", { -12.0, 23.0, 0.0 }, { 5.0, 0.0, 0.0 }, 1.0, 1, 
													rs, { 1.0, 0.5, 0.0 }, { 0.0, 0.0, 0.0 }, this);
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

void WorldManager::addNPC(int type)
{
	// Objetos flotando en el agua
	switch (type) {

	case 1:
	{// BASE
		PxRigidDynamic* npc = _gPhysics->createRigidDynamic(PxTransform({ 0, -8, 0 }));
		auto _npc_geo = PxBoxGeometry(0.6, 0.6, 0.6);
		PxShape* shape_npc = CreateShape(_npc_geo);
		npc->attachShape(*shape_npc);
		npc->setName("NPC");
		npc->setMass(10.0);
		auto _item_npc = new RenderItem(shape_npc, npc, { 0.0, 1.0, 0.0, 1.0 });

		_gScene->addActor(*npc);
		_items.push_back(_item_npc);

		_fr->addRegistry(_buoyancy, npc);

		RigidBody* rs = new RigidBody();
		rs->item = _item_npc;
		rs->lifeSpan = MAXINT; // este actor debe estar siempre pues es la base de clonación
		rs->deathTime = MAXINT;
		rs->actualLife = clock();
		rs->actor = npc;
		rs->forces.push_back("Buoyancy");

		_rigid_statics.push_back(rs);

		auto gen = new UniformRigidBodyGenerator("NPCs", { 0.0, -9.0, 0.0 }, { 0.0, 0.0, 0.0 }, 1.0, 10, rs, { 0.0, 0.0, 0.0 }, { 100.0, 0.0, 100.0 }, this);
		_npcs.push_back(gen);
		for (auto n : gen->generateRB()) {
			n->lifeSpan = MAXINT;
			n->deathTime = MAXINT;
			_items.push_back(n->item);

			PxRigidDynamic* actor = (PxRigidDynamic*)(n->item->actor);
			actor->setMass(npc->getMass());

			//cout << "Type " << type << ": " << actor->getMass() << "\n";
		}

		break;
	}

	case 2:
	{

		PxRigidDynamic* npc = _gPhysics->createRigidDynamic(PxTransform({ 1, -8, 1 }));
		auto _npc_geo = PxBoxGeometry(1, 1, 1);
		PxShape* shape_npc = CreateShape(_npc_geo);
		npc->attachShape(*shape_npc);
		npc->setName("NPC");
		npc->setMass(400.0);
		auto _item_npc = new RenderItem(shape_npc, npc, { 1.0, 0.0, 0.3, 1.0 });

		_gScene->addActor(*npc);
		_items.push_back(_item_npc);

		_fr->addRegistry(_buoyancy, npc);

		RigidBody* rs = new RigidBody();
		rs->item = _item_npc;
		rs->lifeSpan = MAXINT; // este actor debe estar siempre pues es la base de clonación
		rs->deathTime = MAXINT;
		rs->actualLife = clock();
		rs->actor = npc;
		rs->forces.push_back("Buoyancy");

		_rigid_statics.push_back(rs);

		auto gen = new UniformRigidBodyGenerator("NPCs", { 0.0, -9.0, 0.0 }, { 0.0, 0.0, 0.0 }, 1.0, 10, rs, { 0.0, 0.0, 0.0 }, { 110.0, 0.0, 110.0 }, this);
		_npcs.push_back(gen);

		for (auto n : gen->generateRB()) {
			n->lifeSpan = MAXINT;
			n->deathTime = MAXINT;
			_items.push_back(n->item);

			PxRigidDynamic* actor = (PxRigidDynamic*)(n->item->actor);
			actor->setMass(npc->getMass());

			//cout << "Type " << type << ": " << actor->getMass() << "\n";
		}
		break;
	}

	}
	
}
