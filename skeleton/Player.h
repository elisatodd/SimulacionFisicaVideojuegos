#pragma once
#include <PxScene.h>
#include <PxPhysics.h>
#include <list>
#include <vector>
#include <string>

#include "RenderUtils.hpp"

class UniformParticleGenerator;
class GravityForceGenerator;
class ParticleForceRegistry;
class Particle;

using namespace physx;
using namespace std;

class Player
{
public:
	Player(PxPhysics* gp, PxScene* s);

	~Player();

	void update(double t);

	void jump();
	void changeOrientation(float dir);
	void setOnPlatform(bool a);
	void setJumping(bool a);
	inline bool onPlatform() { return _onPlatform; };
	inline bool isJumping() { return _jumping; };

	void cancelJump();

	RenderItem* getItem() { return _render_item; };
private:
	PxPhysics* _gPhysics;
	PxScene* _gScene;

	Vector3 _orientation = {0, 1, 0}; // hacia donde saltará la partícula
	float _jump_power = 25.0f; // potencia del salto

	RenderItem* _render_item; 

	bool _preparing_jump = false;
	bool _onPlatform = false;
	bool _jumping = false;;
	float _next_generation = 0.0;
	float _generation_frequency = 300.0; // 1000.0 == 1 segundo

	float _respawn_time = 5000.0; // reaparece a los 5s. de caer
	float _next_respawn = 0.0;
	bool _death = false;

	// El player emite partículas que muestran la trayectoria del salto
	UniformParticleGenerator* _upg;
	list<Particle*> _particles; // aquí se guardan las partículas que determinan la trayectoria
	GravityForceGenerator* _gfg; // para que en la trayectoria se vea el efecto que tendrá la gravedad sobre el jugador

	ParticleForceRegistry* _pfr;
};

