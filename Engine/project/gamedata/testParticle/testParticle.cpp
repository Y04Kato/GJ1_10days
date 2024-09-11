#include "testParticle.h"

TestParticle* TestParticle::GetInstance()
{
    static TestParticle instance;
    return &instance;
}

void TestParticle::Initialize()
{
    particle_ = std::make_unique<ParticleIbu::GpuParticle>();
    particle_->Create(1, "p");

}
