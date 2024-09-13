#pragma once

#include"../../../base/components/Particle/GpuParticle.h"
#include"../../../base/components/Particle/Emitter/ParticleEmitter.h"
#include"../../../base/components/debugCamera/DebugCamera.h"
#include"../../../base/components/manager/TextureManager/TextureManager.h"
class SetBlockParticle
{
public:

	static SetBlockParticle* GetInstance();

	void Initialize();

	void ImGuiUpdate();
	void Update();

	void Draw(ViewProjection viewProj);

	void Clear();

#pragma region Set

	void SetPos(const Vector3& p) { p_position_ = &p; }
	void ClearPos() { p_position_ = nullptr; }
#pragma endregion


private:

	SetBlockParticle() = default;
	~SetBlockParticle() = default;

	const Vector3* p_position_ = nullptr;

	std::unique_ptr<ParticleIbu::GpuParticle> particle_;
	std::unique_ptr <ParticleIbu::ParticleEmitter<ParticleIbu::EmitType::BoxParam>>emitter_ = nullptr;
};
