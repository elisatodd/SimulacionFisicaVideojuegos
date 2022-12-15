#pragma once
#include <list>
#include <string>
#include "core.hpp"
#include "RigidBodyGenerator.h"

using namespace std;
using namespace physx;

class UniformRigidBodyGenerator : public RigidBodyGenerator 
{
public:

	UniformRigidBodyGenerator(std::string name, Vector3 mPos, Vector3 mVel, double genProb, int numPart, RigidBody* rb, Vector3 vel, Vector3 pos, WorldManager* wm) :
		RigidBodyGenerator(name, mPos, mVel, genProb, numPart, rb, wm), _vel_width(vel), _pos_width(pos) {
		_active = true;
	};

	virtual std::list<RigidBody*> generateRB() override;


private:

	Vector3 _vel_width, _pos_width;
};

