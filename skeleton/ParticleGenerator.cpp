#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(string name, Vector3 mPos, Vector3 mVel, double genProb, int numPart, Particle* m) :
	_name(name), _mean_pos(mPos), _mean_vel(mVel), _generation_probability(genProb), _num_particles(numPart), _model(m)
{
}

ParticleGenerator::~ParticleGenerator()
{
	delete _model;
}

void ParticleGenerator::setParticle(Particle* m)
{
	_model = m;
}
