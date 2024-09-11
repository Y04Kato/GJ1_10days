#pragma once

#include"../../../base/components/Particle/GpuParticle.h"

class TestParticle
{
public:
	TestParticle() {};
	~TestParticle() {};

	static TestParticle* GetInstance();

	void Initialize();

private:
	std::unique_ptr<ParticleIbu::GpuParticle> particle_;
};
