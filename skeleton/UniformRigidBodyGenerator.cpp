#include "UniformRigidBodyGenerator.h"
#include <random>

list<RigidStatic*> UniformRigidBodyGenerator::generateRB()
{
    std::list<RigidStatic*> generations = list<RigidStatic*>();

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

    //POSICION
    std::uniform_real_distribution<> std_dis_posX(_mean_pos.x - _pos_width.x / 2, _mean_pos.x + _pos_width.x / 2);
    std::uniform_real_distribution<> std_dis_posY(_mean_pos.y - _pos_width.y / 2, _mean_pos.y + _pos_width.y / 2);
    std::uniform_real_distribution<> std_dis_posZ(_mean_pos.z - _pos_width.z / 2, _mean_pos.z + _pos_width.z / 2);
    //VELOCIDAD
    std::uniform_real_distribution<> std_dis_velX(_mean_vel.x - _vel_width.x / 2, _mean_vel.x + _vel_width.x);
    std::uniform_real_distribution<> std_dis_velY(_mean_vel.y - _vel_width.y / 2, _mean_vel.y + _vel_width.y);
    std::uniform_real_distribution<> std_dis_velZ(_mean_vel.z - _vel_width.z / 2, _mean_vel.z + _vel_width.z / 2);
    //ROTACION
    std::uniform_real_distribution<> std_dis_rotX(0, 359);
    std::uniform_real_distribution<> std_dis_rotY(0, 359);
    std::uniform_real_distribution<> std_dis_rotZ(0, 359);

    for (size_t i = 0; i < _num_rigid_bodies; ++i) {
        // auto rnd = (double(rand()))/(double(RAND_MAX)+1); // value between 0 and 1
        auto rnd = rand() % 100; // Nº between [0, 99]
        if (rnd <= _generation_probability * 100) { // there's a posibility the particle won't generate
            auto rb = _wm->clone(_model);
            PxRigidDynamic* actor = (PxRigidDynamic*)(rb->item->actor);
            // modify its pos. and vel. according to the generator type
            auto trans = PxTransform({0, 0, 0});
            trans.p = PxVec3((float)std_dis_posX(gen), (float)std_dis_posY(gen), (float)std_dis_posZ(gen));
            trans.q = PxQuat((float)std_dis_rotX(gen), {1,0,0}) * PxQuat((float)std_dis_rotY(gen), { 0,1,0 }) * PxQuat((float)std_dis_rotZ(gen), { 0,0,1 });
            actor->setGlobalPose(trans);
            actor->setLinearVelocity({ (float)std_dis_velX(gen), (float)std_dis_velY(gen), (float)std_dis_velZ(gen)});
            generations.push_back(rb); // new particle
        }
    }

    return generations;
}
