#pragma once
#pragma once
#include"DirectXCommon.h"

#include"../GpuParticle.h"
#include"StructGpuParticleEmitter.h"
using namespace std;

namespace ParticleIbu {

	template<typename T>
	class ParticleEmitter
	{
	public:
		ParticleEmitter() {};
		~ParticleEmitter() {};

		void CreateType(unique_ptr<ParticleIbu::GpuParticle>& particle);

		void ImGuiUpdate();
		void Update();

		void Emit(unique_ptr<ParticleIbu::GpuParticle>& particle);

		void SpownDraw();

		void Clear(uint32_t index) { emitParam_[index] = {}; particleControl_[index] = {}; }
		void AllClear();

#pragma region Get

		vector<T>& GetEmitParam() { return emitParam_; }
		vector<ParticleIbu::ParticleEmitControl>& GetControlParam() { return particleControl_; }

		string GetEmitName() { return name_; }
		const uint32_t GetEmitMax() { return max_; }

#pragma endregion

	private:

		void CreateDebugDraw();

		void BoxImGuiUpdate();
		void SphereImGuiUpdate();
		void CircleImGuiUpdate();


		void FrequencyUpdate(size_t index);
		string name_ = "";
		static const uint32_t max_ = 32;
		uint32_t srvHandle_ = 0;

		vector<T> emitParam_;
		vector<ParticleEmitControl>particleControl_;

		unique_ptr<BufferResource<T>>emitBuf_ = nullptr;

	};

	template<typename T>
	inline void ParticleEmitter<T>::CreateType(unique_ptr<ParticleIbu::GpuParticle>& particle)
	{
		name_ = "EmitSphere";

		emitBuf_ = make_unique<BufferResource<T>>();
		emitBuf_->CreateResource(max_);
		emitBuf_->CreateInstancingResource(max_, name_, sizeof(T));

		//emitの合計数分確保
		emitParam_.resize(max_);
		particleControl_.resize(max_);
	}

	template<typename T>
	inline void ParticleEmitter<T>::ImGuiUpdate()
	{

		if (ImGui::TreeNode(name_.c_str()))
		{
			for (size_t index = 0; index < max_; index++)
			{
				string paramName = to_string(index);
				ImGui::Separator();
				if (ImGui::TreeNode(paramName.c_str()))
				{
					if (ImGui::TreeNode("control"))
					{
						ImGui::Checkbox("useFlag", &particleControl_[index].useFlag_);
						ImGui::DragFloat("frequencyTime", &particleControl_[index].frequencyTime);

						ImGui::TreePop();
					}
					int count = emitParam_[index].count;
					ImGui::DragInt("spownCount", &count, 1);
					emitParam_[index].count = count;
					ImGui::DragFloat3("translate", &emitParam_[index].translate.x, 0.1f);
					ImGui::DragFloat3("rotate", &emitParam_[index].rotate.x, 0.1f);
					ImGui::Separator();
					ImGui::DragFloat3("sizeMin", &emitParam_[index].sizeMin.x, 0.1f);
					ImGui::DragFloat3("sizeMax", &emitParam_[index].sizeMax.x, 0.1f);
					ImGui::Separator();
					ImGui::DragFloat3("velocityMin", &emitParam_[index].velocityMin.x, 0.1f);
					ImGui::DragFloat3("velocityMax", &emitParam_[index].velocityMax.x, 0.1f);
					ImGui::Separator();
					ImGui::DragFloat4("colorDecayMin", &emitParam_[index].colorDecayMin.x, 0.1f);
					ImGui::DragFloat4("colorDecayMax", &emitParam_[index].colorDecayMax.x, 0.1f);
					ImGui::Separator();
					ImGui::DragFloat3("scaleVelocityMin", &emitParam_[index].scaleVelocityMin.x, 0.1f);
					ImGui::DragFloat3("scaleVelocityMax", &emitParam_[index].scaleVelocityMax.x, 0.1f);
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

	}

	template<typename T>
	inline void ParticleEmitter<T>::Update()
	{
		//時間の更新
		for (size_t index = 0; index < max_; index++)
		{
			FrequencyUpdate(index);

		}
		emitBuf_->Map();
		emitBuf_->Setbuffer(emitParam_);
	}

	template<typename T>
	inline void ParticleEmitter<T>::Emit(unique_ptr<ParticleIbu::GpuParticle>& particle)
	{
		ComPtr<ID3D12GraphicsCommandList> list = DirectXCommon::GetInstance()->GetCommandList();
		ID3D12DescriptorHeap* heap[] = { DirectXCommon::GetInstance()->GetSrvDescriptiorHeap().Get() };
		list->SetDescriptorHeaps(1, heap);

		CitrusJunosEngine::GetInstance()->renderer_->ComputeCommand(PipelineType::PARTICLE_EMIT_BOX);

		particle->CallUavRootparam(0);

		list->SetComputeRootDescriptorTable(
			1,
			emitBuf_->GetHandles().GPU
		);
		RunTimeCounter::GetInstance()->ComputeCommandCall(2);

		list->SetComputeRootDescriptorTable(
			3,
			particle->GetFreeListIndexBuf()->GetHandles().GPU
		);
		list->SetComputeRootDescriptorTable(
			4,
			particle->GetFreeListBuf()->GetHandles().GPU
		);

		UINT dispach = UINT(particle->GetNum() / 1024);
		list->Dispatch(dispach, 1, 1);

	}

	template<typename T>
	inline void ParticleEmitter<T>::SpownDraw()
	{

	}

	template<typename T>
	inline void ParticleEmitter<T>::AllClear()
	{
		for (size_t i = 0; i < max_; i++)
		{
			emitParam_[i] = {};
			particleControl_[i] = {};
		}
	}


	template<typename T>
	inline void ParticleEmitter<T>::BoxImGuiUpdate()
	{
		if (ImGui::TreeNode(name_.c_str()))
		{
			for (size_t index = 0; index < max_; index++)
			{
				string paramName = to_string(index);
				ImGui::Separator();
				if (ImGui::TreeNode(paramName.c_str()))
				{
					if (ImGui::TreeNode("control"))
					{
						ImGui::Checkbox("useFlag", &particleControl_[index].useFlag_);
						ImGui::DragFloat("frequencyTime", &particleControl_[index].frequencyTime);

						ImGui::TreePop();
					}
					int count = emitParam_[index].count;
					ImGui::DragInt("spownCount", &count, 1);
					emitParam_[index].count = count;
					ImGui::DragFloat3("translate", &emitParam_[index].translate.x, 0.1f);
					ImGui::DragFloat3("rotate", &emitParam_[index].rotate.x, 0.1f);
					ImGui::Separator();
					ImGui::DragFloat3("sizeMin", &emitParam_[index].sizeMin.x, 0.1f);
					ImGui::DragFloat3("sizeMax", &emitParam_[index].sizeMax.x, 0.1f);
					ImGui::Separator();
					ImGui::DragFloat3("velocityMin", &emitParam_[index].velocityMin.x, 0.1f);
					ImGui::DragFloat3("velocityMax", &emitParam_[index].velocityMax.x, 0.1f);
					ImGui::Separator();
					ImGui::DragFloat4("colorDecayMin", &emitParam_[index].colorDecayMin.x, 0.1f);
					ImGui::DragFloat4("colorDecayMax", &emitParam_[index].colorDecayMax.x, 0.1f);
					ImGui::Separator();
					ImGui::DragFloat3("scaleVelocityMin", &emitParam_[index].scaleVelocityMin.x, 0.1f);
					ImGui::DragFloat3("scaleVelocityMax", &emitParam_[index].scaleVelocityMax.x, 0.1f);
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}

	template<typename T>
	inline void ParticleEmitter<T>::SphereImGuiUpdate()
	{
		if (ImGui::TreeNode(name_.c_str()))
		{
			for (size_t index = 0; index < max_; index++)
			{
				string paramName = to_string(index);
				if (ImGui::TreeNode(paramName.c_str()))
				{
					ImGui::DragFloat3("translate", &emitParam_[index].translate.x, 0.1f);
					ImGui::DragFloat3("rotate", &emitParam_[index].rotate.x, 0.1f);
					ImGui::DragFloat("radious", &emitParam_[index].radious, 0.1f);
					ImGui::Separator();
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
	template<typename T>
	inline void ParticleEmitter<T>::CircleImGuiUpdate()
	{
		if (ImGui::TreeNode(name_.c_str()))
		{
			for (size_t index = 0; index < max_; index++)
			{
				string paramName = to_string(index);
				ImGui::Separator();
				if (ImGui::TreeNode(paramName.c_str()))
				{
					if (ImGui::TreeNode("control"))
					{
						ImGui::Checkbox("useFlag", &particleControl_[index].useFlag_);
						ImGui::DragFloat("frequencyTime", &particleControl_[index].frequencyTime);

						ImGui::TreePop();
					}
					int count = emitParam_[index].count;
					ImGui::DragInt("spownCount", &count, 1);
					emitParam_[index].count = count;
					ImGui::DragFloat3("translate", &emitParam_[index].translate.x, 0.1f);
					ImGui::DragFloat3("rotate", &emitParam_[index].rotate.x, 0.1f);
					ImGui::Separator();
					ImGui::DragFloat("radious", &emitParam_[index].radious, 0.1f);
					ImGui::Separator();
					ImGui::DragFloat3("velocityMin", &emitParam_[index].velocityMin.x, 0.1f);
					ImGui::DragFloat3("velocityMax", &emitParam_[index].velocityMax.x, 0.1f);
					ImGui::Separator();
					ImGui::DragFloat4("colorDecayMin", &emitParam_[index].colorDecayMin.x, 0.1f);
					ImGui::DragFloat4("colorDecayMax", &emitParam_[index].colorDecayMax.x, 0.1f);
					ImGui::Separator();
					ImGui::DragFloat3("scaleVelocityMin", &emitParam_[index].scaleVelocityMin.x, 0.1f);
					ImGui::DragFloat3("scaleVelocityMax", &emitParam_[index].scaleVelocityMax.x, 0.1f);
					ImGui::Separator();
					ImGui::DragFloat3("scaleSizeMin", &emitParam_[index].scaleSizeMin.x, 0.1f);
					ImGui::DragFloat3("scaleSizeMax", &emitParam_[index].scaleSizeMax.x, 0.1f);
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
	template<typename T>
	inline void ParticleEmitter<T>::FrequencyUpdate(size_t index)
	{
		if (particleControl_[index].useFlag_)
		{
			particleControl_[index].frequency += 1.0f / 60.0f;
			if (particleControl_[index].frequency >= particleControl_[index].frequencyTime)
			{
				particleControl_[index].frequency = 0.0f;
				emitParam_[index].emit = 1;
			}
			else
			{
				emitParam_[index].emit = 0;
			}
		}
	}
}