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

#include "datas/datas.h"

struct Vertex {
	Vector3 position;//頂点の位置
	float oscillationTimer = 0.0f;//浮遊アニメーションのためのタイマー
};

class GameSelectScene :public Iscene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void DrawUI() override;
	void DrawPostEffect() override;
	void Finalize() override;

	//ゲーム開始時の処理
	void GameStartProcessing();

	//シーン終了時の処理
	void SceneEndProcessing();

	void MoveCameraToVertex(int vertexIndex, float timerSpeed, float rotationY);

	void UpdateVertices(float deltaTime);

	void CalculateDeltaTime();

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
	std::unique_ptr<Model> model_[6];
	WorldTransform worldTransformModel_[6];
	Vector4 modelMaterial_;

	//
	Vertex hexVertices_[6];
	int currentVertexIndex = 0;
	float currentRotationY = 0.0f;
	const float rotationStep = (float)M_PI / 2.0f;

	//浮遊する高さの範囲
	const float oscillationAmplitude = 0.5f;//上下に動く最大の高さ
	const float oscillationSpeed = 2.0f;//浮遊の速さ（周波数）
	const float returnSpeed = 3.0f;//Y=0に戻るスピード

	float deltaTime = 0.0f;

	//
	Datas* datas_;

	bool isGameStart_ = true;//ゲームスタート時の処理
};