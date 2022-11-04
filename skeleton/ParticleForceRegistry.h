#pragma once
#include <map>
#include "ForceGenerator.h"

typedef std::pair<ForceGenerator*, Particle*> FRPair;
// Parejas de fuerza-part�cula
// Como es multimap, una misma clave (generador) puede tener m�s de un valor (particula)

class ParticleForceRegistry : public std::multimap<ForceGenerator*, Particle*>
{
public:
	void updateForces(double duration) {
		for (auto it = begin(); it != end(); it++) {
			it->first->updateForce(it->second, duration);
		}
	}

	void addRegistry(ForceGenerator* fg, Particle* p) {
		insert({fg, p});
	}

	void deleteParticleRegistry(Particle* p) {
		iterator it = begin();
		for (auto it = begin(); it != end();) {
			if (it->second == p) {
				it = erase(it);
			}
			else
				it++;
		}
	}
};

