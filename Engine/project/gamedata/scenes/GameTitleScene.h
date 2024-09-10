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

struct TestBlock {
	Model model;
	WorldTransform world;
	Vector4 material;
	bool isFloorOrBlockHit;
	int blockType;
};

class GameTitleScene :public Iscene{
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

	void LoadAllBlockData();
	void LoadBlockPopData(int type, int RotateType);
	void SpawnCSVBlock(ModelData ObjModelData, uint32_t ObjTexture, EulerTransform transform, int blockType, Vector3 position, Vector4 color);
	void SelectSpawn(int blockType);
	void Rotate90(std::vector<std::vector<int>>& matrix_);
	//当たり判定まとめ
	void CollisionConclusion();

	void StopConnectedBlocks(TestBlock& block1);

private:
	CitrusJunosEngine* CJEngine_;
	DirectXCommon* dxCommon_;
	ViewProjection viewProjection_;
	TextureManager* textureManager_;

	Audio* audio_;
	SoundData bgm_;
	SoundData select_;
	SoundData set_;
	Input* input_;
	DebugCamera* debugCamera_;

	//
	std::unique_ptr <CreateSprite> titleSprite_;
	std::unique_ptr <CreateSprite> bgSprite_;
	std::unique_ptr <CreateSprite> sphereSprite_;
	std::unique_ptr <CreateSprite> startSprite_;
	std::unique_ptr <CreateSprite> fadeSprite_;
	EulerTransform allSpriteTransform_;
	EulerTransform sphereSpriteTransform_;
	EulerTransform allSpriteUVTransform_;
	Vector4 allSpriteMaterial_;

	Vector2 sphereSize_;

	//UI点滅用
	bool changeAlpha_ = false;
	float spriteAlpha_ = 256.0f;

	//
	float fadeAlpha_ = 0.0f;

	//
	uint32_t titleResourceNum_;
	uint32_t bgResourceNum_;
	uint32_t sphereResourceNum_;
	uint32_t startResourceNum_;

	//
	std::unique_ptr<Model> model_;
	WorldTransform worldTransformModel_;
	Vector4 modelMaterial_;

	// ブロック発生コマンド
	std::stringstream blockPopCommands;
	std::vector<std::vector<int>> matrix_;
	//ブロック
	std::unordered_map<int, std::vector<std::vector<int>>> blockDataMap_; // ブロックタイプとそのデータを保持
	int selectedBlockType = 1; // デフォルトのブロックタイプ
	std::list<TestBlock> blocks_;
	ModelData ObjModelData_;
	uint32_t ObjTexture_;
	WorldTransform world_;//配置指定用の座標
	int RotateType = 0;//Rotate
	const int maxRotateType = 3;// RotateType の範囲を定義
	const int maxType = 11;
	//Editor
	Editors* editors_;
	bool isEditorMode_ = false;

	//押し戻しの倍率
	float pushbackMultiplier_ = 0.01f;
	float pushbackMultiplierBlock_ = 1.5f;

	//Other
	std::random_device seedGenerator;
	bool isSceneStart_ = true;//ゲームスタート時の処理
	bool isNextScene_ = false;//ゲームシーンへ移行
	bool isSetBlock_ = false;//ブロックを出したか否か
};
