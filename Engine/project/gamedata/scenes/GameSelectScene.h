#pragma once
#include "Iscene.h"
#include "components/audio/Audio.h"
#include "components/input/Input.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
#include "TextureManager.h"
#include "components/2d/CreateTriangle.h"
#include "components/2d/CreateSprite.h"
#include "components/2d/CreateParticle.h"
#include "components/3d/CreateLine.h"
#include "components/3d/CreateSphere.h"
#include "components/3d/CreateSkyBox.h"
#include "components/3d/Model.h"
#include "components/debugcamera/DebugCamera.h"
#include "levelDataLoader/LevelDataLoader.h"
#include "postEffect/PostEffect.h"
#include "editors/Editors.h"

class GameSelectScene :public Iscene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void DrawUI() override;
	void DrawPostEffect() override;
	void Finalize() override;

private:
	CitrusJunosEngine* CJEngine_;
	DirectXCommon* dxCommon_;
	ViewProjection viewProjection_;
	TextureManager* textureManager_;

	Audio* audio_;
	SoundData soundData1_;
	Input* input_;
	DebugCamera* debugCamera_;

	//
	std::unique_ptr <CreateSprite> bgSprite_;
	EulerTransform allSpriteTransform_;
	EulerTransform allSpriteUVTransform_;
	Vector4 allSpriteMaterial_;

	//
	uint32_t bgResourceNum_;
};