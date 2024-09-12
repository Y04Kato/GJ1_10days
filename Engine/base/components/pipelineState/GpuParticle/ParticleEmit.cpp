#include "ParticleEmit.h"

void ParticleEmit::ShaderCompile()
{
	csShaderBlob =
		ShaderCompiler::GetInstance()->CompileShader(
			L"project/gamedata/resources/shaders/Particle/Emitter/Particle_TypeBox.CS.hlsl", L"cs_6_0");
	assert(csShaderBlob != nullptr);
}

void ParticleEmit::CreateRootSignature()
{
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	HRESULT hr = {};
	//rootsignature作成
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_ROOT_PARAMETER rootParameters[5] = {};

	//u0 : tパラメーターuav
	D3D12_DESCRIPTOR_RANGE descriptorRange_UAV = {};
	descriptorRange_UAV.BaseShaderRegister = 0;
	descriptorRange_UAV.NumDescriptors = 1;
	descriptorRange_UAV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRange_UAV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[0].DescriptorTable.pDescriptorRanges = &descriptorRange_UAV;
	rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;

	//t1 : tパラメーターsrv
	D3D12_DESCRIPTOR_RANGE descriptorRange_SRV = {};
	descriptorRange_SRV.BaseShaderRegister = 0;
	descriptorRange_SRV.NumDescriptors = 1;
	descriptorRange_SRV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange_SRV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].DescriptorTable.pDescriptorRanges = &descriptorRange_SRV;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;

	//b0 : exe起動時間
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[2].Descriptor.ShaderRegister = 0;

	//u1 freeListIndex
	D3D12_DESCRIPTOR_RANGE descriptorRange_freeListIndex = {};
	descriptorRange_freeListIndex.BaseShaderRegister = 1;
	descriptorRange_freeListIndex.NumDescriptors = 1;
	descriptorRange_freeListIndex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRange_freeListIndex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[3].DescriptorTable.pDescriptorRanges = &descriptorRange_freeListIndex;
	rootParameters[3].DescriptorTable.NumDescriptorRanges = 1;

	//u2 freeList
	D3D12_DESCRIPTOR_RANGE descriptorRange_freeList = {};
	descriptorRange_freeList.BaseShaderRegister = 2;
	descriptorRange_freeList.NumDescriptors = 1;
	descriptorRange_freeList.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRange_freeList.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[4].DescriptorTable.pDescriptorRanges = &descriptorRange_freeList;
	rootParameters[4].DescriptorTable.NumDescriptorRanges = 1;

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	//rootsignatureの作成
	assert(csShaderBlob != nullptr);
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr))
	{
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	Microsoft::WRL::ComPtr<ID3D12Device>device = DirectXCommon::GetInstance()->GetDevice();
	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&PipelineStateObject_.rootSignature));
	assert(SUCCEEDED(hr));
}

void ParticleEmit::CreateInputLayOut()
{
}

void ParticleEmit::CreateBlendState()
{
}

void ParticleEmit::CreateRasterizarState()
{
}

void ParticleEmit::CreatePipelineStateObject()
{

	D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc = {};
	//psoの作成
	psoDesc.CS = {
	.pShaderBytecode = csShaderBlob->GetBufferPointer(),
	.BytecodeLength = csShaderBlob->GetBufferSize()
	};
	psoDesc.pRootSignature = PipelineStateObject_.rootSignature.Get();
	psoDesc.NodeMask = 0;
	Microsoft::WRL::ComPtr<ID3D12Device>device = DirectXCommon::GetInstance()->GetDevice();
	hr = device->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&PipelineStateObject_.graphicsPipelineState));
	assert(SUCCEEDED(hr));

}
