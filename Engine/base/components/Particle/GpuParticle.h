#pragma once
#include"../manager/SRVManager/SRVManager.h"
#include"../CreateResource/BufferResources.h"
#include"WorldTransform.h"
#include"../manager/PSOManager/PSOManager.h"
#include"../RuntimeCounter/RunTimeCounter.h"
#include"Emitter/StructGpuParticleEmitter.h"
#include<vector>
#include<memory>

struct ParticleCS
{
	Vector3 translate;
	Vector3 scale;
	Vector3 rotate;
	Matrix4x4 matWorld;
	float lifeTime;
	Vector3 velocity;
	float currentTime;
	Vector4 color = {1.0f,1.0f,1.0f,1.0f};
	Vector4 colorDecay = { 0.0f,0.0f,0.0f,0.01f };
	Vector3 scaleVelocity = {};
	bool isDraw_ = false;
};

namespace ParticleIbu {

	class GpuParticle
	{
	public:
		GpuParticle() {};
		~GpuParticle() {};

		void Create(const size_t kNum, std::string Name);

		void Update();

		void Draw();

		void CallBarrier();

		void CallUavRootparam(uint32_t rootParamIndex);

		void Clear();

#pragma region Set

		void SetTexhandle(uint32_t texHandle) { texHandle_ = texHandle; }
	
#pragma endregion

#pragma region Get

		std::string GetName() { return name_; }
		uint32_t GetNum() { return uint32_t(particleNum_); }

		BufferResource<uint32_t>*GetFreeListIndexBuf() { return freeListIndexBuf_.get(); }
		BufferResource<uint32_t>* GetFreeListBuf() { return freeListBuf_.get(); }

#pragma endregion

	private:

		size_t particleMin = 1024;
		size_t particleNum_ = 0;
		std::string name_ = "";
		uint32_t mulNum = 1;
		const int vertexNum = 4;
		const int indexNum = 6;
		std::unique_ptr<BufferResource<ParticleIbu::System::StructData::ParticleVertexData>>vertexBuf_ = nullptr;
		std::vector<ParticleIbu::System::StructData::ParticleVertexData>vertexParam_;
		std::unique_ptr<BufferResource<uint32_t>>indexBuf_ = nullptr;
		std::vector<uint32_t>indexParam_;

		std::unique_ptr<BufferResource<ParticleCS>>writeParticleBuf_ = nullptr;
		std::vector<ParticleCS>writeParticleParam_;

		//フリーリストのインデックス
		std::unique_ptr<BufferResource<uint32_t>>freeListIndexBuf_ = nullptr;
		std::vector<uint32_t>freeListIndex_;

		//List
		std::unique_ptr<BufferResource<uint32_t>>freeListBuf_ = nullptr;
		std::vector<uint32_t>freeList_;

		uint32_t texHandle_ = 1;
	};

};