#include "HitBlockParticle.h"

HitBlockParticle* HitBlockParticle::GetInstance()
{
	static HitBlockParticle instance;
	return &instance;
}

void HitBlockParticle::Initialize()
{
	particle_ = std::make_unique<ParticleIbu::GpuParticle>();
	particle_->Create(1, "HitParticle");

	emitter_ = make_unique<ParticleIbu::ParticleEmitter<ParticleIbu::EmitType::BoxParam>>();
	emitter_->CreateType(particle_);

	//テクスチャ
	uint32_t handle = TextureManager::GetInstance()->Load("project/gamedata/resources/uvChecker.png");
	particle_->SetTexhandle(handle);
}

void HitBlockParticle::ImGuiUpdate()
{
}

void HitBlockParticle::Update()
{
	emitter_->Update();
	particle_->CallBarrier();
	particle_->Update();
}

void HitBlockParticle::Draw(ViewProjection viewProj)
{
	particle_->Draw(viewProj);
}

void HitBlockParticle::Clear()
{
}

void HitBlockParticle::Spown()
{
	auto& emitControl = emitter_->GetControlParam()[0];
	if (p_position_)
	{
		auto& emit = emitter_->GetEmitParam()[0];
		emit.translate.x = p_position_->num[0];
		emit.translate.y = p_position_->num[1];
		emit.translate.z = p_position_->num[2];
		emit.count = 32;
		const float kscaleVelo = -0.01f;
		emit.scaleVelocityMin = { kscaleVelo,kscaleVelo,kscaleVelo };
		emit.scaleVelocityMax = { kscaleVelo,kscaleVelo,kscaleVelo };
		emit.sizeMax = {};
		emit.sizeMin = {};
		const Vector3 kvelo = { 0.05f, 0.05f,0.005f };
		emit.sizeMin = { -1.5f,-0.5f,-1.0f };
		emit.sizeMax = { 1.5f,-0.5f,0.-0.5f };

		emit.velocityMin = { -kvelo.num[0], kvelo.num[1] - 0.025f, -kvelo.num[2]};
		emit.velocityMax = { kvelo.num[0],kvelo.num[1],kvelo.num[2] };
		emitControl.useFlag_ = true;
	}
	emitter_->Emit(particle_);
}
