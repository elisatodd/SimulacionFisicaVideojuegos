#pragma once
#include <map>
#include "ForceGenerator.h"

typedef std::pair<ForceGenerator*, physx::PxRigidDynamic*> FRRBPair;

class RigidBodyForceRegistry : public std::multimap<ForceGenerator*, physx::PxRigidDynamic*>
{
public:
	void updateForces(double duration) {
		for (auto it = begin(); it != end(); it++) {
			it->first->updateForceRB(it->second, duration);
		}
	}

	void addRegistry(ForceGenerator* fg, physx::PxRigidDynamic* rb) {
		insert(FRRBPair( fg, rb ));
	}

	void deleteParticleRegistry(physx::PxRigidDynamic* rb) {
		iterator it = begin();
		for (auto it = begin(); it != end();) {
			if (it->second == rb) {
				it = erase(it);
			}
			else
				it++;
		}
	}
};

