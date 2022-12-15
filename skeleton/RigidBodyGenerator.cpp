#include "RigidBodyGenerator.h"

RigidBodyGenerator::RigidBodyGenerator(string name, Vector3 mPos, Vector3 mVel, double genProb, int numPart, RigidBody* rb, WorldManager* wm) :
	_name(name), _mean_pos(mPos), _mean_vel(mVel), _generation_probability(genProb), _num_rigid_bodies(numPart), _model(rb), _active(false), _wm(wm)

{
}

RigidBodyGenerator::~RigidBodyGenerator()
{
}

void RigidBodyGenerator::setRigidBody(RigidBody* rb)
{
	_model = rb;
}
