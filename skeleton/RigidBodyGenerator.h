#pragma once
#include <iostream>
#include <list>
#include "core.hpp"
#include <string>
#include <PxRigidDynamic.h>
#include "WorldManager.h"

using namespace std;
using namespace physx;
enum GeneratorTypes {
	uniform
};

class RigidBodyGenerator
{
public:

	RigidBodyGenerator(string name, Vector3 mPos, Vector3 mVel, double genProb, int numPart, RigidStatic* rb, WorldManager* wm); // actor -> could be dynamic or static
	~RigidBodyGenerator();

	void setRigidBody(RigidStatic* rb);

	virtual std::list<RigidStatic*> generateRB() = 0;

	std::string getGenName() { return _name; }

	inline void setMeanPos(Vector3 p) { _mean_pos = p; };

	inline void setMeanVel(Vector3 v) { _mean_vel = v; };
		
	inline void setActive(bool a) { _active = a; };
	inline bool getActive() { return _active; };

	

protected:
	string _name;
	Vector3 _mean_pos, _mean_vel;
	double _generation_probability;
	int _num_rigid_bodies;
	RigidStatic* _model;
	WorldManager* _wm;

	bool _active; // indica si está activo en la escena -> generando RB constantemente

};


