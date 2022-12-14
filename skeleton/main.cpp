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


Particle* particle;
std::list<Particle*> particles;

ParticleSystem* pSys;
WorldManager* wM;
bool fromCamera = false;
string cameraGen;

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

	//auto diana = new Particle(TargetT, 3.0f, {0.0, 50.0, 0.0});
	//auto suelo = new Particle(FloorT, 500.0, { -100.0, -5.0, -100.0 });

	pSys = new ParticleSystem();
	
	wM = new WorldManager(gPhysics, gScene);
	wM->createBaseScene();
	wM->addRigidDynamic();
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	/*for (int i = 0; i < particles.size(); ++i) {
		particles[i]->integrate(t);

		if (particles[i]->getPose().p.x >= 250.0 || particles[i]->getPose().p.y >= 100.0) {
			delete particles[i];
		}
	}*/

	for (auto p : particles){
		p->integrate(t);

		if (p->getPose().p.x >= 250.0 || p->getPose().p.y >= 100.0) {
			p->setAlive(false);
		}
	}

	auto p = particles.begin();
	while (p != particles.end()) {
		if (!(*p)->isAlive()) {
			delete *p;
			p = particles.erase(p);
		}
		else
			p++;
	}

	if (fromCamera) {
		ParticleGenerator* g = pSys->getParticleGenerator(cameraGen);
		
		g->setMeanPos(GetCamera()->getEye() + GetCamera()->getDir() * 10);
		g->setMeanVel(GetCamera()->getDir()*10);
	}
	pSys->update(t);


	wM->update(t);
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

	delete particle;
	for (auto a : particles)
		delete a;

	delete pSys;
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	case ' ':
	{
		break;
	}
	//case 'F': {
	//	pSys->shootFirework(3);
	//	break;
	//}
	//case 'B': // Bubble Cannon
	//{
	//		// Disparar 1 burbuja
	//	// particles.push_back(new Proyectile(ProyectileTypes::Bubble));

	//		// Primera versión Generador de Partículas
	//	//fromCamera = true;
	//	//pSys->removeAllParticleGenerators();
	//	//pSys->addParticleGenerator("GAUSSIAN", ProyectileTypes::Bubble, GeneratorTypes::normal,
	//	//	GetCamera()->getDir() * 10, GetCamera()->getEye() + GetCamera()->getDir() * 3,
	//	//	{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, 0.5, 1, 5.0);

	//	fromCamera = true;
	//	cameraGen = "Bubble_Cannon";

	//	auto p = pSys->getParticleGenerator("Bubble_Cannon");
	//	p->setActive(!p->getActive());
	//	break;
	//}
	//case 'N': // disparar una bala
	//	particles.push_back(new Proyectile(ProyectileTypes::Bullet));
	//	break;

	//case 'U': { // Fuente de pelotas

	//	fromCamera = false;
	//	auto p = pSys->getParticleGenerator("Ball_Fountain");
	//	p->setActive(!p->getActive());

	//	break;
	//}
	//case 'T': { // Avispero

	//	fromCamera = true;
	//	cameraGen = "Beehive";

	//	auto p = pSys->getParticleGenerator("Beehive");
	//	p->setActive(!p->getActive());

	//	break;

	//}
	//case 'R': { // Luvia de balas
	//
	//	fromCamera = false;
	//	auto p = pSys->getParticleGenerator("Bullet_Rain");
	//	p->setActive(!p->getActive());

	//	break;
	//}
	//case 'C' : { // firework con forma
	//	fromCamera = false;
	//	auto p = pSys->getParticleGenerator("ShapeTest");
	//	auto l = p->generateParticles();
	//	//p->setActive(!p->getActive());

	//	for (auto part : l) {
	//		pSys->addParticle(part);
	//	}
	//	break;
	//}
	//case 'G': { // añade 2 particulas a las que la gravedad afecta de forma distinta

	//	pSys->addBubbles();

	//	break;
	//}
	//case 'H': { // añade 2 partículas, una a la que le afecta la fuerza de la gravedad y otra a la que no

	//	pSys->addDifferentABF(); 
	//	break;
	//}
	//case 'J': {
	//	pSys->testWind();
	//	break;
	//}
	//case 'K': {
	//	pSys->testWhirlwind();
	//	break;
	//}
	//case 'L':
	//{
	//	pSys->generateParticles();
	//	break;
	//}
	//case 'M':
	//{
	//	pSys->testExplosion();
	//	break;
	//}

	//case 'Z':
	//{
	//	pSys->testSpring();
	//	break;
	//}
	//case '+':
	//{
	//	pSys->addKSpring();
	//	break;
	//}
	//case '-':
	//{
	//	pSys->subKSpring();
	//	break;
	//}
	//case 'X':
	//{
	//	pSys->testSprings();
	//	break;
	//}
	//case 'V':
	//{
	//	pSys->testRubberBand();
	//	break;
	//}
	//case 'C': 
	//{
	//	pSys->testSlinky();
	//	break;
	//}
	//case 'F': {
	//	pSys->testFloat();
	//	break;
	//}	
	//case ',': {
	//	pSys->addMass();
	//	break;
	//}
	//case '.': {
	//	pSys->subMass();
	//	break;
	//}

	case 'T': 
	{
		wM->changeTorque();
		break;
	}
	case 'E':
	{
		wM->changeExplosion();
		break;
	}
	case 'R':
	{
		wM->addRBGenerator();
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
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