#include <random>
#include "GaussianParticleGenerator.h"

list<Particle*> GaussianParticleGenerator::generateParticles() {

    std::list<Particle*> generations = list<Particle*>();

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

    //POSICION
    std::normal_distribution<> std_dis_posX(_mean_pos.x, _std_dev_pos.x);
    std::normal_distribution<> std_dis_posY(_mean_pos.y, _std_dev_pos.y);
    std::normal_distribution<> std_dis_posZ(_mean_pos.z, _std_dev_pos.z);
    //VELOCIDAD
    std::normal_distribution<> std_dis_velX(_mean_vel.x, _std_dev_vel.x);
    std::normal_distribution<> std_dis_velY(_mean_vel.y, _std_dev_vel.y);
    std::normal_distribution<> std_dis_velZ(_mean_vel.z, _std_dev_vel.z);
    // TIEMPO
    std::uniform_real_distribution<> std_dis_time(2.0, std_dev_t); // crear variable de media para el tiempo

    for (size_t i = 0; i < _num_particles; ++i) {
        // auto rnd = (double(rand()))/(double(RAND_MAX)+1); // value between 0 and 1
        auto rnd = rand() % 100; // Nº between [0, 99]
        if (rnd <= _generation_probability * 100) { // there's a posibility the particle won't generate
            auto part = _model->clone();
            generations.push_back(part); // new particle

            // modify its pos. and vel. according to the generator type

            part->setPosition({ (float)std_dis_posX(gen), (float)std_dis_posY(gen) , (float)std_dis_posZ(gen) });
            part->setVel({ (float)std_dis_velX(gen), (float)std_dis_velY(gen), (float)std_dis_velZ(gen) });

            // set a rnd life span
            part->setRemainingTime(std_dis_time(gen));
        }
    }


    return generations;



}