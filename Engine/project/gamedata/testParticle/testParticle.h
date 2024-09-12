#pragma once

#include"../../../base/components/Particle/GpuParticle.h"
#include"../../../base/components/Particle/Emitter/ParticleEmitter.h"
#include"../../../base/components/debugCamera/DebugCamera.h"
#include"../../../base/components/manager/TextureManager/TextureManager.h"
class TestParticle
{
public:
	TestParticle() {};
	~TestParticle() {};

	static TestParticle* GetInstance();

	void Initialize();

	void ImGuiUpdate();
	void Update();

	void Draw(ViewProjection viewProj);

private:
	std::unique_ptr<ParticleIbu::GpuParticle> particle_;
	std::unique_ptr <ParticleIbu::ParticleEmitter<ParticleIbu::EmitType::BoxParam>>emitter_ = nullptr;
};
