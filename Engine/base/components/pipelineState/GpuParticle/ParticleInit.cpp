#include "ParticleInit.h"

void ParticleInit::ShaderCompile()
{
	csShaderBlob = ShaderCompiler::GetInstance()->CompileShader(L"project/gamedata/resources/shaders/Particle/Particle_Init.CS.hlsl", L"cs_6_0");
	assert(csShaderBlob != nullptr);

}

void ParticleInit::CreateRootSignature()
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

	//u1 : freeCountuav
	D3D12_DESCRIPTOR_RANGE descriptorRange_freeCount = {};
	descriptorRange_freeCount.BaseShaderRegister = 1;
	descriptorRange_freeCount.NumDescriptors = 1;
	descriptorRange_freeCount.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRange_freeCount.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[1].DescriptorTable.pDescriptorRanges = &descriptorRange_freeCount;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;

	//u2 : freeList
	D3D12_DESCRIPTOR_RANGE descriptorRange_freeList = {};
	descriptorRange_freeList.BaseShaderRegister = 2;
	descriptorRange_freeList.NumDescriptors = 1;
	descriptorRange_freeList.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRange_freeList.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = &descriptorRange_freeList;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = 1;

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	//rootsignatureの作成
	assert(csShaderBlob != nullptr);
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	Microsoft::WRL::ComPtr<ID3D12Device> device =DirectXCommon::GetInstance()->GetDevice();
	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&PipelineStateObject_.rootSignature));
	assert(SUCCEEDED(hr));

}

void ParticleInit::CreateInputLayOut()
{
}

void ParticleInit::CreateBlendState()
{
}

void ParticleInit::CreateRasterizarState()
{
}

void ParticleInit::CreatePipelineStateObject()
{
	Microsoft::WRL::ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->GetDevice();

	D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc = {};
	//psoの作成
	psoDesc.CS = {
	.pShaderBytecode = csShaderBlob->GetBufferPointer(),
	.BytecodeLength = csShaderBlob->GetBufferSize()
	};
	psoDesc.pRootSignature =PipelineStateObject_.rootSignature.Get();
	psoDesc.NodeMask = 0;
	hr = device->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&PipelineStateObject_.graphicsPipelineState));
	assert(SUCCEEDED(hr));

}
