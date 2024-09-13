#include "walkParticle.h"

WalkParticle* WalkParticle::GetInstance()
{
	static WalkParticle instance;
	return &instance;
}

void WalkParticle::Initialize()
{
    particle_ = std::make_unique<ParticleIbu::GpuParticle>();
    particle_->Create(1, "walkParticle");

    emitter_ = make_unique<ParticleIbu::ParticleEmitter<ParticleIbu::EmitType::BoxParam>>();
    emitter_->CreateType(particle_);

    //テクスチャ
    uint32_t handle = TextureManager::GetInstance()->Load("project/gamedata/resources/circle.png");
    particle_->SetTexhandle(handle);
}

void WalkParticle::ImGuiUpdate()
{
    emitter_->ImGuiUpdate();
}

void WalkParticle::Update()
{

    auto& emitControl = emitter_->GetControlParam()[0];
    if (p_position_)
    {
        auto& emit = emitter_->GetEmitParam()[0];
        emit.translate.x = p_position_->num[0];
        emit.translate.y = p_position_->num[1];
        emit.translate.z = p_position_->num[2];
        emit.count = 1;
        const float kscaleVelo = -0.01f;
        emit.scaleVelocityMin = { kscaleVelo,kscaleVelo,kscaleVelo };
        emit.scaleVelocityMax = { kscaleVelo,kscaleVelo,kscaleVelo };
        emit.sizeMax = {};
        emit.sizeMin = {};
        const float kvelo = 0.01f;
        emit.velocityMin = { 0.0f, kvelo, 0.0f};
        emit.velocityMax = { 0.0f,kvelo,0.0f };

        emitControl.frequencyTime = 0.25f;
        emitControl.useFlag_ = true;
    }
    else
    {
        emitControl.useFlag_ = false;
    }

    emitter_->Update();
    emitter_->Emit(particle_);
    particle_->CallBarrier();
    particle_->Update();
}

void WalkParticle::Draw(ViewProjection viewProj)
{
    particle_->Draw(viewProj);
}

void WalkParticle::Clear()
{
    particle_->Clear();
    emitter_->AllClear();
}
