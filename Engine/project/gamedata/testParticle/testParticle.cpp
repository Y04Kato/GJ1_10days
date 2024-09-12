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

    emitter_ = make_unique<ParticleIbu::ParticleEmitter<ParticleIbu::EmitType::BoxParam>>();
    emitter_->CreateType(particle_);

    //テクスチャ
    uint32_t handle  = TextureManager::GetInstance()->Load("project/gamedata/resources/uvChecker.png");
    particle_->SetTexhandle(handle);
}

void TestParticle::ImGuiUpdate()
{
    emitter_->ImGuiUpdate();
}

void TestParticle::Update()
{
    emitter_->Update();
    emitter_->Emit(particle_);
    particle_->CallBarrier();
    particle_->Update();
}

void TestParticle::Draw(ViewProjection viewProj)
{
    particle_->Draw(viewProj);
}
