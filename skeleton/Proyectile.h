#pragma once
#include "Particle.h"

enum ProyectileTypes {
	Bubble,
	Bullet,
	Ball
};

class Proyectile : public Particle {

public:							
	Proyectile(ProyectileTypes t) : Particle(ParticleTypes::ProyectileT) {
	
		myType = t;
		Vector3 newPos, newVel;

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

			renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(bullet_radius)), &pose, bullet_color);

			newPos = GetCamera()->getEye();
			setPosition(newPos);

			newVel = GetCamera()->getDir() * bullet_velMagnitude;
			setVel(newVel);


			setAcc(bullet_acceleration);
			setDamping(bullet_damping);

			break;

		case Ball:

			renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(ball_radius)), &pose, ball_color);

			//setVel({0.0, 10.0, 0.0});

			setAcc(ball_acceleration);
			setDamping(ball_damping);

			break;
		}

	}; 

	~Proyectile() {};

	Particle* clone() const override;

private:

	Vector4 bubble_color = Vector4(0.8, 1.0, 1.0, 1.0); // AZUL CIELO
	float bubble_velMagnitude = 20.0;
	float bubble_damping = 0.4f;
	float bubble_radius = 0.7f;
	Vector3 bubble_acceleration = Vector3(0.0, 1.0, 0.0);

	Vector4 bullet_color = Vector4(1.0, 0.6, 0.2, 1.0); // NARANJA-DORADO
	float bullet_radius = 0.2f;
	float bullet_velMagnitude = 50.0;
	float bullet_damping = 1.0;
	Vector3 bullet_acceleration = Vector3(0.0, -0.5, 0.0);


	Vector4 ball_color = Vector4(1.0, 0.5, 0.5, 1.0);
	float ball_velMagnitud = 0.0f;
	float ball_damping = 1.0;
	float ball_radius = 0.8f;
	Vector3 ball_acceleration = Vector3(0.0, -10.0, 0.0);


	ProyectileTypes myType;
};