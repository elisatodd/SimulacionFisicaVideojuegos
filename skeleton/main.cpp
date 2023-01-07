#pragma once
#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <list>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "Particle.h"
#include "Proyectile.h"
#include "ParticleGenerator.h"
#include "ParticleSystem.h"

#include "WorldManager.h"
#include "Player.h"

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;


ParticleSystem* pSys;
WorldManager* wM;
bool fromCamera = false;
string cameraGen;

Player* _player;
bool _win = false;
// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	pSys = new ParticleSystem();
	
	_player = new Player(gPhysics, gScene);

	wM = new WorldManager(gPhysics, gScene, _player);
	wM->createBaseScene();
	//wM->addRigidDynamic();

}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	bool a = false;
	a = gScene->fetchResults(true);

	//if (fromCamera) {
	//	ParticleGenerator* g = pSys->getParticleGenerator(cameraGen);
	//	
	//	g->setMeanPos(GetCamera()->getEye() + GetCamera()->getDir() * 10);
	//	g->setMeanVel(GetCamera()->getDir()*10);
	//}

	pSys->update(t);

	wM->update(t);
	_player->update(t);

	auto cam = GetCamera();
	auto playerTr = _player->getItem()->actor->getGlobalPose();
	cam->setEye({playerTr.p.x, playerTr.p.y, playerTr.p.z + 40});
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();

	delete pSys;
	delete _player;
	delete wM;
}
	
// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);
	//cout << toupper(key) << endl;
	switch(toupper(key))
	{
	case ' ': // espacio --> saltar
	{
		_player->jump();
		break;
	}
	case 'Z': // cancelar salto
	{
		_player->cancelJump();
		break;
	}
	case 'Q':
	{
		_player->changeOrientation(-1);
		break;
	}
	case 'E':
	{
		_player->changeOrientation(1);
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	if (actor1->getName() == "Player" && actor2->getName() == "Platform" || actor1->getName() == "Platform" && actor2->getName() == "Player") {
		//if (!_player->onPlatform()) {
		//	_player->setOnPlatform(true);
		//	_player->setJumping(false);
		//}
		_player->setOnPlatform(true);
		//	cout << "The player reached a platform!\n";
	}
	// si pisa el boton de la bomba, explota
	else if (actor1->getName() == "BotonBomba" || actor2->getName() == "BotonBomba") {
		//cout << "Toca el boton.\n";
		if (!wM->getExplosionActive())
			wM->changeExplosion();
	}
	// Si llega a la meta, salen fireworks
	else if (!_win && (actor1->getName() == "Meta" || actor2->getName() == "Meta") && (actor1->getName() == "Player" || actor2->getName() == "Player")) {
		pSys->shootFirework(3);
		_win = true;
	}


	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}