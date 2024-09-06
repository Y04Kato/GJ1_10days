#include "ParticleUpdate.h"

void ParticleUpdate::ShaderCompile()
{
	csShaderBlob = ShaderCompiler::GetInstance()->CompileShader(L"project/gamedata/resources/shaders/Particle/Update/Particle_Update.CS.hlsl", L"cs_6_0");
	assert(csShaderBlob != nullptr);
}

void ParticleUpdate::CreateRootSignature()
{

	D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc = {};
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	HRESULT hr = {};
	//rootsignature作成
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_ROOT_PARAMETER rootParameters[3] = {};

	//u0 : tパラメーターuav
	D3D12_DESCRIPTOR_RANGE descriptorRange = {};
	descriptorRange.BaseShaderRegister = 0;
	descriptorRange.NumDescriptors = 1;
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[0].DescriptorTable.pDescriptorRanges = &descriptorRange;
	rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;

	//u1 : FreeListIndex
	D3D12_DESCRIPTOR_RANGE descriptorRange_FreeListIndex = {};
	descriptorRange_FreeListIndex.BaseShaderRegister = 1;
	descriptorRange_FreeListIndex.NumDescriptors = 1;
	descriptorRange_FreeListIndex.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRange_FreeListIndex.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].DescriptorTable.pDescriptorRanges = &descriptorRange_FreeListIndex;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;

	//u2 : FreeList
	D3D12_DESCRIPTOR_RANGE descriptorRange_FreeList = {};
	descriptorRange_FreeList.BaseShaderRegister = 2;
	descriptorRange_FreeList.NumDescriptors = 1;
	descriptorRange_FreeList.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRange_FreeList.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = &descriptorRange_FreeList;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = 1;

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

	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();
	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&PipelineStateObject_.rootSignature));
	assert(SUCCEEDED(hr));

}

void ParticleUpdate::CreateInputLayOut()
{
}

void ParticleUpdate::CreateBlendState()
{
}

void ParticleUpdate::CreateRasterizarState()
{
}

void ParticleUpdate::CreatePipelineStateObject()
{

	D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc = {};
	//psoの作成
	psoDesc.CS = {
	.pShaderBytecode = csShaderBlob->GetBufferPointer(),
	.BytecodeLength = csShaderBlob->GetBufferSize()
	};
	psoDesc.pRootSignature = PipelineStateObject_.rootSignature.Get();
	psoDesc.NodeMask = 0;
	Microsoft::WRL::ComPtr<ID3D12Device>device=DirectXCommon::GetInstance()->GetDevice();
	hr = device->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&PipelineStateObject_.graphicsPipelineState));
	assert(SUCCEEDED(hr));

}
