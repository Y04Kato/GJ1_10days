#pragma once
#include "IPipelineStateObject.h"
#include "PSOManager/PSOManager.h"

class Scanlines : public IPipelineStateObject {
public:
	void ShaderCompile() override;
	void CreateRootSignature() override;
	void CreateInputLayOut() override;
	void CreateBlendState() override;
	void CreateRasterizarState() override;
	void CreatePipelineStateObject() override;

private:

};