#include "Player.h"
#include "Proyectile.h"
#include "UniformParticleGenerator.h"
#include "GravityForceGenerator.h"
#include "ParticleForceRegistry.h"

using namespace physx;

Player::Player(PxPhysics* gp, PxScene* s)
{
	_gPhysics = gp;
	_gScene = s;

	// Rígido sólido del jugador
	PxRigidDynamic* new_solid;
	float size = 5.0;

	new_solid = _gPhysics->createRigidDynamic(PxTransform({ 0, 0, 0 }));

	new_solid->setLinearVelocity({ 0.0, 0.0,0.0 }); // velocidad inicial
	new_solid->setAngularVelocity({ 0.0, 0.0, 0.0 }); // velocidad de giro

	auto shape = CreateShape(PxSphereGeometry(size));
	new_solid->attachShape(*shape);

	new_solid->setMassSpaceInertiaTensor({ size * size, size * size, size * size }); // tensor de inercia, marca cómo gira el objeto al chocar

	_render_item = new RenderItem(shape, new_solid, { 0.5, 1.0, 1.0, 1.0 });

	_gScene->addActor(*new_solid);

	// orientación empieza hacia arriba
	_orientation = { 0, 1, 0 };

	// Partícula para generar cuando se calcule la trayectoria
	Particle* p = new Proyectile(ProyectileTypes::Ball);
	p->setMass(1.0);
	p->setABF(true);
	p->setRemainingTime(1000.0); // se muestran durante 1 segundo
	p->setPosition({ -100000.0,  -100000.0 , -100000.0 }); // ocultar la partícula base
	//La posición y velocidad del generador deben ser dependientes de la posición del jugador --> se hace en cada update
	_upg = new UniformParticleGenerator("PlayerGenerator", {0.0, 0.0, 0.0} , {0.0, 0.0, 0.0}, 1, 1, p, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0});


	_gfg = new GravityForceGenerator({ 0.0, -9.8, 0.0 });
	_pfr = new ParticleForceRegistry();
}

Player::~Player()
{
}

void Player::update(double t)
{
	if (_preparing_jump ) {

		if (_next_generation <= clock()) {

			_upg->setMeanPos(_render_item->actor->getGlobalPose().p);
			_upg->setMeanVel(_orientation*_jump_power);
			
			for (auto p : _upg->generateParticles()) {
				_particles.push_back(p);
				_pfr->addRegistry(_gfg, p);
				p->setRemainingTime(1000.0); // se muestran durante 1 segundo -> ignorar el tiempo de la distribución
			}
			_next_generation += _generation_frequency;	
		}

		_pfr->updateForces(t);	

		for (auto pa : _particles) {
			pa->integrate(t);
		}
	}

	// elimina las partículas muertas
	auto p = _particles.begin();
	while (p != _particles.end()) {
		if (!(*p)->isAlive()) {
			_pfr->deleteParticleRegistry(*p);
			delete* p;
			p = _particles.erase(p);
		}
		else
			p++;
	}
}

void Player::jump()
{
	if (_preparing_jump) {
		// estaba preparando el salto -> ahora comienza a ejecutarlo

		PxRigidDynamic* actor = (PxRigidDynamic*)(_render_item->actor);
		//actor->setAngularVelocity(_orientation * _jump_power);
		actor->setLinearVelocity(_orientation*_jump_power);

		// elimina las particulas de la trayectoria
		auto p = _particles.begin();
		while (p != _particles.end()) {
			(*p)->setAlive(false);
			p++;
		}
	}
	_preparing_jump = !_preparing_jump;
}

void Player::changeOrientation(float dir)
{
	// dir = 1 if ->
	// dir = -1 if <-
	_orientation.x += dir * 0.1;
}
