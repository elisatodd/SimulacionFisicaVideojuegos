#pragma once
#include "ForceGenerator.h"
class TorqueGenerator : public ForceGenerator
{
public:
    TorqueGenerator(double k, double R, Vector3 p);
    ~TorqueGenerator() = default;

    virtual void updateForce(Particle* p, double t); // método para partículas 
    virtual void updateForceRB(physx::PxRigidDynamic* p, double t);

    void changeState() { _enabled = !_enabled; };
    bool getState() { return _enabled; }

protected:

    bool _enabled = false;

    double _k, _R;
    Vector3 _point;
};

