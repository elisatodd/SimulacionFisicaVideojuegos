#pragma once
#include "Particle.h"

enum ProyectileTypes {
	Bubble,
	Bullet
};

class Proyectile : public Particle {

public:							
	Proyectile(ProyectileTypes t) : Particle(ParticleTypes::ProyectileT) {
	
		myType = t;
		Vector3 newPos, newVel, newAcc;

		switch (myType) {
		case Bubble:

			renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(bubble_radius)), &pose, bubble_color);

			newPos = GetCamera()->getEye();
			setPosition(newPos);

			newVel = GetCamera()->getDir() * bubble_velMagnitude;
			setVel(newVel);

			setAcc(bubble_acceleration);
			setDamping(bubble_damping);

			break;

		case Bullet:

			renderItem = new RenderItem(CreateShape(physx::PxCapsuleGeometry(bullet_radius, bullet_height)), &pose, bullet_color);

			newPos = GetCamera()->getEye();
			setPosition(newPos);

			newVel = GetCamera()->getDir() * bullet_velMagnitude;
			setVel(newVel);

			newAcc = bullet_acceleration;
			setAcc(newAcc);
			setDamping(bullet_damping);

			break;
		}

	}; 

	~Proyectile() {};

private:

	Vector4 bubble_color = Vector4(0.8, 1.0, 1.0, 1.0); // AZUL CIELO
	float bubble_velMagnitude = 20.0;
	float bubble_damping = 0.4f;
	float bubble_radius = 0.7f;
	Vector3 bubble_acceleration = Vector3(0.0, 1.0, 0.0);

	Vector4 bullet_color = Vector4(1.0, 0.6, 0.2, 1.0); // NARANJA-DORADO
	float bullet_radius = 0.2f;
	float bullet_height = 0.3f;
	float bullet_velMagnitude = 10.0;
	float bullet_damping = 1.0;
	Vector3 bullet_acceleration = Vector3(10.0, 0.0, 0.0);

	ProyectileTypes myType;
};