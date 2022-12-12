#include "TorqueGenerator.h"

TorqueGenerator::TorqueGenerator(double k, double R, Vector3 p) : _k(k), _R(R), _point(p)
{
}

void TorqueGenerator::updateForce(Particle* p, double t)
{
}

void TorqueGenerator::updateForceRB(physx::PxRigidDynamic* rb, double t)
{
	if (!_enabled)
		return;

	auto pos = rb->getGlobalPose().p;

	auto difX = pos.x - _point.x;
	auto difY = pos.y - _point.y;
	auto difZ = pos.z - _point.z;

	auto r2 = pow(difX, 2) + pow(difY, 2) + pow(difZ, 2);

	if (r2 > _R * _R)
		return;

	rb->addTorque(Vector3(difX, difY, difZ) * _k);
}