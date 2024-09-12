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

class GameClearScene :public Iscene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void DrawUI() override;
	void DrawPostEffect() override;
	void Finalize() override;

	//シーン開始時の処理
	void SceneStartProcessing();
	//シーン終了時の処理
	void SceneEndProcessing();

private:
	CitrusJunosEngine* CJEngine_;
	DirectXCommon* dxCommon_;
	ViewProjection viewProjection_;
	TextureManager* textureManager_;

	Audio* audio_;
	SoundData select_;
	SoundData set_;
	Input* input_;
	DebugCamera* debugCamera_;

	//
	std::unique_ptr <CreateSprite> bgSprite_;
	std::unique_ptr <CreateSprite> startSprite_;
	std::unique_ptr <CreateSprite> fadeSprite_;
	EulerTransform allSpriteTransform_;
	EulerTransform sphereSpriteTransform_;
	EulerTransform allSpriteUVTransform_;
	Vector4 allSpriteMaterial_;

	//UI点滅用
	bool changeAlpha_ = false;
	float spriteAlpha_ = 256.0f;

	//
	float fadeAlpha_ = 256.0f;
	bool isfadeIn_ = false;

	//
	uint32_t bgResourceNum_;
	uint32_t startResourceNum_;

	//
	std::unique_ptr<Model> model_;
	WorldTransform worldTransformModel_;
	Vector4 modelMaterial_;

	//Other
	std::random_device seedGenerator;
	bool isSceneStart_ = true;//ゲームスタート時の処理
};