#include "ParticleAddDraw.h"

void ParticleAddDraw::ShaderCompile()
{
	pixelShaderBlob =
		ShaderCompiler::GetInstance()->CompileShader(
			L"project/gamedata/resources/shaders/Particle/Draw/Particle_DebugDraw.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob != nullptr);
	vertexShaderBlob =
		ShaderCompiler::GetInstance()->CompileShader(
			L"project/gamedata/resources/shaders/Particle/Draw/Particle_DebugDraw.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);
}

void ParticleAddDraw::CreateRootSignature()
{

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	//rootsignature作成
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_ROOT_PARAMETER rootParameters[5] = {};
	//u0 : VS param uav
	D3D12_DESCRIPTOR_RANGE descriptorRangeVS_UAV[1] = {};
	descriptorRangeVS_UAV[0].BaseShaderRegister = 0;
	descriptorRangeVS_UAV[0].NumDescriptors = 1;
	descriptorRangeVS_UAV[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRangeVS_UAV[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRangeVS_UAV;
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeVS_UAV);

	//u1 : PS param uav
	D3D12_DESCRIPTOR_RANGE descriptorRangePS_UAV[1] = {};
	descriptorRangePS_UAV[0].BaseShaderRegister = 0;
	descriptorRangePS_UAV[0].NumDescriptors = 1;
	descriptorRangePS_UAV[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	descriptorRangePS_UAV[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangePS_UAV;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangePS_UAV);

	//b0 : VSに送るカメラ

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[2].Descriptor.ShaderRegister = 0;

	//b0 : PSに送るカメラ
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[3].Descriptor.ShaderRegister = 0;

	//テクスチャ
	D3D12_DESCRIPTOR_RANGE descriptorRangePS_Tex[1] = {};
	descriptorRangePS_Tex[0].BaseShaderRegister = 0;
	descriptorRangePS_Tex[0].NumDescriptors = 1;
	descriptorRangePS_Tex[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangePS_Tex[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[4].DescriptorTable.pDescriptorRanges = descriptorRangePS_Tex;
	rootParameters[4].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangePS_Tex);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	//Sampler
	staticSamplers[0].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);
	
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr))
	{
	    Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice().Get();
	hr = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&this->PipelineStateObject_.rootSignature));
	assert(SUCCEEDED(hr));

}

void ParticleAddDraw::CreateInputLayOut()
{
	inputElementDescs4[0].SemanticName = "POSITION";
	inputElementDescs4[0].SemanticIndex = 0;
	inputElementDescs4[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs4[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	//texCoord
	inputElementDescs4[1].SemanticName = "TEXCOORD";
	inputElementDescs4[1].SemanticIndex = 0;
	inputElementDescs4[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs4[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	//normal
	inputElementDescs4[2].SemanticName = "NORMAL";
	inputElementDescs4[2].SemanticIndex = 0;
	inputElementDescs4[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs4[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	//instancedId
	inputElementDescs4[3].SemanticName = "INSTANCEID";
	inputElementDescs4[3].SemanticIndex = 0;
	inputElementDescs4[3].Format = DXGI_FORMAT_R32_UINT;
	inputElementDescs4[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc.pInputElementDescs = inputElementDescs4;
	inputLayoutDesc.NumElements = _countof(inputElementDescs4);
}

void ParticleAddDraw::CreateBlendState()
{
	blendDesc[kBlendModeNone].RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	//通常ブレンド
	blendDesc[kBlendModeNormal].RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc[kBlendModeNormal].RenderTarget[0].BlendEnable = TRUE;
	blendDesc[kBlendModeNormal].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc[kBlendModeNormal].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc[kBlendModeNormal].RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc[kBlendModeNormal].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc[kBlendModeNormal].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc[kBlendModeNormal].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	//加算ブレンド
	blendDesc[kBlendModeAdd].RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc[kBlendModeAdd].RenderTarget[0].BlendEnable = TRUE;
	blendDesc[kBlendModeAdd].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc[kBlendModeAdd].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc[kBlendModeAdd].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	blendDesc[kBlendModeAdd].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc[kBlendModeAdd].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc[kBlendModeAdd].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	//減算ブレンド
	blendDesc[kBlendModeSubtract].RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc[kBlendModeSubtract].RenderTarget[0].BlendEnable = TRUE;
	blendDesc[kBlendModeSubtract].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc[kBlendModeSubtract].RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blendDesc[kBlendModeSubtract].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	blendDesc[kBlendModeSubtract].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc[kBlendModeSubtract].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc[kBlendModeSubtract].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	//乗算ブレンド
	blendDesc[kBlendModeMultiply].RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc[kBlendModeMultiply].RenderTarget[0].BlendEnable = TRUE;
	blendDesc[kBlendModeMultiply].RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
	blendDesc[kBlendModeMultiply].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc[kBlendModeMultiply].RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_COLOR;
	blendDesc[kBlendModeMultiply].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc[kBlendModeMultiply].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc[kBlendModeMultiply].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	//スクリーンブレンド
	blendDesc[kBlendModeScreen].RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc[kBlendModeScreen].RenderTarget[0].BlendEnable = TRUE;
	blendDesc[kBlendModeScreen].RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	blendDesc[kBlendModeScreen].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc[kBlendModeScreen].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	blendDesc[kBlendModeScreen].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc[kBlendModeScreen].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc[kBlendModeScreen].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

}

void ParticleAddDraw::CreateRasterizarState()
{
	//Rasterrizer設定
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
}

void ParticleAddDraw::CreatePipelineStateObject()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = PipelineStateObject_.rootSignature.Get();//RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;//Inputlayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize() };//vertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
		pixelShaderBlob->GetBufferSize() };//pixcelShader
	graphicsPipelineStateDesc.BlendState = blendDesc[kBlendModeNormal];//BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;//rasterizerState

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//どのように画面に色を打ち込むのかの設定（気にしなく良い）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化
	depthStencilDesc.DepthEnable = true;
	//書き込み
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	//比較関数はLessEqual 近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//実際に生成
	hr = DirectXCommon::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&PipelineStateObject_.graphicsPipelineState));
	assert(SUCCEEDED(hr));
}
